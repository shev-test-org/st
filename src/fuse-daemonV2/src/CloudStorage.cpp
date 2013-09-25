/*
 * CloudStorage.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#include "CloudStorage.h"

#include <string>

class Block;


CloudStorage::CloudStorage() {
	// TODO Auto-generated constructor stub

}

CloudStorage::~CloudStorage() {
	// TODO Auto-generated destructor stub
}

void CloudStorage::put(string key, char* buf, int len) {
}

Block* CloudStorage::get(string key) {
	return NULL;
}

void CloudStorage::remove(string key) {
}

void CloudStorage::dereference(string key) {
}

void CloudStorage::lock() {
}

void CloudStorage::unlock() {
}
