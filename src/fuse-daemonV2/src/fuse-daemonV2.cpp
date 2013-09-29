/*
 FUSE: Filesystem in Userspace
 Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
 Copyright (C) 2011       Sebastian Pipping <sebastian@pipping.org>

 This program can be distributed under the terms of the GNU GPL.
 See the file COPYING.

 gcc -Wall fusexmp.c `pkg-config fuse --cflags --libs` -o fusexmp
 */

#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../test/Md5Test.h"
#ifdef linux
/* For pread()/pwrite()/utimensat() */
#define _XOPEN_SOURCE 700
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <assert.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif
#include "MemoryCache.h"
#include "DiskCache.h"
#include "BaiDuCloudStorage.h"
#include "StorageMgr.h"
#include "../inc/bcs_sdk.h"
#include "global.h"
#include "filelock.h"
#include "FPListOp.h"

// global parameters
static StorageMgr *storageMgr = NULL;

static int xmp_getattr(const char *path, struct stat *stbuf) {
	int res;

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_access(const char *path, int mask) {
	int res;

	res = access(path, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size) {
	int res;

	res = readlink(path, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info *fi) {
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(path);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev) {
	int res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	 is more portable */
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode) {
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path) {
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path) {
	int res;

	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_symlink(const char *from, const char *to) {
	int res;

	res = symlink(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to) {
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_link(const char *from, const char *to) {
	int res;

	res = link(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode) {
	int res;

	res = chmod(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid) {
	int res;

	res = lchown(path, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size) {
	int res;

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}

#ifdef HAVE_UTIMENSAT
static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;

	/* don't use utime/utimes since they follow symlinks */
	res = utimensat(0, path, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
	return -errno;

	return 0;
}
#endif

static int xmp_open(const char *path, struct fuse_file_info *fi) {
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi) {
	int fd;
	int res;
	bool locked = false;
	struct stat st = {0};
	FileMeta *fileMeta = NULL;
	(void) fi;

	_try {
		fd = open(path, O_RDONLY);
		if (fd == -1) {
			try_return(res = -errno);
		}

		res = read_lock(fd);
		locked = true;

		res = fstat(fd, &st);
		if (res == -1) {
			try_return(res = -errno);
		}
		if (st.st_size == 0) {
			try_return(res = -ENOBUFS);
		}
		assert(st.st_size > sizeof(FileMeta));

		fileMeta = (FileMeta *)new char[st.st_size];
		res = pread(fd, (char *)fileMeta, st.st_size, 0);
		if (res == -1) {
			try_return(res = -errno);
		}

		if (offset >= fileMeta->header.fileSize) {
			try_return(res = ENOBUFS);
		}

		// if in smallBuf
		if (fileMeta->header.fileSize <= SMALL_FILE_SIZE) {
			assert(st.st_size == sizeof(FileMeta));

			if (offset + size <= fileMeta->header.fileSize) {
				memcpy(buf,fileMeta->smallBuf + offset, size);
				try_return(res = size);
			}

			if (offset + size > fileMeta->header.fileSize) {
				memcpy(buf, fileMeta->smallBuf + offset, SMALL_FILE_SIZE - offset);
				try_return(res = SMALL_FILE_SIZE - offset);
			}
		} else {
			int startOffset = offset / fileMeta->header.blockSize * fileMeta->header.blockSize;

			char *p = buf;
			int leftSize = size;
			off_t fileOffset = offset;
			res = 0;
			//
			while(startOffset < fileMeta->header.fileSize || leftSize == 0) {
				assert(startOffset <= fileOffset);
				FingurePoint *fp = getFP(fileMeta, startOffset / fileMeta->header.blockSize);
				Block *block = storageMgr->get(fp->md5);
				assert(NULL != block);
				assert(block->len == fileMeta->header.blockSize);
				if (startOffset + fileMeta->header.blockSize >= fileOffset + leftSize) {
					memcpy(p, block->buf + fileOffset - startOffset,
							leftSize);
					res += leftSize;
					p += leftSize;
					fileOffset += leftSize;
					leftSize = 0;
				} else {
					memcpy(p, block->buf + fileOffset - startOffset,
							fileMeta->header.blockSize);
					res += fileMeta->header.blockSize;
					p += fileMeta->header.blockSize;
					fileOffset += fileMeta->header.blockSize;
					leftSize -= fileMeta->header.blockSize;
				}

				delete block;
				block = NULL;

				startOffset += fileMeta->header.blockSize;
			}
		}

	} _finally {
		if (NULL != fileMeta) {
			delete[] fileMeta;
			fileMeta = NULL;
		}

		if (locked) {
			un_lock(fd);
		}

		if (fd != -1) {
			close(fd);
		}
	}

	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		off_t offset, struct fuse_file_info *fi) {


		return 0;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf) {
	int res;

	res = statvfs(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_release(const char *path, struct fuse_file_info *fi) {
	/* Just a stub.	 This method is optional and can safely be left
	 unimplemented */

	(void) path;
	(void) fi;
	return 0;
}

static int xmp_fsync(const char *path, int isdatasync,
		struct fuse_file_info *fi) {
	/* Just a stub.	 This method is optional and can safely be left
	 unimplemented */

	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

#ifdef HAVE_POSIX_FALLOCATE
static int xmp_fallocate(const char *path, int mode,
		off_t offset, off_t length, struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;

	if (mode)
	return -EOPNOTSUPP;

	fd = open(path, O_WRONLY);
	if (fd == -1)
	return -errno;

	res = -posix_fallocate(fd, offset, length);

	close(fd);
	return res;
}
#endif

#ifdef HAVE_SETXATTR
/* xattr operations are optional and can safely be left unimplemented */
static int xmp_setxattr(const char *path, const char *name, const char *value,
		size_t size, int flags) {
	int res = lsetxattr(path, name, value, size, flags);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_getxattr(const char *path, const char *name, char *value,
		size_t size) {
	int res = lgetxattr(path, name, value, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_listxattr(const char *path, char *list, size_t size) {
	int res = llistxattr(path, list, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_removexattr(const char *path, const char *name) {
	int res = lremovexattr(path, name);
	if (res == -1)
		return -errno;
	return 0;
}
#endif /* HAVE_SETXATTR */

static struct fuse_operations xmp_oper;
void test();

static void* xmp_init (struct fuse_conn_info *conn)
{
	storageMgr = new StorageMgr();
	return conn;
}

int main(int argc, char *argv[]) {
	memset(&xmp_oper, 0x00, sizeof(xmp_oper));
	xmp_oper.getattr = xmp_getattr;
	xmp_oper.access = xmp_access;
	xmp_oper.readlink = xmp_readlink;
	xmp_oper.readdir = xmp_readdir;
	xmp_oper.mknod = xmp_mknod;
	xmp_oper.mkdir = xmp_mkdir;
	xmp_oper.symlink = xmp_symlink;
	xmp_oper.unlink = xmp_unlink;
	xmp_oper.rmdir = xmp_rmdir;
	xmp_oper.rename = xmp_rename;
	xmp_oper.link = xmp_link;
	xmp_oper.chmod = xmp_chmod;
	xmp_oper.chown = xmp_chown;
	xmp_oper.truncate = xmp_truncate;
#ifdef HAVE_UTIMENSAT
	xmp_oper.utimens = xmp_utimens;
#endif
	xmp_oper.open = xmp_open;
	xmp_oper.read = xmp_read;
	xmp_oper.write = xmp_write;
	xmp_oper.statfs = xmp_statfs;
	xmp_oper.release = xmp_release;
	xmp_oper.fsync = xmp_fsync;
#ifdef HAVE_POSIX_FALLOCATE
	xmp_oper.fallocate = xmp_fallocate;
#endif
#ifdef HAVE_SETXATTR
	xmp_oper.setxattr = xmp_setxattr;
	xmp_oper.getxattr = xmp_getxattr;
	xmp_oper.listxattr = xmp_listxattr;
	xmp_oper.removexattr = xmp_removexattr;
#endif
	xmp_oper.init = xmp_init;

	umask(0);
	//return fuse_main(argc, argv, &xmp_oper, NULL);

	test();
}



void test() {
	// md5 test
	//Md5Test::doTest();

	// memcache test
	//MemoryCache::MemoryCacheTest();

	// diskcache test
	//DiskCache::DiskCacheTest();

	// BaiDuCloud test
	//BaiDuCloudStorage::BaiDuCloudStorageTest();

	// StorageMgr test
	StorageMgr::StorageMgrTest();
}
