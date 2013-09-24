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
	virtual char* get(string key);
	virtual void remove(string key);
	virtual bool isExisted(string key);
	virtual int getReferenceCount();
	virtual void dereference(string key);
	virtual void reference(string key);

private:
	map<string, char*> cacheMap;
	Lock cacheMapLock;
};

#endif /* DISKCACHE_H_ */