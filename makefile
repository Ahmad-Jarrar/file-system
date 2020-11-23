CC=g++
CFLAGS=-Wall -g -Werror -I.

DEPS= File.h Util.h
OBJ = File.o Util.o main.o

# LIBS=-lpthread

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

filesystem: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f *.o filesystem
