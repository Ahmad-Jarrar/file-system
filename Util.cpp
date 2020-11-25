// #include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
// #include <bitset>
#include "Util.h"
#include "config.h"

using namespace std;


Header::Header(char prev, char next, bool is_occupied, bool is_dir) {
	this->is_dir = is_dir;
	this->prev = prev;
	this->next = next;
	this->is_occupied = is_occupied;
}

Header::Header(int block_no) {
	read(block_no);
	// cout << (int)prev << endl << (int)next << endl << is_occupied << endl << is_dir << endl;
}

void Header::write(int block_no) {
	const char buffer[2] = { this->prev, (char) (this->next | (this->is_occupied ? IS_OCCUPIED : 0) | (this->is_dir ? IS_DIR : 0)) };
	fstream file("data.dat", ios::binary | ios::out | ios::in);

	file.seekp(block_no * 256);
	file << buffer;
	file.close();
}

void Header::read(int block_no) {
	fstream file("data.dat", ios::binary | ios::out | ios::in);
	file.seekg(block_no * 256);

	file >> prev;
	file >> next;

	is_occupied = (bool)(next & IS_OCCUPIED);
	is_dir = (bool)(next & IS_DIR);
	next = next & H_NEXT_MASK;

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
		Header root_header(prev, next, true, false);
		root_header.write(0);
		
		Header test(0);
		// cout << bitset<16>(build_header('?', '>', true, true)) << endl;
	}
}