/*
 * CloudStorage.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef CLOUDSTORAGE_H_
#define CLOUDSTORAGE_H_

#include "Storage.h"

class CloudStorage: public Storage {
public:
	CloudStorage();
	virtual ~CloudStorage();

	virtual void put(string key, char *buf, int len);
	virtual Block* get(string key);
	virtual void remove(string key);
	virtual void dereference(string key);
	virtual void lock();
	virtual void unlock();

};

#endif /* CLOUDSTORAGE_H_ */
