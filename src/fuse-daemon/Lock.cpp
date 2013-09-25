/*
 * Lock.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: root
 */

#include "Lock.h"

Lock::Lock() {
	// TODO Auto-generated constructor stub
	owner = 0;
	pthread_mutex_init(&mutex, 0);
}

Lock::~Lock() {
	// TODO Auto-generated destructor stub
}

int Lock::lock()
{
    int r = pthread_mutex_lock(&mutex);
    owner = pthread_self();
    return r;
}

int Lock::unlock()
{
    int r = pthread_mutex_unlock(&mutex);
    owner = 0;
    return r;
}

int Lock::try_lock()
{
	int r = pthread_mutex_trylock(&mutex);
	if (0 == r) {
		owner = pthread_self();
	}
	return r;
}

pthread_t Lock::get_owner()
{
	return owner;
}



