/*
 * Storage.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef STORAGE_H_
#define STORAGE_H_
#include <string>
using namespace std;

class Block {
public:
	Block(int ref, char *buf) {
		this->ref = ref;
		this->buf = buf;
	}

	int ref;
	char *buf;
};

class Storage {
public:
	Storage();
	virtual ~Storage();

	virtual void put(string key, char *buf) = 0;
	virtual Block* get(string key) = 0;
	virtual void remove(string key) = 0;
	virtual void dereference(string key) = 0;
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

#endif /* STORAGE_H_ */
