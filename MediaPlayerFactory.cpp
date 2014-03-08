
#include "MediaPlayerFactory.h"

#include "Player.h"
#include "VLCPlayer.h"


Player *MediaPlayerFactory::_instance = NULL;


Player *MediaPlayerFactory::create_player( void ) {
	if( MediaPlayerFactory::_instance == NULL) {
		// create Instance of something that inherits the abstract class Player

		// TODO, there should be more options than VLCPlayer only
		// but for now, VLC will be sufficient

		MediaPlayerFactory::_instance = new VLCPlayer();
	}
	// deliver instance
	return MediaPlayerFactory::_instance;
}



void MediaPlayerFactory::destroy_player( void ) {
	delete MediaPlayerFactory::_instance;
	MediaPlayerFactory::_instance = NULL;
	return;
}


