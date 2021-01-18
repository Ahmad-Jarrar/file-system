#include "../headers/File.h"


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
    allocation_mtx.lock();
    file_start = find_empty_block(0);
    first_header = Header(file_start, 0, 0, true, false);
    first_header.write();
    allocation_mtx.unlock();
    open(false);
    write("");
    close();
}

void File::open(bool is_read) {
    mode_mtx.lock();
    cout << file_name << "is being opened" << endl;
    int mode = first_header.get_mode();
    if (mode < 0) {
        mode_mtx.unlock();
        throw("Cannot open. File already opened in write mode\n");
    }
    else if (!is_read && mode > 0) {
        mode_mtx.unlock();
        throw("Cannot open in write mode. File already in use.\n");
    }
    first_header.set_mode(is_read);
    mode_mtx.unlock();
    this->is_read = is_read;
}

void File::close() {
    mode_mtx.lock();
    first_header.clear_mode();
    mode_mtx.unlock();
}

void File::write(string file_contents) {
    write(file_contents, 0);
}

void File::write(string file_contents, int start) {
    // determine which block no. is to be written to
    // prepend relevant contents of that block to new contents
    // unoccupy that and all subsequent blocks
    // allocate and write to new blocks according to old strategy

    if (is_read)
        throw("File opened in read mode\n");

    file_contents = escape(file_contents);

    int total_blocks = count_blocks(first_header);

    // might need to modify this condition to check for position within last block
    if (start > (total_blocks << 8) || start < 0) {
        throw("Invalid start address\n");
    }
    
    // obtain header that must be written to
    int start_block_no = start / (BLOCK_SIZE - 2);
    Header header(&first_header);
    header = find_header_no(header, start_block_no);

    // prepend contents of block to file_contents
    string block_contents = read_block_contents(header.block_no, 0);

    if((int)block_contents.length() > 0)
        file_contents = block_contents.substr(0, start - (start_block_no<<8)) + file_contents;

    // unoccupy subsequent blocks
    clear_subsequent_blocks(header);

    // employ old strategy
    int blocks_required = (file_contents.length() + 1) / (BLOCK_SIZE - 2) + 1;
    int blocks[blocks_required];

    blocks[0] = header.block_no;
    try {
        for(int i = 1; i < blocks_required; i++) {
            blocks[i] = allocate_extra_block(first_header);
            // cout << blocks[i] << "th block has been allocated to " << file_name << endl;
        }
    }
    catch(int err) {
        clear_subsequent_blocks(Header(blocks[0]));
        throw("Not enough free space!\n");
    }
    

    Header headers[blocks_required];
    for(int i = 0; i < blocks_required; i++) {
        headers[i].is_occupied = true; headers[i].is_dir = false;

        headers[i].mode = i > 0 ? 0 : header.mode;
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

    while((int)file_contents.length() < size) {
        // header.print();
        string block_contents = read_block_contents(header.block_no, 
                            header.block_no == first_header.block_no ? start - (start_block_no<<8) : 0);
        file_contents += block_contents;
        if(header.next == 0)
            break;
        header.read(header.next);
    }
    return file_contents.substr(0, size);
}

string File::read() {
    return read(0, MAX_INT);
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

    if(target > (int)file_contents.length()-1) {
        file_contents+=move_string;
    }
    else if((int)move_string.length() > (int)file_contents.length()-1-target){
        file_contents = file_contents.substr(0, target) + move_string;
    }
    else {
        file_contents.replace(target, (int)move_string.length(), move_string);
    }
    write(file_contents, 0);
}