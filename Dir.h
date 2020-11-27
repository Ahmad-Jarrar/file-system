#include "File.h"

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

    Entry find_empty_entry();

    void add_entry(string, char, bool, bool);
    /* TODO:
        - MOVE
        - MOVE-WITHIN-FILE
        - APPEND
        - DELETE
        - COPY
    */
};


void create_directory();    // make header in a new block; adds an entry to