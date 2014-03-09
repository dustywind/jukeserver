
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
				// TODO throw exception?
				return NULL;
			}

			this->playmanager.mp = libvlc_media_player_new_from_media( m );

			libvlc_media_release( m );	// release media
	}
	return this->playmanager.mp;
}

bool VLCPlayer::clear_current_media_player( void ) {
	libvlc_media_player_release( this->playmanager.mp );
	this->playmanager.mp = NULL;
	delete [] this->playmanager.songpath;
	return false;
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
	if( this->playmanager.has_songs == false ) {
		return false;
	}
	libvlc_media_t *m = NULL; // create media
	const char *songpath = NULL;

	switch( this->playmanager.status ) {
		case (int) VLCPlayer::PLAY:
		case (int) VLCPlayer::RESUME:
			return true;
		case (int) VLCPlayer::STOP:	// load the song again and play from the beginning

			songpath = this->mlist.list[this->playmanager.current_song].path.c_str();
			m = libvlc_media_new_path(	this->vlc_instance, songpath );

			this->playmanager.mp = libvlc_media_player_new_from_media( m );

			libvlc_media_release( m );	// release media
			//delete [] songpath;
			
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
