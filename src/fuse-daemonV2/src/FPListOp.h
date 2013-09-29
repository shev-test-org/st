/*
 * FPListOp.h
 *
 *  Created on: Sep 29, 2013
 *      Author: root
 */

#ifndef FPLISTOP_H_
#define FPLISTOP_H_

#include "global.h"
#include <assert.h>
#include <stdio.h>


bool mergeFPList(file_header_t *header, FPList *l1, FPList *l2) {
	assert(NULL != header);
	assert(NULL != l1);
	assert(NULL != l2);

	if (l1->offsetToHeader + l1->fpCount * sizeof(FingurePoint)
			== l2->offsetToHeader
			&& l1->startFPIndex + l1->fpCount == l2->fpCount) {
		l1->fpCount += l2->fpCount;
		return true;

	}
}

FingurePoint* getFP(file_header_t *header, int index)
{
	assert(NULL != header);
	for (int i=0; i<header->fpListCount; i++) {
		FPList *fpList = header->fpList + i;
		assert(fpList);

		if (fpList->startFPIndex <= index &&
				index < fpList->startFPIndex + fpList->fpCount) {
			return (FingurePoint*)((char *)fpList->offsetToHeader + (index - fpList->startFPIndex) * sizeof(FingurePoint));
		}
	}

	return NULL;
}

#endif /* FPLISTOP_H_ */
