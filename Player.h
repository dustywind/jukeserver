/* virtual, abstract class.
 * Each implemented player has also to implement these methods
*/

#include "jukebox.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {


	public:

		virtual bool create_list( int, song_t * ) = 0;
		virtual bool add_to_list( int, song_t * ) = 0;
		virtual bool clear_list( void ) = 0;

		virtual bool play( void ) = 0;
		virtual bool pause( void ) = 0;
		virtual bool resume( void ) = 0;

		virtual bool skip_to( int ) = 0;

		virtual bool volume_up( void ) = 0;
		virtual bool volume_down( void ) = 0;

		virtual bool mute( void ) = 0;
		virtual bool unmute( void ) = 0;

};

#endif /* PLAYER_H */
