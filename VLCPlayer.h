
#include <vlc/vlc.h>

#include "jukebox.h"
#include "Player.h"

#ifndef VLCPLAYER_H
#define VLCPLAYER_H


/*
typedef struct media_list_t {
	int count;
	song_t *list;
} media_list_t;
*/


class VLCPlayer : public Player {

	private:
		enum playerstatus_t { PLAY=0, STOP, PAUSE, RESUME };
		libvlc_instance_t *vlc_instance;

		struct media_list_t {
			int count;
			song_t *list;
		} mlist;

		struct playmanager {
			bool			has_songs;
			int				current_song;	// index of mlist.list, which indicates the current sont
			playerstatus_t	status;

			libvlc_media_player_t *mp;	// media player
		} playmanager;


	public:
		VLCPlayer();
		~VLCPlayer();

		bool create_list( int, song_t * );
		bool add_to_list( int, song_t * );
		bool clear_list( void );

		bool play( void );
		bool pause( void );
		bool resume( void );

		bool skip_to( int );

		bool volume_up( void );
		bool volume_down( void );

		bool mute( void );
		bool unmute( void );

};

#endif /* VLCPLAYER */


