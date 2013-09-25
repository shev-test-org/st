/*
 * MemoryCache.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#include "MemoryCache.h"

#include <cassert>
#include <cstdio>
#include <map>
#include <new>
#include <string>
#include <utility>

#include "Md5.h"
#include "Storage.h"

using namespace std;
class Block;

MemoryCache::MemoryCache() {
	// TODO Auto-generated constructor stub
	cacheMap.clear();
}

MemoryCache::~MemoryCache() {
	// TODO Auto-generated destructor stub
}


void MemoryCache::put(string key, char *buf, int len)
{
	string md5 = MD5(buf, len).md5();
	assert(key.compare(md5) == 0);
	Block *block = NULL;
	block = get(key);
	if (NULL == block) {
		block = new Block(1, buf, len);
		cacheMap.insert(pair<string, Block*>(key, block));
	} else {
		block->ref ++;
		assert(block->ref >= 0);
	}
}

Block* MemoryCache::get(string key)
{
	map<string, Block*>::iterator it = cacheMap.find(key);
	if (it != cacheMap.end()) {
		return it->second;
	}

	return NULL;
}

void MemoryCache::remove(string key)
{
	cacheMap.erase(key);
}

void MemoryCache::dereference(string key)
{
	Block *block = get(key);
	assert(block != NULL);

	block->ref --;
	assert(block->ref >= 0);
	if (0 == block->ref) {
		remove(key);
	}
}

void MemoryCache::MemoryCacheTest()
{
	printf("MemoryCache::MemoryCacheTest() started.\n");
	MemoryCache *cache = new MemoryCache();
	cache->lock();
	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);
	cache->put("781e5e245d69b566979b86e28d23f2c7", "0123456789", 10);

	Block *block = cache->get("781e5e245d69b566979b86e28d23f2c7");
	assert(NULL != block);
	assert(block->ref == 4);
	cache->dereference("781e5e245d69b566979b86e28d23f2c7");
	cache->dereference("781e5e245d69b566979b86e28d23f2c7");
	cache->dereference("781e5e245d69b566979b86e28d23f2c7");
	cache->dereference("781e5e245d69b566979b86e28d23f2c7");

	block = cache->get("781e5e245d69b566979b86e28d23f2c7");
	assert(NULL == block);

	cache->unlock();

	delete cache;
	cache = NULL;


	printf("MemoryCache::MemoryCacheTest() OK.\n");
}
