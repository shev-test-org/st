/*
 * Configuration.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include <string>
using namespace std;

class Configuration {
public :
	static string WORK_DIR = "/st";
	static uint32_t BLOCK_SIZE = 64 << 10; // 64K
	static uint32_t MEMORY_CACHE_SIZE = 1024; // 64K * 1024
	static uint32_t DISK_CACHE_SZIE = 1024;
public:
	Configuration();
	virtual ~Configuration();
};

#endif /* CONFIGURATION_H_ */
