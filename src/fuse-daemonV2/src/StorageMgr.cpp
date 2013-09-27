/*
 * StorageMgr.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#include "StorageMgr.h"

#include <cassert>
#include <new>
#include <string>

#include "BaiDuCloudStorage.h"
#include "CloudStorage.h"
#include "DiskCache.h"
#include "global.h"
#include "Lock.h"
#include "MemoryCache.h"
#include "Storage.h"

class Block;

StorageMgr::StorageMgr() {
	// TODO Auto-generated constructor stub
	memoryCache = new MemoryCache();
	diskCache = new DiskCache();
	diskCache->setCacheDir("/cache_dir");
	cloudStorage = (CloudStorage *)new BaiDuCloudStorage();
}

StorageMgr::~StorageMgr() {
	// TODO Auto-generated destructor stub
	delete memoryCache;
	delete diskCache;
	delete cloudStorage;
	memoryCache = NULL;
	diskCache = NULL;
	cloudStorage = NULL;
}

void StorageMgr::put(string key, char *buf, int len)
{
	mgrLock.lock();
	memoryCache->lock();
	memoryCache->put(key, buf, len);
	memoryCache->unlock();

	diskCache->lock();
	diskCache->put(key, buf, len);
	diskCache->unlock();

	cloudStorage->lock();
	cloudStorage->put(key, buf, len);
	cloudStorage->unlock();

	mgrLock.unlock();
}

Block* StorageMgr::get(string key)
{
	Block *block = NULL;

	_try {
		mgrLock.lock();

		memoryCache->lock();
		block = memoryCache->get(key);
		memoryCache->unlock();
		if (NULL != block) {
			_try_return(block);
		}

		diskCache->lock();
		diskCache->get(key);
		diskCache->unlock();
		if (NULL != block) {
			_try_return(block);
		}

		cloudStorage->lock();
		cloudStorage->get(key);
		cloudStorage->unlock();
		if (NULL != block) {
			_try_return(block);
		}
	}_finally {
		mgrLock.unlock();
	}

	return block;
}

void StorageMgr::remove(string key)
{
	mgrLock.lock();

	memoryCache->lock();
	memoryCache->dereference(key);
	memoryCache->unlock();

	diskCache->lock();
	diskCache->dereference(key);
	diskCache->unlock();

	cloudStorage->lock();
	cloudStorage->dereference(key);
	cloudStorage->unlock();

	mgrLock.unlock();
}

void StorageMgr::StorageMgrTest()
{
	StorageMgr *storageMgr = new StorageMgr();
	storageMgr->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	storageMgr->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	storageMgr->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	storageMgr->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	Block *block = storageMgr->get("781e5e245d69b566979b86e28d23f2c7");
	assert(4 == block->ref);

	storageMgr->remove("781e5e245d69b566979b86e28d23f2c7");
	storageMgr->remove("781e5e245d69b566979b86e28d23f2c7");
	storageMgr->remove("781e5e245d69b566979b86e28d23f2c7");
	storageMgr->remove("781e5e245d69b566979b86e28d23f2c7");

	storageMgr->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	block = storageMgr->get("781e5e245d69b566979b86e28d23f2c7");
	assert(1 == block->ref);

}
