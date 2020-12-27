CC=g++
CFLAGS=-std=c++17 -Wall -g -Werror -I.

DEPS= File.h Util.h Dir.h memory_map.h FileSystem.h
OBJ = File.o Util.o Dir.o memory_map.o FileSystem.o

LIBS=-lpthread

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

filesystem: $(OBJ) main.o
	$(CC) -o $@ $^ $(LIBS)

server: $(OBJ) main_server.o
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f *.o filesystem *.dat out_*
