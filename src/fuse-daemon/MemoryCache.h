/*
 * MemoryCache.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef MEMORYCACHE_H_
#define MEMORYCACHE_H_

#include <string>
#include <map>
using namespace std;

#include "Storage.h"

class MemoryCache: public Storage {
public:
	MemoryCache();
	virtual ~MemoryCache();
	virtual void put(string key, char *buf);
	virtual char* get(string key);
	virtual void remove(string key);
	virtual bool isExisted(string key);
	virtual int getReferenceCount();
	virtual void dereference(string key);
	virtual void reference(string key);

private:
	map<string, char*> 	cacheMap;
	Lock				cacheMapLock;
};

#endif /* MEMORYCACHE_H_ */
