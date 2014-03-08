
#gcc $(pkg-config --cflags libvlc) -c test.c -o test.o
#gcc test.o -o test $(pkg-config --libs libvlc)

CC = g++ -L. -I. -I/usr/include/vlc/ -g
RM = rm -f
PTHREAD = -pthread
VLCLIB = -lvlc

.PHONY: all

all: main

clean:
	$(RM) main *.so *.o *.h.gch

main: main.c MediaPlayerFactory.o VLCPlayer.o Semaphore.o
	$(CC) -o $@ main.c *.o $(VLCLIB)


MediaPlayerFactory.o: MediaPlayerFactory.cpp MediaPlayerFactory.h VLCPlayer.o 
	$(CC) -c -fPIC MediaPlayerFactory.cpp

VLCPlayer.o: VLCPlayer.cpp VLCPlayer.h Player.h
	$(CC) -o $@ -c -fPIC VLCPlayer.cpp $(VLCLIB)

Semaphore.o: Semaphore.cpp Semaphore.h
	$(CC) -c -fPIC Semaphore.cpp $(PTHREAD)

