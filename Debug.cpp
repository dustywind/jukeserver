
#include <stdio.h>
#include "Debug.h"

void Debug::print( char *msg ) {
    #ifdef DEBUG
    fprintf( stderr, "DEBUG:\t%s\n", msg );
    #endif /* DEBUG */
}





