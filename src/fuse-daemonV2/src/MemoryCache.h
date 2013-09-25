/*
 * MemoryCache.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef MEMORYCACHE_H_
#define MEMORYCACHE_H_

#include <map>
#include <string>
#include <utility>

#include "Lock.h"

class Block;

using namespace std;

#include "Storage.h"

class MemoryCache: public Storage {
public:
	MemoryCache();
	virtual ~MemoryCache();
	virtual void put(string key, char *buf, int len);
	virtual Block* get(string key);
	virtual void remove(string key);
	virtual void dereference(string key);
	virtual void lock() {cacheMapLock.lock();};
	virtual void unlock() {cacheMapLock.unlock();};

private:
	map<string, Block*> 	cacheMap;
	Lock				cacheMapLock;
};

#endif /* MEMORYCACHE_H_ */
