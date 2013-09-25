/*
 * Lock.h
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#ifndef LOCK_H_
#define LOCK_H_

#include <pthread.h>

class Lock {
public:
	Lock();
	virtual ~Lock();

	int lock();
	int unlock();

	int try_lock();

	pthread_t get_owner();

private:
	pthread_mutex_t mutex;
	pthread_t owner;
};

#endif /* LOCK_H_ */
