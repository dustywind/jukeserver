
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
	this->playmanager.has_songs = false;
}

VLCPlayer::~VLCPlayer() {
	libvlc_release( this->vlc_instance );

	this->clear_list();
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

	song_t *s = ( song_t * ) malloc( sizeof( song_t ) * (count + this->mlist.count) );
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
	if( this->playmanager.has_songs == false ) {
		return false;
	}
	libvlc_media_t *m = NULL; // create media
	switch( this->playmanager.status ) {
		case (int) VLCPlayer::PLAY:
		case (int) VLCPlayer::RESUME:
			return true;
		case (int) VLCPlayer::STOP:	// load the song again and play from the beginning
			m = libvlc_media_new_path(	this->vlc_instance, 
										this->mlist.list[this->playmanager.current_song] );

			this->playmanager.mp = libvlc_media_player_new_from_media( m );

			libvlc_media_release( m );	// release media
			
			// and finally play it
			libvlc_media_player_play( this->playmanager.mp );

			this->playmanager.status = VLCPlayer::PLAY;
			return true;
		case (int) VLCPlayer::PAUSE:	// continue playing
			

			break;
		default:
			return false;
	};
	return true;
}

bool VLCPlayer::pause( void ) {
	return false;
}

bool VLCPlayer::resume( void ) {
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
