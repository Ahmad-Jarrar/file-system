#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include "config.h"


using namespace std;

class Header {
public:
	char prev, next, block_no;
	bool is_occupied, is_dir;
	Header() {}
	Header(char, char, char, bool, bool);
	Header(int);
	void print();
	void write(int);
	void read(int);
};


void initialize();

void read_data();