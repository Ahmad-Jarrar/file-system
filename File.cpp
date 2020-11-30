#include "File.h"


using namespace std;

File::File() {this->is_dir=false;}

File::File(string file_name) {
    File();
    this->file_name = file_name;
}

File::File(string file_name, char file_start, Header first_header) {
    File();
    this->file_name = file_name;
    this->file_start = file_start;
    this->first_header = first_header;
}

File::File(Entry entry) {
    File();
    this->file_name = entry.file_name;
    this->file_start = entry.file_start;
    this->first_header.read(this->file_start);
}

void File::create() {
    file_start = find_empty_block(0);
    first_header.read(file_start);
    write("");
}

void File::write(string file_contents) {
    write(file_contents, 0);
}

void File::write(string file_contents, int start) {
    // determine which block no. is to be written to
    // prepend relevant contents of that block to new contents
    // unoccupy that and all subsequent blocks
    // allocate and write to new blocks according to old strategy

    file_contents = escape(file_contents);

    int total_blocks = count_blocks(first_header);

    // might need to modify this condition to check for position within last block
    if (start > (total_blocks << 8) || start < 0) {
        cout << "Invalid start address" << endl;
        return;
    }
    
    // obtain header that must be written to
    int start_block_no = start / (BLOCK_SIZE - 2);
    Header header(&first_header);
    header = find_header_no(header, start_block_no);

    // prepend contents of block to file_contents
    string block_contents = read_block_contents(header.block_no, 0);

    if(block_contents.length() > 0)
        file_contents = block_contents.substr(0, start - (start_block_no<<8)) + file_contents;

    // unoccupy subsequent blocks
    clear_subsequent_blocks(header);

    // employ old strategy
    int blocks_required = (file_contents.length() + 1) / (BLOCK_SIZE - 2) + 1;
    int blocks[blocks_required];

    blocks[0] = header.block_no;
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

        headers[i].prev = i > 0 ? (char)blocks[i - 1] : header.prev;
        headers[i].next = i < blocks_required - 1 ? (char)blocks[i + 1] : 0;

        write_block(headers[i], file_contents.substr(i*(BLOCK_SIZE-2), BLOCK_SIZE-2), blocks[i], i == (blocks_required - 1));
    }
    first_header.read(first_header.block_no);
}

string File::read(int start, int size) {
    int start_block_no = start / (BLOCK_SIZE - 2);
    Header header(&first_header);
    header = find_header_no(header, start_block_no);
    string file_contents = "";

    while(file_contents.length() < size) {
        // header.print();
        string block_contents = read_block_contents(header.block_no, header.prev == 0 ? start - (start_block_no<<8) : 0);
        file_contents += block_contents;
        if(header.next == 0)
            break;
        header.read(header.next);
    }
    return file_contents.substr(0, size);
}

string File::read(int start) {
    return read(start, MAX_INT);
}

void File::truncate(int max_size) {
    write("", max_size);
}

void File::move_within_file(int start, int size, int target) {
    string file_contents = read(0);
    string move_string = file_contents.substr(start, size);

    if(target > file_contents.length()-1) {
        file_contents+=move_string;
    }
    else if(move_string.length() > file_contents.length()-1-target){
        file_contents = file_contents.substr(0, target) + move_string;
    }
    else {
        file_contents.replace(target, move_string.length(), move_string);
    }

    write(file_contents, 0);
}