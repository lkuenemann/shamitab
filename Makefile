HEADERS = source/shamitab.h source/charlist.h source/view.h
OBJECTS = source/shamitab.o source/charlist.o source/view.o
CFLAGS = -Wall -g -O0
LIBS = 
CC = gcc
VPATH = source

default: shamitab

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

shamitab: $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f shamitab

