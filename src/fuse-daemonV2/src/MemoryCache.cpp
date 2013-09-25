/*
 * MemoryCache.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#include "MemoryCache.h"

#include <cassert>
#include <map>
#include <new>
#include <string>
#include <utility>

#include "md5.h"

#include "Storage.h"

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
	char md5[33] = {0};
	assert(key.compare(md5) == 0);
	Block *block = NULL;
	block = get(key);
	if (NULL == block) {
		block = new Block(1, buf);
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
