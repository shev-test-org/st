/*
 * Configuration.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#include "Configuration.h"

Configuration *Configuration::INSTANCE = NULL;

Configuration* Configuration::getInstance()
{
	if (NULL == INSTANCE) {
		INSTANCE = new Configuration();
	}

	return INSTANCE;
}

Configuration::Configuration() {
	// TODO Auto-generated constructor stub

}

Configuration::~Configuration() {
	// TODO Auto-generated destructor stub
}

int Configuration::getBlockSize() const {
	return blockSize;
}

void Configuration::setBlockSize(int blockSize) {
	this->blockSize = blockSize;
}

int Configuration::getDiskCacheSize() const {
	return diskCacheSize;
}

void Configuration::setDiskCacheSize(int diskCacheSize) {
	this->diskCacheSize = diskCacheSize;
}

int Configuration::getMemoryCacheSize() const {
	return memoryCacheSize;
}

void Configuration::setMemoryCacheSize(int memoryCacheSize) {
	this->memoryCacheSize = memoryCacheSize;
}

const string& Configuration::getWorkDir() const {
	return workDir;
}

void Configuration::setWorkDir(const string& workDir) {
	this->workDir = workDir;
}

int Configuration::getFPListExtendSize() const {
	return this->fpListExtendSize;
}

void Configuration::setFPListExtendSize(int fpListExtendSize) {
	this->fpListExtendSize = fpListExtendSize;
}
