#include "Dir.h"

Entry Directory::find_empty_entry() {
    int block_no = first_header.block_no;
    Entry entry;
    Header header(&first_header);
    
    do {
        try {
            return find_empty_entry_helper(header.block_no);
        }
        catch(int err) {
            header.read(header.next);
        }
    } while(header.next != 0);
    return *(new Entry("",0,false, true));
}


Directory::Directory(char file_start, string file_name, bool is_dir, Header first_header) {
    this->file_start = file_start;
    this->file_name = file_name;
    this->is_dir = is_dir;
    this->first_header = first_header;
}

void Directory::add_entry(string file_name, char file_start, bool is_dir, bool is_occupied) {
    Entry entry = find_empty_entry();

    if (entry.is_occupied) {
        int new_block_no = find_empty_block(0);
        Header last_header = find_last_header(first_header);
        
        last_header.next = new_block_no;
        last_header.write(last_header.block_no);

        Header new_last_header = Header(new_block_no, last_header.block_no, 0, last_header.is_occupied, last_header.is_dir);

        entry = find_empty_entry();
    }

    entry.file_name = file_name; entry.file_start = file_start; 
    entry.is_dir = is_dir; entry.is_occupied = is_occupied;

    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    // cout << entry.entry_no;
    file.seekp((((int)entry.block_no) << 8) + 2 + entry.entry_no*31); // seek to start of first unoccupied entry found
    
    entry.stringify();
    file.write(entry.buffer, 31);
    file.close();

}

