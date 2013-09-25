/*
 * Md5.h
 *
 *  Created on: Sep 24, 2013
 *      Author: root
 */

#ifndef MD5_H_
#define MD5_H_
#include <string>
using namespace std;

class Md5 {
public:
	Md5();
	virtual ~Md5();

	static string getMd5(char *buf);
};

#endif /* MD5_H_ */
