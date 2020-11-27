#include "Util.h"


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