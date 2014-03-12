
#include <pthread.h>

#include "AtomicBool.h"

AtomicBool::AtomicBool( bool init ) {
    this->state = init;
    pthread_mutex_init( &(this->mutex), NULL );
}

AtomicBool::~AtomicBool() {
    pthread_mutex_destroy( &(this->mutex) );
}

bool AtomicBool::get() {
    bool retVal = false;

    pthread_mutex_lock( &(this->mutex) );
    retVal = this->state;
    pthread_mutex_unlock( &(this->mutex) );

    return retVal;
}

void AtomicBool::set( bool state ) {

    pthread_mutex_lock( &(this->mutex) );
    this->state = state;
    pthread_mutex_unlock( &(this->mutex) );

}


