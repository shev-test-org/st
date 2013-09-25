/*
 * Md5Test.cpp
 *
 *  Created on: Sep 25, 2013
 *      Author: root
 */

#include "Md5Test.h"

//#include <cassert>
#include <cstdio>
//#include <cstring>
#include <string>
using namespace std;
#include "../src/md5.h"


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
	//assert(strcmp("3749f52bb326ae96782b42dc0a97b4c1", md5) == 0);
}
