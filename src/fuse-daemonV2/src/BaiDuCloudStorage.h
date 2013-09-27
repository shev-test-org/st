/*
 * BaiDuCloudStorage.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef BAIDUCLOUDSTORAGE_H_
#define BAIDUCLOUDSTORAGE_H_

#include <string>

#include "CloudStorage.h"
#include "Lock.h"

class bcs_sdk;
class Block;
#define REF_TAG "x-bs-meta-ref"

class BaiDuCloudStorage: public CloudStorage {
public:
	BaiDuCloudStorage();
	virtual ~BaiDuCloudStorage();

	virtual void put(string key, char *buf, int len);
	virtual Block* get(string key);
	virtual void remove(string key);
	virtual void dereference(string key) ;
	virtual void lock();
	virtual void unlock() ;

private:
	const char *ak;
	const char *sk;
	const char *hostname;
	const char *bucket;
	bcs_sdk *sdk;

	Lock storaeLock;

public:
	static void BaiDuCloudStorageTest();
};

#endif /* BAIDUCLOUDSTORAGE_H_ */
