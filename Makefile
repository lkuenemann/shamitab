HEADERS = source/shamitab.h
OBJECTS = source/shamitab.o

VPATH = source

default: shamitab

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

program: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f shamitab

