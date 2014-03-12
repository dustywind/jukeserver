/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE":
 * <vcharlie> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

#include <errno.h>

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#include "MPlayerPlayer.h"


MPlayerPlayer *MPlayerPlayer::_instance = NULL;

const char *mplayer = "mplayer";

/* create, destruct things */

MPlayerPlayer::MPlayerPlayer() {
    this->childpid = 0;
    this->hasSong = false;
    this->isPlaying = false;
    this->slist = NULL;
    this->slistLen = 0;

    // set the signal listener
    sigset_t myset;
    sigemptyset( &myset );
    sigaddset( &myset, SIGCHLD );

    struct sigaction sa;
    memset( &sa, 0, sizeof( struct sigaction ) );

    void (*f)(int, siginfo_t*, void*) = ( void (*) (int, siginfo_t*, void * ) ) &MPlayerPlayer::burychild;
    //void (*f)(int, siginfo_t*, void*) = NULL;

    sa.sa_sigaction = f;
    //sa.sa_sigaction =  &MPlayerPlayer::burychild;
    sa.sa_mask = myset;
    sa.sa_flags = SA_NOCLDWAIT | SA_RESTART;

    sigaction( SIGCHLD, &sa, NULL );
}

MPlayerPlayer::~MPlayerPlayer() {
    close( this->p_stdin[ P_READ ] );
    close( this->p_stdin[ P_WRITE ] );

    close( this->p_stdout[ P_READ ] );
    close( this->p_stdout[ P_WRITE ] );

    // delete slist
    if( this->slistLen > 0 ) {
        delete this->slist;
    }
}

MPlayerPlayer *MPlayerPlayer::getInstance( void ) {
    if( _instance == NULL) {
        _instance = new MPlayerPlayer();
    }
    return _instance;
}

void MPlayerPlayer::selfdestruct( void ) {
    if( _instance == NULL ) {
        return;
    }
    // TODO kill the child properly
    _instance->stop();
    delete _instance;
    _instance = NULL;
    return;
}


/* Signal handling */
void MPlayerPlayer::burychild(int signal, siginfo_t *info, void *context) {
    if( signal != SIGCHLD ) {
        return;
    }
    if( this->childpid == 0 ) {
        // kinda weird, there shouldn't be any child processes ...
        return;
    }
    // received a dead child
    // collect it
    this->collectZombie( this->childpid );	
    return;	
}

bool MPlayerPlayer::collectZombie(int pid) {
    return waitpid( pid, NULL, WNOHANG ) >= 0;
}



/* impl of other functions */

bool MPlayerPlayer::loadSong( char *path ) {
    // TODO, Check whether file exists or not
    // throw error, if not

    int pid = 0;

    // close the old(?)-filedescriptors, to void any trouble
    close( this->p_stdin[ P_READ ] );
    close( this->p_stdin[ P_WRITE ] );

    close( this->p_stdout[ P_READ ] );
    close( this->p_stdout[ P_WRITE ] );

    if( pipe( this->p_stdin ) || pipe( this->p_stdout ) ) {
        perror("Could not create pipes");
        // TODO throw error
        return EXIT_FAILURE;
    }

    //
    switch( pid = fork() ) {
        case 0:		// CHILD
            // close all unneccessary descriptors
            close( this->p_stdin[ P_WRITE ] );
            dup2( this->p_stdin[ P_READ ], STDIN_FILENO );

            close( this->p_stdout[ P_READ ]  );
            dup2( this->p_stdout[ P_WRITE ], STDOUT_FILENO );

            // and load the new program
            //execlp( mplayer, mplayer, "-ss", "60", path, NULL);
            execlp( mplayer, mplayer, path, NULL);
            // if this code is executed, something went wrong.
            perror( "could not exec");

            exit( EXIT_FAILURE );
            break;
        case -1:	// ERROR
            perror( "could not fork" );
            return false;
            break;
        default:	// FATHER
            this->childpid = pid;
            break;
    }

    this->hasSong = true;
    this->isPlaying = true;
    this->childpid = pid;

    return pid > 0;
}


/* public */
MPlayerPlayer *MPlayerPlayer::createList( int count, song_t *songlist ) {
    if( ! songlist || count <= 0 ) {
        // TODO throw ERROR	
        return NULL;
    }
    else if( _instance == NULL ) {
        // TODO throw ERROR
        return NULL;
    }
    // if there is already a list with songs, delete it
    if( !this->slist ) {
        this->clearList();
    }

    this->slistLen = count;
    this->slist = new song_t[count];
    if(  ! this->slist ) {
        // TODO throw exception
        // could not allocate memory
        return NULL;
    }
    memcpy( this->slist, songlist, sizeof( song_t ) * count );

    if( ! this->loadSong( this->slist[0].path ) ) {
        // TODO throw error
    }

    return _instance;	
}

MPlayerPlayer *MPlayerPlayer::addToList( int count, song_t *songlist )  {
    if( ! songlist ) {
        // TODO throw error
        return NULL;
    }

    if( this->slistLen == 0 ) {
        return this->createList( count, songlist );
    }

    song_t *tmp = new song_t[this->slistLen + count];
    if( tmp == NULL ) {
        // TODO throw exception
        return NULL;
    }

    memcpy( tmp, this->slist, sizeof( song_t ) * this->slistLen );
    memcpy( &tmp[this->slistLen], songlist, sizeof( song_t ) * count );

    this->slist = tmp;

    if( !this->loadSong( this->slist[0].path ) ) {
        // TODO throw error
    }

    return _instance;
}

MPlayerPlayer *MPlayerPlayer::clearList( void ) {
    delete this->slist;
    this->slistLen= 0;
    return _instance;
}

bool MPlayerPlayer::pause(void) {
    if( !this->hasSong ) {
        return false;
    }
    if( !this->isPlaying ) {
        return true;
    }

    write( p_stdin[ P_WRITE ] , " ", 1);
    this->isPlaying = false;

    return true;
}

bool MPlayerPlayer::resume(void) {
    if( !this->hasSong) {
        return false;
    }
    if( this->isPlaying ) {
        return true;
    }
    write( p_stdin[ P_WRITE ] , " ", 1);
    return true;
}

bool MPlayerPlayer::stop(void) {
    if( !this->hasSong ) {
        return true;
    }
    write( p_stdin[ P_WRITE ] , "q", 1);
    this->hasSong = false;
    return false;
}

bool MPlayerPlayer::skipTo(int seconds) {
    return false;
}

bool MPlayerPlayer::volumeUp(void) {
    return false;
}

bool MPlayerPlayer::volumeDown(void) {
    return false;
}

bool MPlayerPlayer::mute(void) {
    return false;
}

bool MPlayerPlayer::unmute(void) {
    return false;
}



