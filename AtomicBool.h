
#include <pthread.h>


#ifndef ATOMICBOOL_H
#define ATOMICBOOL_H

class AtomicBool {

    private:
        bool state;
        pthread_mutex_t mutex;

    public:
        AtomicBool( bool );
        ~AtomicBool();

        bool get();
        void set( bool );
};


#endif  /* ATOMICBOOL_H */
