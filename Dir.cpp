#include "Dir.h"


Entry Directory::find_empty_entry() {
    Header header(&first_header);
    Entry available_entry;

    while (true) {
        if (header.next == 0) {
            return find_empty_entry_helper(header.block_no);
        }
        else {
            try {
                return find_empty_entry_helper(header.block_no);
            }
            catch (int i) {
                header = new Header(header.next);
            }
        }
    }
}


Directory::Directory(char file_start, string file_name, bool is_dir, Header first_header) {
    this->file_start = file_start;
    this->file_name = file_name;
    this->is_dir = is_dir;
    this->first_header = first_header;
    first_header.write(first_header.block_no);
}

void Directory::add_entry(string file_name, char file_start, bool is_dir, bool is_occupied) {
    Entry entry;
    try {
        entry = find_empty_entry();
    }
    catch (int i) {
        allocate_extra_block(first_header);
        first_header.read(first_header.block_no);

        try {
            entry = find_empty_entry();
        }
        catch (int b) {
            cout << "Exception" << endl;
        }
    }
   

    entry.file_name = file_name; 
    entry.file_start = file_start; 
    entry.is_dir = is_dir;
    entry.is_occupied = is_occupied;

    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    file.seekp((((int)entry.block_no) << 8) + 2 + entry.entry_no*31); // seek to start of first unoccupied entry found
    
    entry.stringify();
    file.write(entry.buffer, 31);
    file.close();

}

