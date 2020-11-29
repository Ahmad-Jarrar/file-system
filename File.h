#include "Dir.h"


class File {
public:
    char file_start;
    string file_name;
    bool is_dir;
    // int total_blocks;
    Header first_header;


    File();
    File(string);
    File(string, char, Header);
    File(Entry);

    void create();

    void open(string);

    string read();
    string read(int);
    string read(int, int);

    void append(string);
    void append(string, int);

    void write(string);
    void write(string, int, int);
    void write(string, bool is_new);
    
    // void create_file(string file_contents);

    /* TODO:
        - MOVE
        - MOVE-WITHIN-FILE
        - APPEND
        - DELETE
        - COPY
    */
};