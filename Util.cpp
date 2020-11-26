// #include <bitset>
#include "Util.h"

using namespace std;


Header::Header(char block_no, char prev, char next, bool is_occupied, bool is_dir) {
	this->is_dir = is_dir;
	this->block_no = block_no;
	this->prev = prev;
	this->next = next;
	this->is_occupied = is_occupied;
}

Header::Header(Header* header) {
	this->block_no = header->block_no;
	this->is_dir = header->is_dir;
	this->is_occupied = header->is_occupied;
	this->next = header->next;
	this->prev = header->prev;
}

Header::Header(int block_no) {
	this->block_no = block_no;
	read(block_no);
	// cout << (int)prev << endl << (int)next << endl << is_occupied << endl << is_dir << endl;
}

void Header::write(int block_no) {
	const char buffer[2] = { this->prev, (char) (this->next | (this->is_occupied ? IS_OCCUPIED : 0) | (this->is_dir ? IS_DIR : 0)) };
	fstream file(DATA_FILE, ios::binary | ios::out | ios::in);

	file.seekp(block_no << 8);
	file.write(buffer, 2);
	file.close();
}

void Header::read(int block_no) {
	fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
	file.seekg(block_no << 8);

	file >> prev;
	file >> next;

	is_occupied = (bool)(next & IS_OCCUPIED);
	is_dir = (bool)(next & IS_DIR);
	next = next & H_NEXT_MASK;

	file.close();
}

void Header::print() {
	cout << endl << "===Header info beg===\n"<< "Block no.: " << block_no << endl << "Prev: " << (int)prev << endl << "Next: " << (int)next << endl << "is_dir: " << is_dir << " is_occupied: " << is_occupied << "\n===Header info end===\n";
}

inline bool file_exists(const std::string& file_name) {
  struct stat buffer;   
  return (stat (file_name.c_str(), &buffer) == 0); 
}

void initialize() {
	if(!file_exists(DATA_FILE)) {
		ofstream my_file(DATA_FILE, ios::binary);
		char c = 0;
		for (int i = 0; i < ADDRESS_SPACE; i++)
			my_file.write(&c, 1);
		my_file.close();

		char prev = 0, next = 0;
		Header root_header(0, prev, next, true, true);
		root_header.write(0);
		
		// Header test(1, 1, true, false);
		// test.write(1);
		// cout << bitset<16>(build_header('?', '>', true, true)) << endl;
	}
}