#include <iostream>
#include "File.h"

using namespace std;


void write_contents(Header first_header, string file_contents, char file_start) {
    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    file.seekp((((int)file_start) << 8) + 2);

    file << file_contents;
    file.close();
}

void write_file(string file_contents, bool is_dir) {
    Header first_header;
    int block_no = 0;
    do {
        ++block_no;
        first_header.read(block_no);
    } while(first_header.is_occupied);

    first_header.is_occupied = true; first_header.is_dir = is_dir;
    first_header.next = first_header.prev = 1;
    first_header.write(block_no);

    write_contents(first_header, file_contents, block_no);
}