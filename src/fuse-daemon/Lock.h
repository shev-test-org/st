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

	int lock();
	int unlock();
private:
	pthread_mutex_t mutex;
	pthread_t owner;
};

#endif /* LOCK_H_ */
