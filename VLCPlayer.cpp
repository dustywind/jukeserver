
#include <stdlib.h>
#include <string.h>

#include <vlc/vlc.h>

#include "jukebox.h"

#include "Semaphore.h"

#include "VLCPlayer.h"

VLCPlayer::VLCPlayer() {
    this->vlc_instance = libvlc_new( 0, NULL );
    if( this->vlc_instance == NULL) {
        // TODO throw exception
    }
    // init all values
    this->playmanager.status = VLCPlayer::STOP;
    this->playmanager.mp = NULL;
    this->playmanager.has_songs = false;

    this->mlist.count = 0;
    this->mlist.list = NULL;

    // create a thread, which is responsible for playing the next song
    this->thread_control.s = new Semaphore( 1 );
    this->thread_control.play_next = (pthread_t*) malloc( sizeof(pthread_t) );
    
    pthread_create( this->thread_control.play_next,
                    NULL,
                    &VLCPlayer::play_next_routine_helper,
                    this );
}

VLCPlayer::~VLCPlayer() {
    this->clear_current_media_player();
    // set exit-handler
    // TODO
    libvlc_release( this->vlc_instance );
    // block, till fully exited
    this->clear_list();
}

// thread routine
void *VLCPlayer::play_next_routine_helper( void *context) {
    (( VLCPlayer *) context)->play_next_routine( NULL );
    pthread_exit( NULL );
}
void *VLCPlayer::play_next_routine(void *args) {
    // get my semaphore
    Semaphore *s =  this->thread_control.s;
    volatile bool *suicide = &( this->thread_control.kill );
    while( true ) {
        // block
        s->P();

        // may I have to kill myself?
        if ( *suicide ) {
            break;
        }
        // otherwise
        // ...

    }
    return NULL;
};

libvlc_media_player_t *VLCPlayer::get_current_media_player( void ) {
    if( this->playmanager.mp == NULL) {
        // if there is a current song, create a media-player
        // otherwise NULL will be returned

        libvlc_media_t *m = libvlc_media_new_path(  this->vlc_instance,
                                                    this->mlist.list[this->playmanager.current_song].path.c_str());
        if( m == NULL ) {
            // TODO throw exception?
            return NULL;
        }

        this->playmanager.mp = libvlc_media_player_new_from_media( m );
        if( this->playmanager.mp == NULL ) {
            // TODO throw exception?
        }

        // get the event-manager
        libvlc_event_manager_t *em = libvlc_media_player_event_manager( this->playmanager.mp );
        libvlc_event_attach(    em,
                                libvlc_MediaPlayerEndReached,
                                (void (*)(const struct libvlc_event_t *, void *)) &VLCPlayer::next_when_media_ended,
                                NULL );

        libvlc_media_release( m );    // release media
    }

    return this->playmanager.mp;
}

/**
 *  Attention. In some cases libvlc_media_player can only be release within the stop_callback!
 */
void VLCPlayer::clear_current_media_player( void ) {
    libvlc_media_player_release( this->playmanager.mp );
    this->playmanager.mp = NULL;
}

void VLCPlayer::next_when_media_ended(const struct libvlc_event_t *, void *) {
    fprintf(stdout, "song ended\n");
    // give a signal to next-thread, that the next song can be played
    return;
}


bool VLCPlayer::create_list( int count, song_t *songs ) {
    if( count > 1 || songs == NULL) {
        // TODO throw exception
    }
    // destroy, what is already there (just a safety measure)
    this->clear_list();

    this->mlist.count = count;
    this->mlist.list = ( song_t *) malloc( sizeof( song_t ) * count );
    if( this->mlist.list == NULL) {
        // TODO throw exception?
        return false;
    }

    memcpy( this->mlist.list, songs, sizeof( song_t ) * count );

    this->playmanager.has_songs = true;

    return true;
}

bool VLCPlayer::add_to_list( int count, song_t *songs ) {
    if( this->mlist.count == 0) {
        return this->create_list( count, songs );
    }

    song_t *s = (song_t *) malloc( sizeof( song_t ) * ( count + this->mlist.count ) );
    if( s == NULL ) {
        // TODO throw exception?
        return false;
    }

    memcpy( s, this->mlist.list, sizeof( song_t) * this->mlist.count );
    memcpy( s + this->mlist.count, songs, sizeof( song_t ) * count );

    free( this->mlist.list );
    this->mlist.list = s;
    this->mlist.count = count + this->mlist.count;

    return false;
}

bool VLCPlayer::clear_list( void ) {
    this->mlist.count = 0;
    if( this->mlist.list != NULL) {
        free( this->mlist.list );
    }
    this->playmanager.has_songs = false;
    return false;
}


bool VLCPlayer::play( void ) {

    // retrieve libvlc_media_player_t
    libvlc_media_player_t *mp = get_current_media_player();
    if( mp == NULL ) {
        return false;
    }

    if( libvlc_media_player_play( mp ) == -1) {
        return false;
    }

    return true;
}

bool VLCPlayer::toggle_play_pause( void ) {
    // retrieve liblvlc_media_player_t
    libvlc_media_player_t *mp = this->get_current_media_player();
    if( mp == NULL) {
        return false;
    }
    libvlc_media_player_pause( mp );
    return true;;
}

bool VLCPlayer::pause( void ) {
    return this->toggle_play_pause();
}

bool VLCPlayer::resume( void ) {
    return this->toggle_play_pause();
}

bool VLCPlayer::stop( void ) {
    libvlc_media_player_t *mp = this->get_current_media_player();
    if( mp == NULL) {
        return false;
    }
    libvlc_media_player_stop( mp );
    this->clear_current_media_player();
    
    fprintf( stdout, "return stop();\n" );

    return true;
}


bool VLCPlayer::previous( void ) {
    return false;
}

bool VLCPlayer::next( void ) {
    this->clear_current_media_player();
    return false;
}


bool VLCPlayer::skip_to( int second ) {
    return false;
}

bool VLCPlayer::volume_up( void ) {
    return false;
}

bool VLCPlayer::volume_down( void ) {
    return false;
}


bool VLCPlayer::mute( void ) {
    return false;
}

bool VLCPlayer::unmute( void ) {
    return false;
}
