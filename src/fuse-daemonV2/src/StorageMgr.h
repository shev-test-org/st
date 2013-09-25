/*
 * StorageMgr.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef STORAGEMGR_H_
#define STORAGEMGR_H_

#include <string>

#include "CloudStorage.h"
#include "DiskCache.h"
#include "MemoryCache.h"

using namespace std;

class StorageMgr {
public:
	StorageMgr();
	virtual ~StorageMgr();

	void put(string key, char *buf);
	char* get(string key);
	void remove(string key);
private:
	MemoryCache memoryCache;
	DiskCache diskCache;
	CloudStorage cloudStorage;
};

#endif /* STORAGEMGR_H_ */
