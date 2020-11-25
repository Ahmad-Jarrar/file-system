#include <iostream>
#include "File.h"

using namespace std;


void write_block(Header header, string file_contents, char block_no) {
    header.write(block_no);

    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    file.seekp((((int)block_no) << 8) + 2);

    // file.write(file_contents.c_str(), file_contents.length());
    // file.write(0, 1);

    file << file_contents << '\0';
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
    int blocks_required = file_contents.length() / (BLOCK_SIZE - 2) + 1;
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

        write_block(headers[i], file_contents.substr(i*(BLOCK_SIZE-2), BLOCK_SIZE-2), blocks[i]);
    }
}