#include "FileSystem.h"



FileSystem::FileSystem(){


    initialize();
    Header root_header(0);
	current_dir = *(new Directory(0, "/", true, root_header, Entry("/", 0, true, true)));

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
        Directory root(0, "/", true, root_header, Entry("/", 0, true, true));
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

    if (!dir_name.compare(".."))
    {
        current_dir = current_dir.parent_dir;
        return;
    }
    
    try
    {
        entry = current_dir.find_entry(dir_name, true, false);
        Directory child_dir(entry);
        current_dir = child_dir;
    }
    catch(int err)
    {
        cout << dir_name << " does not exist" << endl;
    }
    
}

void FileSystem::run(string command) {

}