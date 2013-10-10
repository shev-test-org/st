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
public:
	static Configuration *getInstance();
	virtual ~Configuration();
	int getBlockSize() const;
	void setBlockSize(int blockSize);
	int getDiskCacheSize() const;
	void setDiskCacheSize(int diskCacheSize);
	int getMemoryCacheSize() const;
	void setMemoryCacheSize(int memoryCacheSize);
	const string& getWorkDir() const;
	void setWorkDir(const string& workDir);
	int Configuration::getFPListExtendSize() const;
	void Configuration::setFPListExtendSize(int fpListExtendSize);

protected:
	static Configuration *INSTANCE;
	Configuration();

private:
	string workDir;
	int blockSize;
	int memoryCacheSize;
	int diskCacheSize;
	int fpListExtendSize;

};

#endif /* CONFIGURATION_H_ */
