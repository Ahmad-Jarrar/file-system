#include "Dir.h"


Directory::Directory(char file_start, string file_name, bool is_dir, Header first_header, Entry parent_dir) {
    this->file_start = file_start;
    this->file_name = file_name;
    this->is_dir = is_dir;
    this->first_header = first_header;
    this->parent_dir = parent_dir;
    
}

Directory::Directory(Entry entry) {
    this->file_start = entry.file_start;
    this->file_name = entry.file_name;
    this->is_dir = entry.is_dir;
    this->first_header = *(new Header(file_start));
    Entry parent;
    parent.read(0, file_start);
    this->parent_dir = parent;
}

void Directory::write() {
    clean_block(first_header.block_no);
    first_header.write(first_header.block_no);
    add_entry(parent_dir);
}

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

void Directory::add_entry(string file_name, char file_start, bool is_dir, bool is_occupied) {
    add_entry(Entry(file_name, file_start, is_dir, is_occupied));
}

void Directory::add_entry(Entry input_entry) {
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

    entry.file_name = input_entry.file_name; 
    entry.file_start = input_entry.file_start; 
    entry.is_dir = input_entry.is_dir;
    entry.is_occupied = input_entry.is_occupied;

    entry.write();
}

void Directory::list_contents() {
    Header header(&first_header);
    
    while (true) {
        list_entry_helper(header.block_no, first_header.block_no == header.block_no);
        if (header.next == 0)  
            break;
        header = new Header(header.next);
    }
}

void list_structure_helper(int block_no, bool first_block, string prefix) {
    Entry entry;
    for(int entry_no = first_block ? 1 : 0; entry_no < 8; entry_no++) {
        entry.read(entry_no, block_no);
        if(entry.is_occupied) {
            cout << prefix << '-' << entry.file_name;
            if (entry.is_dir) {
                cout << "/\n";
                Directory dir(entry);
                dir.list_structure(prefix+"\t");
            }
            else
                cout << endl;
        }
    }
}

void Directory::list_structure(string prefix) {
    Header header(&first_header);
    
    while (true) {
        list_structure_helper(header.block_no, first_header.block_no == header.block_no, prefix);
        if (header.next == 0)  
            break;
        header = new Header(header.next);
    }
}

void Directory::list_structure() {
    list_structure("");
}


Entry Directory::entrify() {

    return Entry(file_name, file_start, true, true);
}

Entry Directory::find_entry(string name,bool dir_only, bool file_only) {

    Header header(&first_header);

    while (true) {
        if (header.next == 0) {
            return search_entry_helper(header.block_no, name, dir_only, file_only, header.block_no==first_header.block_no);
        }
        else {
            try {
                return search_entry_helper(header.block_no, name, dir_only, file_only, header.block_no==first_header.block_no);
            }
            catch (int i) {
                header = new Header(header.next);
            }
        }
    }

}

Entry Directory::find_entry(string name) {
    return find_entry(name, false, false);
}

void Directory::remove_entry(string file_name) {
    try
    {
        find_entry(file_name).clear();

    }
    catch(int err)
    {
        cout << "Exception: trying to remove non-existant entry" << endl;
        exit(1);
    }
    
}

void Directory::clear() {

    Header header(&first_header);

    while (true) {
        Entry entry;
        bool first_block = first_header.block_no == header.block_no;
        if (first_block) {
            entry.read(0, header.block_no);
            entry.clear();
            
        }

        for(int entry_no = first_block ? 1 : 0; entry_no < 8; entry_no++) {
            entry.read(entry_no, header.block_no);
            
            if (!entry.is_occupied)
                continue;

            if(entry.is_dir) {
                Directory dir(entry);
                dir.clear();
            }
            delete_file(entry);  
        }
        if (header.next == 0)
            break;
        header.read(header.next);
    }

}

bool Directory::is_empty() {
    Header header(&first_header);
    
    while (true) {
        if(!is_empty_helper(header.block_no, first_header.block_no == header.block_no))
            return false;
        if (header.next == 0)  
            break;
        header = new Header(header.next);
    }
    return true;
}


void Directory::print() {
    cout << endl << "===Dir info beg===\n"<< "Block no.: " << (int)file_start << " Parent: " << parent_dir.file_name << endl << "File name: " << file_name << endl << "is_dir: " << is_dir << "\n===Dir info end===\n";
}

