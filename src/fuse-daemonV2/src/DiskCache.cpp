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
	mkdir(cacheDir.c_str(), 0666);
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

	if (F_OK == access(key.c_str(), F_OK)) {
		fd = open(filename.c_str(), O_RDONLY);
		assert(fd > 0);

		sz = fgetxattr(fd, REF_ATTR, &ref, sizeof(int));
		assert(sz == sizeof(int));
		assert(ref > 0);

		ref++;
		assert(ref > 0);

		sz = fsetxattr(fd, REF_ATTR, &ref, sizeof(int), XATTR_REPLACE);
		assert(sz == sizeof(int));
	} else {
		fd = open(filename.c_str(), O_CREAT);
		assert(fd > 0);

		ref = 1;
		sz = fsetxattr(fd, REF_ATTR, &ref, sizeof(int), XATTR_CREATE);
		assert(sz == sizeof(int));
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
	assert(fd > 0);

	sz = fgetxattr(fd, REF_ATTR, &ref, sizeof(int));
	assert(sz == sizeof(int));
	assert(ref > 0);

	struct stat st;
	memset(&st, 0x00, sizeof(st));
	r = fstat(fd, &st);
	assert(r == 0);

	char *buf = new char[st.st_size];
	memset(buf, 0x00, st.st_size);
	r = read(fd, buf, st.st_size);
	assert(r == st.st_size);

	Block *block = new Block(ref, buf);

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
	fd = open(filename.c_str(), O_RDONLY);
	assert(fd > 0);

	sz = fgetxattr(fd, REF_ATTR, &ref, sizeof(int));
	assert(sz == sizeof(int));
	assert(ref > 0);

	ref--;
	assert(ref > 0);

	sz = fsetxattr(fd, REF_ATTR, &ref, sizeof(int), XATTR_REPLACE);
	assert(sz == sizeof(int));

	close(fd);
	fd = 0;
}

