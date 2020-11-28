#include "File.h"


using namespace std;

File::File() {this->is_dir=false;}

File::File(string file_name, char file_start, Header first_header) {
    File();
    this->file_name = file_name;
    this->file_start = file_start;
    this->first_header = first_header;
}


void File::write(string file_contents) {
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
        headers[i].is_occupied = true; headers[i].is_dir = false;

        headers[i].prev = i > 0 ? (char)blocks[i - 1] : 0;
        headers[i].next = i < blocks_required - 1 ? (char)blocks[i + 1] : 0;

        write_block(headers[i], file_contents.substr(i*(BLOCK_SIZE-2), BLOCK_SIZE-2), blocks[i], i == (blocks_required - 1));
    }
}