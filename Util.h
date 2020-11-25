#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

using namespace std;

class Header {
public:
	char prev, next;
	bool is_occupied, is_dir;

	Header(char, char, bool, bool);
	Header(int);
	void write(int);
	void read(int);
};


void initialize();

void read_data();