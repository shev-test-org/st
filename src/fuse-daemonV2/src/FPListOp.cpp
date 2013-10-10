/*
 * FPListOp.cpp
 *
 *  Created on: Sep 29, 2013
 *      Author: root
 */

#include "FPListOp.h"

FingurePoint* getFP(FileMeta *fileMeta, int index)
{
	assert(NULL != fileMeta);
	assert(index >= 0);
	FPList *fpList = &fileMeta->firstFPList;

	do {
		if (fpList->startFPIndex <= index
				&& index < fpList->startFPIndex + fpList->fpCount) {
			return (FingurePoint*) ((char *) fpList->offsetToHeader + sizeof(FPList)
					+ (index - fpList->startFPIndex) * sizeof(FingurePoint));
		}
		fpList = (FPList *)((char *)fileMeta + fpList->nextFpListOffset);
	} while (fpList->nextFpListOffset != -1);


	return NULL;
}

void extendFPList(int fd, FileMeta *fileMeta, int index) {
	assert(NULL != fileMeta);
	assert(index >= 0);
	FPList *fpList = &fileMeta->firstFPList;

	Configuration* config =  Configuration::getInstance();

	// Find the smallest one from all indexes which are bigger than the request index.
	unsigned int upIndex = -1;  // maximum int
	do {
		if (fpList->startFPIndex > index && fpList->startFPIndex < upIndex) {
			upIndex = fpList->startFPIndex;
		}
		fpList = (FPList *)((char *)fileMeta + fpList->nextFpListOffset);
	} while (fpList->nextFpListOffset != -1);

	fpList = &fileMeta->firstFPList;
	int fpCount = min(config->getFPListExtendSize(), upIndex - index);

	FPList *new_fpList = new FPList(fileMeta->header.metaSize, index, fpCount);
	memset((void*)new_fpList->offsetToHeader + sizeof(FPList), 0, new_fpList->fpCount * sizeof(FingurePoint));

	new_fpList->nextFpListOffset = fpList;
	fileMeta->firstFPList = new_fpList;

	lseek(fd, fileMeta->header.metaSize, SEEK_SET);
	pwrite(fd, new_fpList, sizeof(FPList) + fpCount * sizeof(FingurePoint), 0);
	fileMeta->header.metaSize += sizeof(FPList) + fpCount * sizeof(FingurePoint);
}
