#include "FileSystem.h"



FileSystem::FileSystem(){


    initialize();
    Header root_header(0);
	current_dir = *(new Directory(0, "root", true, root_header, Entry("root", 0, true, true)));

}

void FileSystem::initialize() {
    struct stat buffer;   
    
    if(!(stat (DATA_FILE, &buffer) == 0)) {
		ofstream my_file(DATA_FILE, ios::binary);
		char c = 0;
		for (int i = 0; i < ADDRESS_SPACE; i++)
			my_file.write(&c, 1);
		my_file.close();


        Header root_header(0);
        root_header.is_dir = root_header.is_occupied = true;
        root_header.write(0);
        root_header.read(root_header.block_no);
        Directory root(0, "root", true, root_header, Entry("root", 0, true, true));
		root.write();
	}
}


void FileSystem::mkdir(string dirname) {

    if (dirname.length() > 30) 
    {
        cout << "File names cannot exceed 30 Characters!" << endl;
        return;
    }

    char new_block = (char)find_empty_block(0);
    Header header(new_block, 0, 0, true, true);
    Directory dir(new_block, dirname, true, header, current_dir.entrify());
    dir.write();

    current_dir.add_entry(dirname, new_block, true, true);

}

void FileSystem::pwd() {
    cout << current_dir.file_name << endl;
}

void FileSystem::cd(string dir_name){
    Entry entry;

    if (!dir_name.compare("..")) {
        current_dir = current_dir.parent_dir;
        return;
    }
    
    try {
        entry = current_dir.find_entry(dir_name, true, false);
        Directory child_dir(entry);
        current_dir = child_dir;
    }
    catch(int err) {
        cout << dir_name << " does not exist" << endl;
    }
    
}

void FileSystem::ls() {
    current_dir.list_contents();
}

void FileSystem::rm(string file_name) {
    Entry entry = current_dir.find_entry(file_name);
    Directory directory;    // use to recursively remove non-empty directories
    Header last_header = find_last_header(Header(entry.file_start));

    while (true) {
        last_header.is_occupied = false;
        last_header.write();
        if(last_header.prev == 0)
            break;
        last_header.read(last_header.prev);
    }
    entry.is_occupied = false;
    entry.write();
}

void FileSystem::run(string command) {

}