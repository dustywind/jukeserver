/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE":
 * <vcharlie> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <unistd.h>

#include <vlc/vlc.h>

#include "Player.h"
#include "MediaPlayerFactory.h"

int main(int argc, char **argv) {
	//std::string songpath = "/media/data/Musik/Nine Inch Nails/Broken/01 - Nine Inch Nails - Pinion.mp3";
	std::string songpath = "/media/data/test.mp3";

	Player *p = MediaPlayerFactory::create_player();

	song_t narf;
	narf.id = 1;
	narf.path = songpath.c_str();

	p->create_list( 1, &narf );

	p->play();

	sleep( 10 );

	p->stop();

	sleep( 1 );

	MediaPlayerFactory::destroy_player();

	return EXIT_SUCCESS;
}

