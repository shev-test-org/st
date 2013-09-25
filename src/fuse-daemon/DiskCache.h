/*
 * DiskCache.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef DISKCACHE_H_
#define DISKCACHE_H_

#include "Storage.h"

class DiskCache: public Storage {
public:
	DiskCache();
	virtual ~DiskCache();
	virtual void put(string key, char *buf);
	virtual Block* get(string key);
	virtual void remove(string key);
	virtual void dereference(string key);
	virtual void lock() {cacheMapLock.lock();};
	virtual void unlock() {cacheMapLock.unlock();};

private:
	string		cacheDir;
	Lock		cacheMapLock;
};

#endif /* DISKCACHE_H_ */
