// #include <bitset>
#include "Util.h"

using namespace std;


/*
	============
	============
	Header Class
	============
	============
*/

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

/*
	===========
	===========
	Entry Class
	===========
	===========
*/

Entry::Entry(string file_name, char file_start, bool is_dir, bool is_occupied) {
    this->file_start = file_start;
    this->file_name = file_name;
    this->is_dir = is_dir;
    this->is_occupied = is_occupied;
}

void Entry::read(int entry_no, int block_no) {
    this->block_no = block_no;
    read(entry_no);
}

void Entry::read(int entry_no) {
    this->entry_no = entry_no;
	fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
	file.seekg((block_no << 8) + 2 + entry_no*31);
    char buffer[30];

    // file.seekg(entry_no*31, ios_base::cur);
	file.read(buffer, 30);
	file.read(&file_start, 1);

    // cout << "startout====" << buffer << "*" << hex << (int)file_start << "===endout" << endl;

	is_occupied = (bool)(file_start & IS_OCCUPIED);
	is_dir = (bool)(file_start & IS_DIR);
	file_start = file_start & H_NEXT_MASK;
    file_name = *(new string(buffer));

	file.close();
}

void Entry::stringify() {
    char last_char = this->file_start;
    last_char = last_char | (this->is_occupied ? IS_OCCUPIED : 0) | (this->is_dir ? IS_DIR : 0);
    sprintf(buffer, "%30s%c", this->file_name.c_str(), last_char);
    // return buffer;
}

void Entry::print() {
	cout << endl << "===Entry info beg===\n"<< "Block no.: " << (int)block_no << endl << "File name: " << file_name << endl << "is_dir: " << is_dir << " is_occupied: " << is_occupied << "\n===Header info end===\n";
}

/*
	=================
	=================
	Utility Functions
	=================
	=================
*/

inline bool file_exists(const std::string& file_name) {
  struct stat buffer;   
  return (stat (file_name.c_str(), &buffer) == 0); 
}

void write_file_entry(string file_name, char file_start, bool is_dir) {
    // file_name = file_name.substr(0, 30); TODO: VALIDATE LENGTH OF FILE NAME UPON ENTRY
    file_start = file_start | (is_dir ? IS_DIR : 0);
    int padding = 30 - file_name.length();
    for (int i = 0; i < padding; i++) {
        file_name += (char)0;
    }
}

Entry find_empty_entry_helper(int block_no) {
    Entry entry;
    int entry_no = 0;
    do {
        if (entry_no > 8)
            throw (entry_no);
        entry.read(entry_no);
        ++entry_no;
    } while(entry.is_occupied);
    entry.read(entry_no-1);
    return entry;
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