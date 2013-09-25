/*
 * Lock.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef LOCK_H_
#define LOCK_H_

class Lock {
public:
	Lock();
	virtual ~Lock();

	void lock() {};
	void unlock() {};
};

#endif /* LOCK_H_ */
