
#include <stdlib.h>
#include <string.h>

#include <vlc/vlc.h>

#include "jukebox.h"

#include "VLCPlayer.h"

VLCPlayer::VLCPlayer() {
	this->vlc_instance = libvlc_new( 0, NULL );
	if( this->vlc_instance == NULL) {
		// TODO throw exception
	}
	this->playmanager.status = VLCPlayer::STOP;
	this->playmanager.mp = NULL;
	this->playmanager.has_songs = false;
}

VLCPlayer::~VLCPlayer() {
	this->clear_current_media_player();
	libvlc_release( this->vlc_instance );
	this->clear_list();
}
libvlc_media_player_t *VLCPlayer::get_current_media_player( void ) {
	if( this->playmanager.mp == NULL) {
		// if there is a current song, create a media-player
		// otherwise NULL will be returned
		
			this->playmanager.songpath = (char *) this->mlist.list[this->playmanager.current_song]
												.path.c_str();

			libvlc_media_t *m = libvlc_media_new_path(	this->vlc_instance,
														this->playmanager.songpath );
			if( m == NULL ) {
				delete [] this->playmanager.songpath;
				// TODO throw exception?
				return NULL;
			}

			this->playmanager.mp = libvlc_media_player_new_from_media( m );
			if( this->playmanager.mp == NULL ) {
				delete [] this->playmanager.songpath;
				// TODO throw exception?
				// libvlc_media_release( m );
				// return NULL;
			}

			libvlc_media_release( m );	// release media
	}
	return this->playmanager.mp;
}

/**
 *	Attention. In some cases libvlc_media_player can only be release within the stop_callback!
 */
void VLCPlayer::clear_current_media_player( void ) {
	libvlc_media_player_release( this->playmanager.mp );
	this->playmanager.mp = NULL;
	delete [] this->playmanager.songpath;
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
	return true;
}


bool VLCPlayer::previous( void ) {
	return false;
}

bool VLCPlayer::next( void ) {
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
