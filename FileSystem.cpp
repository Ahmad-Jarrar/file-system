#include "FileSystem.h"



FileSystem::FileSystem(){


    initialize();
    Header root_header(0);
	current_dir = *(new Directory(0, "root", true, root_header, Entry("root", 0, true, true)));


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

    current_dir.add_entry(dirname, new_block, true, true);

}



void FileSystem::run(string command) {

}