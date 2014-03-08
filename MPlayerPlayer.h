/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE":
 * <vcharlie> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

#include <signal.h>
#include <stdlib.h>

#include "jukebox.h"

#ifndef MPLAYERPLAYER_H
#define MPLAYERPLAYER_H

#define P_READ	0
#define P_WRITE	1

class MPlayerPlayer {

	private:
		static MPlayerPlayer* _instance;	// implicit null?

		// pid of child-process
		int childpid;
	
		// descriptors for communicating with the child-process
		int p_stdin[2];
		int p_stdout[2];

		// list of songs, which shall be played
		int slistLen;
		song_t *slist;

		bool hasSong;
		bool isPlaying;

	private:
		MPlayerPlayer();
		~MPlayerPlayer();
		bool loadSong( char * );

		bool collectZombie( int );
	
	public:
		static MPlayerPlayer *getInstance( void );
		static void selfdestruct( void );

		// signal handler
		void burychild(int, siginfo_t *, void * );

		MPlayerPlayer *createList( int, song_t * );
		MPlayerPlayer *addToList( int, song_t * );
		MPlayerPlayer *clearList( void );

		bool pause(void);
		bool resume(void);

		bool stop(void);

		bool skipTo(int seconds);

		bool volumeUp(void);
		bool volumeDown(void);

		bool mute(void);
		bool unmute(void);
};


#endif	/* MPLAYERPLAYER_H */
