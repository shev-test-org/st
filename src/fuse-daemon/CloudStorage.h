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

	virtual void put(string key, char *buf);
	virtual char* get(string key);
	virtual void remove(string key);
	virtual bool isExisted(string key);
	virtual int getReferenceCount();
	virtual void dereference(string key);
	virtual void reference(string key);
};

#endif /* CLOUDSTORAGE_H_ */
