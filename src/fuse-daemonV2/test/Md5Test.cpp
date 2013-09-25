/*
 * Md5Test.cpp
 *
 *  Created on: Sep 25, 2013
 *      Author: root
 */

#include "Md5Test.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>

using namespace std;
#include "../src/Md5.h"


Md5Test::Md5Test() {
	// TODO Auto-generated constructor stub

}

Md5Test::~Md5Test() {
	// TODO Auto-generated destructor stub
}

void Md5Test::doTest()
{
	string str = "0123456789";
	MD5 md5(str);
	printf(md5.md5().c_str());
	assert(md5.md5().compare("781e5e245d69b566979b86e28d23f2c7") == 0);
}
