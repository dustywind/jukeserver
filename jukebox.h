
#include <string>

#ifndef JUKEBOX_H
#define JUKEBOX_H

typedef struct song_t {
	int			id;		// id of the song (pk of the song)
	std::string	path;	// path to the file on the harddrive
} song_t;

#endif /* JUKEBOX_H */



