
#include <errno.h>
#include <pthread.h>

#include "Semaphore.h"

#define C_PTR &(this->c)
#define M_PTR &(this->m)

#define LOCK_M		pthread_mutex_lock( &(this->m) )
#define UNLOCK_M	pthread_mutex_unlock( &(this->m) )


Semaphore::Semaphore( int i ) {
	this->count = i;

	errno = pthread_mutex_init( M_PTR, NULL );
	errno = pthread_cond_init( C_PTR, NULL );
}

Semaphore::~Semaphore( void ) {
	// free all Ps with while(?) V(); 	???
	int error = 0;
	if( error = pthread_cond_destroy( C_PTR ) ) {
		// TODO throw error
	}
	if( error = pthread_mutex_destroy( M_PTR ) ) {
		// TODO throw error;
	}
}

void Semaphore::P( void ) {
	// decrement
	LOCK_M;
	while( this->count == 0 ) {
		pthread_cond_wait( C_PTR, M_PTR );
	}
	this->count--;
	UNLOCK_M;
}

void Semaphore::V( void ) {
	// increment
	LOCK_M;
	this->count++;
	pthread_cond_broadcast( C_PTR );
	UNLOCK_M;
}



