/*
 * global.h
 *
 *  Created on: Sep 27, 2013
 *      Author: root
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <stdint.h>

#define _try
#define _finally _quit:;
#define try_return(s) do {s; goto _quit;} while(0)

#define MAGIC 'STBL'
#define SMALL_FILE_SIZE 3 << 10
#pragma pack (1)
typedef struct _FPList {
	int offsetToHeader;
	uint64_t startFPIndex;
	int fpCount;

	_FPList(int offsetToHeader, uint64_t startFPIndex, int fpCoun) {
		this->offsetToHeader = offsetToHeader;
		this->startFPIndex = startFPIndex;
		this->fpCount = fpCount;
	}
}FPList;

typedef struct _file_header {
	int magic; //'STBL'
	int fileSize;
	int blockSize;
	int blockNum;
	int crc;
	char smallBuf[SMALL_FILE_SIZE];

	// FP list ...
	int fpListCount;
	FPList fpList[1];

}file_header_t;

typedef struct _FingerPoint {
	char md5[33];
}FingurePoint;

#pragma pack ()

#define min(a, b) a > b ? b : a
#define max(a, b) a > b ? a : b

#endif /* GLOBAL_H_ */
