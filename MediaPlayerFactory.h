
#include "Player.h"
#include "VLCPlayer.h"

#ifndef MEDIAPLAYERFACTORY_H
#define MEDIAPLAYERFACTORY_H 


class MediaPlayerFactory {

	private:
		static Player *_instance;
	
	public:
		static Player *create_player( void );
		static void destroy_player( void );
};




#endif /* MEDIAPLAYERFACTORY_H */
