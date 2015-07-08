CC=gcc
CFLAGS=-Wall -Wno-strict-aliasing -O2 -c -pthread $(DEBUG)
LFLAGS=-lSDL2 -lm -lImlib2
SOURCES=gui.c images.c entity.c
OBJECTS=$(SOURCES:.c=.o)

.PHONY: debug remake clean

run: $(OBJECTS)
	$(CC) $(OBJECTS) $(DEBUG) -o run $(LFLAGS)

gui.o: gui.c gui.h images.h entity.h
	$(CC) $(CFLAGS) gui.c

images.o: images.c gui.h
	$(CC) $(CFLAGS) images.c

entity.o: entity.c
	$(CC) $(CFLAGS) entity.c

clean:
	rm -f *.o run

debug:
	$(MAKE) DEBUG=" -g -O0"
	
remake:
	$(MAKE) clean
	$(MAKE)
