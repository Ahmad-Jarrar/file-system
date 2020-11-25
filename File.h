#include <streambuf>
#include "Util.h"


class Entry {
public:
	char file_start;
    char buffer[31];
    string file_name;
	bool is_occupied, is_dir;
	Entry() {}
	Entry(string, char, bool, bool);
	// Entry(int);
	void stringify();
	void write(int);
	void read(int);
};



class Directory {
public:
    char file_start;
    string file_name;
    bool is_dir;
    int total_blocks;
    Header first_header;
    // parent directory

    Directory() {}
    Directory(char, string, bool, Header);
    // void create_file(string file_contents);

    void add_entry(string, char, bool, bool);
    /* TODO:
        - MOVE
        - MOVE-WITHIN-FILE
        - APPEND
        - DELETE
        - COPY
    */
};




class File {
public:
    char file_start;
    string file_name[30];
    bool is_dir;
    int total_blocks;
    Header first_header;

    
    // void create_file(string file_contents);

    /* TODO:
        - MOVE
        - MOVE-WITHIN-FILE
        - APPEND
        - DELETE
        - COPY
    */
};


void write_contents(Header first_header, string file_contents, char file_start);

void write_file(string file_contents, bool is_dir);

void write_file_entry(string file_name, char file_start, bool is_dir);

void create_directory();    // make header in a new block; adds an entry to