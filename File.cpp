#include "File.h"


using namespace std;

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