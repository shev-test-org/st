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

class Storage {
public:
	Storage();
	virtual ~Storage();
	virtual void put(string key, char *buf) = 0;
	virtual char* get(string key) = 0;
	virtual void remove(string key) = 0;
	virtual bool isExisted(string key) = 0;
	virtual int getReferenceCount() = 0;
	virtual void dereference(string key) = 0;
	virtual void reference(string key) = 0;
};

#endif /* STORAGE_H_ */
