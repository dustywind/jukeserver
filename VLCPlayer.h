
#include <vlc/vlc.h>

#include "jukebox.h"
#include "Player.h"
#include "Semaphore.h"
#include "AtomicBool.h"

#ifndef VLCPLAYER_H
#define VLCPLAYER_H


class VLCPlayer : public Player {

    private:
        enum playerstatus_t { PLAY=0, STOP, PAUSE, RESUME };
        libvlc_instance_t *vlc_instance;

        struct thread_control {
            Semaphore       *s;
            pthread_t       play_next;
            //volatile bool   kill;
            AtomicBool      *kill;
        } thread_control;

        struct media_list_t {
            int     count;
            song_t  *list;
        } mlist;

        struct playmanager {
            bool        has_songs;
            int         current_song;   // index of mlist.list, which indicates the current sont
            playerstatus_t  status;

            libvlc_media_player_t *mp;  // media player
            char        *songpath;  // has to be stored for the play-duration, otherwise vlcplayer crashes :/
        } playmanager;


        // methods
        static void *play_next_routine_helper( void * );
        void *play_next_routine( void * );

        // callbacks, which are neccessary to work with libvlc properly
        // catch the stop-event, to play the next song
        static void next_when_media_ended_helper(const struct libvlc_event_t *, void *);
        void next_when_media_ended(const struct libvlc_event_t *, void *);

    public:
        VLCPlayer();
        ~VLCPlayer();

        /* helper methods */
        // create a media-player object which can be used by the other methods
        libvlc_media_player_t *get_current_media_player( void );
        void clear_current_media_player( void );

        /* inherited from Player.h */
        bool create_list( int, song_t * );  // create a list of songs, which shall be played in
        // this specific order
        bool add_to_list( int, song_t * );
        bool clear_list( void );

        // just "forward" these methods and let the vlc-lib handle everything
        bool toggle_play_pause( void ); // helper method
        bool play( void );
        bool pause( void );
        bool resume( void );
        bool stop( void );

        //
        bool previous( void );
        bool next( void );

        bool skip_to( int );        // skip to x-seconds from the beginning of the current song

        bool volume_up( void );
        bool volume_down( void );

        bool mute( void );
        bool unmute( void );
};

#endif /* VLCPLAYER */


