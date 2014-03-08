
#include <pthread.h>

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class Semaphore {

	private:
		int count;

		pthread_cond_t c;
		pthread_mutex_t m;
	
	public:
		Semaphore( int );
		~Semaphore();

		void P( void );
		void V( void );

};


#endif /* SEMAPHORE_H */
