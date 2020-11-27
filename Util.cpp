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
	cout << endl << "===Header info beg===\n"<< "Block no.: " << (int)block_no << endl << "Prev: " << (int)prev << endl << "Next: " << (int)next << endl << "is_dir: " << is_dir << " is_occupied: " << is_occupied << "\n===Header info end===\n";
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

    file_name = trim(file_name);
        
    
    


	file.close();
}

void Entry::stringify() {
    char last_char = this->file_start;
    last_char = last_char | (this->is_occupied ? IS_OCCUPIED : 0) | (this->is_dir ? IS_DIR : 0);
    sprintf(buffer, "%30s%c", this->file_name.c_str(), last_char);
    // return buffer;
}

void Entry::print() {
	cout << endl << "===Entry info beg===\n"<< "Block no.: " << (int)block_no << " Entry no.: " << (int)entry_no << endl << "File name: " << file_name << endl << "is_dir: " << is_dir << " is_occupied: " << is_occupied << "\n===Entry info end===\n";
}

/*
	=================
	=================
	Utility Functions
	=================
	=================
*/



void write_file_entry(string file_name, char file_start, bool is_dir) {
    // file_name = file_name.substr(0, 30); TODO: VALIDATE LENGTH OF FILE NAME UPON ENTRY
    file_start = file_start | (is_dir ? IS_DIR : 0);
    int padding = 30 - file_name.length();
    for (int i = 0; i < padding; i++) {
        file_name += (char)0;
    }
}

string trim(string& str)
{
    const auto strBegin = str.find_first_not_of(" ");
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(" ");
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void write_block(Header header, string file_contents, char block_no, bool is_last) {
    header.write(block_no);

    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    file.seekp((((int)block_no) << 8) + 2);

    // file.write(file_contents.c_str(), file_contents.length());
    // file.write(0, 1);

    file << file_contents;
    if(is_last) file << '\0';
    file.close();
}

int find_empty_block(int start_block = 0) {
    Header header;
    do {
        ++start_block;
        if (start_block > ADDRESS_SPACE / BLOCK_SIZE - 1)
            throw (start_block);
        header.read(start_block);
    } while(header.is_occupied);
    return start_block;
}

Header find_last_header(Header first_header) {
    Header header(&first_header);
    while(header.next != 0)
        header.read(header.next);
    return header;
}

Entry find_empty_entry_helper(int block_no) {
    Entry entry;
    int entry_no = 0;
    do {
        if (entry_no > 7)
            throw (entry_no);
        entry.read(entry_no, block_no);
        ++entry_no;
    } while(entry.is_occupied);
    entry.read(entry_no-1);
    return entry;
}

Entry search_entry_helper(int block_no, string file_name, bool dir_only, bool file_only) {
	
    Entry entry;
    int entry_no = 0;
    do {
        if (entry_no > 7)
            throw (entry_no);
        entry.read(entry_no, block_no);
		// cout << "Entry name:" << entry.file_name << endl << "file name:" << file_name << endl;
        ++entry_no;
    } while(entry.file_name.compare(file_name) || (dir_only ? !entry.is_dir : false) || (file_only ? entry.is_dir : false));
    entry.read(entry_no-1);
    return entry;
}

void allocate_extra_block(Header first_header) {
	int new_block_no = find_empty_block(0);
	Header last_header = find_last_header(first_header);
	
	last_header.next = new_block_no;
	last_header.write(last_header.block_no);

	Header new_last_header = Header(new_block_no, last_header.block_no, 0, last_header.is_occupied, last_header.is_dir);
	new_last_header.write(new_last_header.block_no);

}