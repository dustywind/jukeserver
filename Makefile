
#gcc $(pkg-config --cflags libvlc) -c test.c -o test.o
#gcc test.o -o test $(pkg-config --libs libvlc)

CC = g++ -L. -I. -I/usr/include/vlc/ -g
RM = rm -f
PTHREAD = -pthread
VLCLIB = -lvlc

.PHONY: all

all: test

clean:

	$(RM) test *.so *.o *.h.gch

test: test.c MediaPlayerFactory.o VLCPlayer.o Semaphore.o
	$(CC) -o $@ test.c *.o $(VLCLIB)

MediaPlayerFactory.o: MediaPlayerFactory.cpp MediaPlayerFactory.h VLCPlayer.o 
	$(CC) -c -fPIC MediaPlayerFactory.cpp

VLCPlayer.o: VLCPlayer.cpp VLCPlayer.h Player.h Semaphore.o AtomicBool.o
	$(CC) -o $@ -c -fPIC VLCPlayer.cpp $(VLCLIB) $(PTHREAD)

Semaphore.o: Semaphore.cpp Semaphore.h
	$(CC) -c -fPIC Semaphore.cpp $(PTHREAD)

AtomicBool.o: AtomicBool.cpp AtomicBool.h
	$(CC) -c -fPIC AtomicBool.cpp $(PTHREAD)
