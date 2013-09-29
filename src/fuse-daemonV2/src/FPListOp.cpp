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
	FPList *fpList = &fileMeta->firstFPList;

	do {
		if (fpList->startFPIndex <= index
				&& index < fpList->startFPIndex + fpList->fpCount) {
			return (FingurePoint*) ((char *) fpList->offsetToHeader
					+ (index - fpList->startFPIndex) * sizeof(FingurePoint));
		}
		fpList = (FPList *)((char *)fileMeta + fpList->nextFpListOffset);
	} while (fpList->nextFpListOffset != -1);


	return NULL;
}
