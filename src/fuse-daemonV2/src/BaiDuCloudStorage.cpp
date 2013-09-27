/*
 * BaiDuCloudStorage.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#include "BaiDuCloudStorage.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include <iostream>
#include <map>
#include <memory>
#include <new>
#include <string>
#include <utility>

#include "../inc/bcs_sdk.h"
#include "Lock.h"
#include "Storage.h"

class Block;

BaiDuCloudStorage::BaiDuCloudStorage() {
	// TODO Auto-generated constructor stub
	ak = "iLsryT9U4HdhbOCOBGf0K7oM";
	sk = "NzY52ezn9fP0nluj24wG5LP7i5T8H3B4";
	hostname = "bcs.duapp.com";
	bucket = "buck222222";
	sdk = new bcs_sdk(ak, sk, hostname);
	assert(sdk);
}

BaiDuCloudStorage::~BaiDuCloudStorage() {
	// TODO Auto-generated destructor stub
	delete sdk;
	sdk = NULL;
}

void BaiDuCloudStorage::put(string key, char* buf, int len) {
	int r = 0;

	assert(buf && len);
	response resp;
	map <string, string> header;
	string content = string(buf, len);

	r = sdk->head_object(bucket, key.c_str(), resp);
	if (r) {
		header.insert(pair<string, string>(REF_TAG, "1"));
	} else {
		int ref_count = atoi(resp.header[REF_TAG].c_str()) + 1;
		char tmp[64] = {0};
		sprintf(tmp, "%d", ref_count);
		header[REF_TAG] = tmp;
	}

	r = sdk->put_object_from_memory(bucket, key.c_str(), content, resp, &header);
	assert(r == 0);

	r = sdk->head_object(bucket, key.c_str(), resp);
	assert(r == 0);

}

Block* BaiDuCloudStorage::BaiDuCloudStorage::get(string key) {

	int r = 0;
	response resp;
	r = sdk->get_object_to_memory(bucket, key.c_str(), resp);
	if (r) {
		return NULL;
	}

	int ref = atoi(resp.header[REF_TAG].c_str());
	int len = resp.body.length();
	char *buf = new char[len];
	memcpy(buf, resp.body.data(), len);

	Block *block = new Block(ref, buf, len);
	return block;
}

void BaiDuCloudStorage::remove(string key) {
	int r = 0;
	response resp;

	r = sdk->delete_object(bucket, key.c_str(), resp);
	assert(r == 0);
}

void BaiDuCloudStorage::dereference(string key) {
	int r = 0;
	int ref = 0;
	response resp;
	map <string, string> header;

	r = sdk->get_object_to_memory(bucket, key.c_str(), resp);
	ref = atoi(resp.header[REF_TAG].c_str());
	ref --;
	if (ref == 0) {
		remove(key);
	} else {
		char tmp[64] = {0};
		sprintf(tmp, "%d", ref);
		header[REF_TAG] = tmp;
		string content(resp.body);
		r = sdk->put_object_from_memory(bucket, key.c_str(), resp.body, resp, &header);
		assert(r == 0);
	}
}

void BaiDuCloudStorage::lock() {
	storaeLock.lock();
}

void BaiDuCloudStorage::unlock() {
	storaeLock.unlock();
}

void bcs_sdk_test() {
	map<string, string> header;
	map<string, string> opt;
	response res;
	int ret;
	char *ak = "iLsryT9U4HdhbOCOBGf0K7oM";
	char *sk = "NzY52ezn9fP0nluj24wG5LP7i5T8H3B4";
	char bucket[130] = "buck222222";
	char *host = "bcs.duapp.com";
	bcs_sdk *bcs = new bcs_sdk(ak, sk, host);
	bcs->create_bucket(bucket, res);
}

void BaiDuCloudStorage::BaiDuCloudStorageTest()
{
	printf("BaiDuCloudStorage::BaiDuCloudStorageTest() started.\n");
	// prepare
	BaiDuCloudStorage *cloud = new BaiDuCloudStorage();
	Block *block = cloud->get("781e5e245d69b566979b86e28d23f2c7");
	if (NULL != block) {
		cloud->remove("781e5e245d69b566979b86e28d23f2c7");
		delete block;
		block = NULL;
	}

	cloud->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	block = cloud->get("781e5e245d69b566979b86e28d23f2c7");
	cloud->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	cloud->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	cloud->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	block = cloud->get("781e5e245d69b566979b86e28d23f2c7");
	assert(4 == block->ref);
	delete block;
	block = NULL;

	cloud->dereference("781e5e245d69b566979b86e28d23f2c7");
	cloud->dereference("781e5e245d69b566979b86e28d23f2c7");
	cloud->dereference("781e5e245d69b566979b86e28d23f2c7");
	block = cloud->get("781e5e245d69b566979b86e28d23f2c7");
	assert(1 == block->ref);
	delete block;
	block = NULL;

	printf("DiskCache::DiskCacheTest() OK.\n");
}
