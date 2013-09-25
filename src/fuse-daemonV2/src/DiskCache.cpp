/*
 * DiskCache.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#include "DiskCache.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <unistd.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <memory>
#include <new>
#include <string>

#include "Storage.h"

//#include "Lock.h"

class Block;
#define REF_ATTR "ref_attr"

DiskCache::DiskCache() {
	// TODO Auto-generated constructor stub
}

DiskCache::~DiskCache() {
	// TODO Auto-generated destructor stub
}

void DiskCache::put(string key, char *buf, int len) {
	int r = 0;
	ssize_t sz = 0;
	int ref = 0;
	int fd = 0;
	string filename = (cacheDir + "/" + key);

	if (F_OK == access(filename.c_str(), F_OK)) {
		fd = open(filename.c_str(), O_RDWR);
		assert(fd > 0);

		ref = getRefCount(fd);
		assert(ref > 0);
		ref++;
		setRefCount(fd, ref);
	} else {
		fd = open(filename.c_str(), O_CREAT | O_RDWR);
		if (fd <= 0) {
			perror("open");
		}
		assert(fd > 0);

		ref = 1;
		setRefCount(fd, ref);
		setBuf(fd, buf, len);
	}

	close(fd);
	fd = 0;
}

// the return block and its buf need to be freed outside.
Block* DiskCache::get(string key) {
	int r = 0;
	ssize_t sz = 0;
	int ref = 0;
	string filename = (cacheDir + "/" + key);
	int fd = 0;
	fd = open(filename.c_str(), O_RDONLY);
	if (fd <= 0) {
		return NULL;
	}

	ref = getRefCount(fd);

	struct stat st;
	memset(&st, 0x00, sizeof(st));
	r = fstat(fd, &st);
	assert(r == 0);

	char *buf = new char[st.st_size - sizeof(int)];
	memset(buf, 0x00, st.st_size - sizeof(int));
	getBuf(fd, buf, st.st_size - sizeof(int));

	Block *block = new Block(ref, buf, st.st_size - sizeof(int));

	close(fd);
	fd = 0;

	return block;
}

void DiskCache::remove(string key) {
	string filename = (cacheDir + "/" + key);

	::remove(filename.c_str());
}

void DiskCache::dereference(string key) {
	int r = 0;
	ssize_t sz = 0;
	int ref = 0;
	string filename = (cacheDir + "/" + key);
	int fd = 0;
	fd = open(filename.c_str(), O_RDWR);
	assert(fd > 0);
	ref = getRefCount(fd);
	assert(ref > 0);

	ref--;
	if (ref > 0) {
		setRefCount(fd, ref);
	} else {
		unlink(filename.c_str());
	}

	close(fd);
	fd = 0;
}



const string& DiskCache::getCacheDir() const {
	return cacheDir;
}

void DiskCache::setCacheDir(const string& cacheDir) {
	this->cacheDir = cacheDir;
	mkdir(cacheDir.c_str(), 0666);
}


int DiskCache::getRefCount(int fd)
{
	assert(fd > 0);
	int ref = 0;
	lseek(fd, 0, SEEK_SET);
	int r = read(fd, &ref, sizeof(int));
	assert(r == sizeof(int));

	return ref;
}

void DiskCache::setRefCount(int fd, int ref)
{
	assert(fd > 0);
	lseek(fd, 0, SEEK_SET);
	int r = write(fd, &ref, sizeof(int));
	assert(r == sizeof(int));
	fsync(fd);
}

void DiskCache::getBuf(int fd, char *buf, int len)
{
	assert(buf);
	assert(len);

	lseek(fd, sizeof(int), SEEK_SET);
	int r = read(fd, buf, len);
	assert(r == len);
}

void DiskCache::setBuf(int fd, char *buf, int len)
{
	assert(buf);
	assert(len);

	lseek(fd, sizeof(int), SEEK_SET);
	int r = write(fd, buf, len);
	assert(r == len);
	fsync(fd);
}

void DiskCache::DiskCacheTest()
{
	printf("DiskCache::DiskCacheTest() started.\n");
	// prepare
	unlink("/cache_dir/781e5e245d69b566979b86e28d23f2c7");
	rmdir("/cache_dir");
	DiskCache *cache = new DiskCache();
	cache->setCacheDir("/cache_dir");
	cache->lock();
	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);

	Block *block = cache->get("781e5e245d69b566979b86e28d23f2c7");
	assert(NULL != block);
	assert(block->ref == 4);
	delete block;
	block = NULL;

	cache->dereference("781e5e245d69b566979b86e28d23f2c7");
	cache->dereference("781e5e245d69b566979b86e28d23f2c7");
	cache->dereference("781e5e245d69b566979b86e28d23f2c7");
	cache->dereference("781e5e245d69b566979b86e28d23f2c7");

	block = cache->get("781e5e245d69b566979b86e28d23f2c7");
	assert(NULL == block);

	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	assert(F_OK == access("/cache_dir/781e5e245d69b566979b86e28d23f2c7", F_OK));
	cache->unlock();

	delete cache;
	cache = NULL;

	printf("DiskCache::DiskCacheTest() OK.\n");
}
