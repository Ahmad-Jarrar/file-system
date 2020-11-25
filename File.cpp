#include <iostream>
#include "File.h"

using namespace std;

int find_empty_entry(int block_no) {
    Entry entry;
    int entry_no = 0;
    do {
        ++entry_no;
        if (entry_no > ADDRESS_SPACE / BLOCK_SIZE)
            throw (entry_no);
        entry.read(entry_no);
    } while(entry.is_occupied);
    return entry_no;
}

Entry::Entry(string file_name, char file_start, bool is_dir, bool is_occupied) {
    this->file_start = file_start;
    this->file_name = file_name;
    this->is_dir = is_dir;
    this->is_occupied = is_occupied;
}

void Entry::read(int entry_no) {
	fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
	file.seekg(entry_no << 8);
    char buffer[30];

	file.read(buffer, 30);
	file.read(&file_start, 1);

	is_occupied = (bool)(file_start & IS_OCCUPIED);
	is_dir = (bool)(file_start & IS_DIR);
	file_start = file_start & H_NEXT_MASK;
    file_name = *(new string(buffer));

	file.close();
}

void Entry::stringify() {
    char last_char = this->file_start;
    last_char = last_char | (this->is_occupied ? IS_OCCUPIED : 0) | (this->is_dir ? IS_DIR : 0);
    sprintf(buffer, "%29s%c", this->file_name.c_str(), last_char);
    // return buffer;
}


Directory::Directory(char file_start, string file_name, bool is_dir, Header first_header) {
    this->file_start = file_start;
    this->file_name = file_name;
    this->is_dir = is_dir;
    this->first_header = first_header;
}

void Directory::add_entry(string file_name, char file_start, bool is_dir, bool is_occupied) {
    Entry entry(file_name, file_start, is_dir, is_occupied);

    int entry_no = find_empty_entry((int)this->file_start);
    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    cout << entry_no;
    file.seekp((((int)file_start) << 8) + 2 + entry_no*31); // seek to start of first unoccupied entry found
    entry.stringify();
    cout<<entry.buffer;
    file.write("ABC", 31);
    file.close();
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
        if (start_block > ADDRESS_SPACE / BLOCK_SIZE)
            throw (start_block);
        header.read(start_block);
    } while(header.is_occupied);
    return start_block;
}

void write_file(string file_contents, bool is_dir) {
    int blocks_required = (file_contents.length() + 1) / (BLOCK_SIZE - 2) + 1;
    int blocks[blocks_required];

    blocks[0] = find_empty_block(0);
    for(int i = 1; i < blocks_required; i++) {
        try {
            blocks[i] = find_empty_block(blocks[i - 1]);
        }
        catch(int err) {
            cout << "Not enough free space!" << endl;
            return;
        }
    }

    Header headers[blocks_required];
    for(int i = 0; i < blocks_required; i++) {
        headers[i].is_occupied = true; headers[i].is_dir = is_dir;

        headers[i].prev = i > 0 ? (char)blocks[i - 1] : 0;
        headers[i].next = i < blocks_required - 1 ? (char)blocks[i + 1] : 0;

        write_block(headers[i], file_contents.substr(i*(BLOCK_SIZE-2), BLOCK_SIZE-2), blocks[i], i == (blocks_required - 1));
    }
}


void write_file_entry(string file_name, char file_start, bool is_dir) {
    // file_name = file_name.substr(0, 30); TODO: VALIDATE LENGTH OF FILE NAME UPON ENTRY
    file_start = file_start | (is_dir ? IS_DIR : 0);
    int padding = 30 - file_name.length();
    for (int i = 0; i < padding; i++) {
        file_name += (char)0;
    }
}