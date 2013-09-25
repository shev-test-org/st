/*
 * DiskCache.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef DISKCACHE_H_
#define DISKCACHE_H_

#include <string>

#include "Lock.h"
#include "Storage.h"

class Block;

class DiskCache: public Storage {
public:
	DiskCache();
	virtual ~DiskCache();
	virtual void put(string key, char *buf, int len);
	virtual Block* get(string key);
	virtual void remove(string key);
	virtual void dereference(string key);
	virtual void lock() {cacheMapLock.lock();};
	virtual void unlock() {cacheMapLock.unlock();};

private:
	int getRefCount(int fd);
	void setRefCount(int fd, int ref);

	void getBuf(int fd, char *buf, int len);
	void setBuf(int fd, char *buf, int len);

public:
	static void DiskCacheTest();
	const string& getCacheDir() const;
	void setCacheDir(const string& cacheDir);

private:
	string		cacheDir;
	Lock		cacheMapLock;
};

#endif /* DISKCACHE_H_ */
