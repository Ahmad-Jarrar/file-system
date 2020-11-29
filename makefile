CC=g++
CFLAGS=-std=c++17 -Wall -g -Werror -I.

DEPS= File.h Util.h Dir.h memory_map.h FileSystem.h
OBJ = File.o Util.o Dir.o memory_map.o FileSystem.o main.o 

# LIBS=-lpthread

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

filesystem: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f *.o filesystem *.dat
