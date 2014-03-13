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
    std::string song_1 = "/media/data/test_1.mp3";
    std::string song_2 = "/media/data/test_2.mp3";

    Player *p = MediaPlayerFactory::create_player();

    song_t narf[2];
    narf[0].id = 1;
    narf[0].path = song_1.c_str();

    narf[1].id = 2;
    narf[1].path = song_2.c_str();

    p->create_list( 2, narf );

    p->play();

    sleep( 20 );

    //p->stop();


    MediaPlayerFactory::destroy_player();

    return EXIT_SUCCESS;
}

