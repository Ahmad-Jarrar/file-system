// #include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <stdio.h>
#include <sstream>
// #include <fcntl.h>
#include <unistd.h>
#include <bitset>
#include "Util.h"
#include "config.h"

using namespace std;


inline short int build_header(char prev, char next, bool is_occupied, bool is_dir) {
	short int header = (0 | prev) << 8;
	return header | next | (is_occupied ? IS_OCCUPIED : 0) | (is_dir ? IS_DIR : 0);
}

void write_header(char prev, char next, bool is_occupied, bool is_dir) {
	const char buffer[2] = { prev, (char) (next | (is_occupied ? IS_OCCUPIED : 0) | (is_dir ? IS_DIR : 0)) };
	// int fd = open("data.dat", O_APPEND);
	// buffer[0] = prev;
	// buffer[1] = ;
	
	// FILE* file = fopen("data.dat", "a+b");
	// fseek(file, 0, SEEK_SET);
	// fputs(buffer, file);
	// fclose(file);
	fstream file("data.dat", ios::binary | ios::out | ios::in);
	ostringstream oss(buffer);

	file.seekp(3);
	file << buffer;
	file.close();
}

inline bool file_exists(const std::string& file_name) {
  struct stat buffer;   
  return (stat (file_name.c_str(), &buffer) == 0); 
}

void initialize() {
	if(!file_exists(DATA_FILE)) {
		ofstream my_file("data.dat", ios::binary);
		char c = 0;
		for (int i = 0; i < ADDRESS_SPACE; i++)
			my_file.write(&c, 1);
		my_file.close();

		char prev = 16, next = 15;
		write_header(prev, next, true, false);
		
		// cout << bitset<16>(build_header('?', '>', true, true)) << endl;
	}
}