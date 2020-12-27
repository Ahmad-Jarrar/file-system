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

    void write(string);
    void write(string, int);
    void write(string, bool is_new);
    
    string read();
    string read(int);
    string read(int, int);

    void truncate(int);

    void move_within_file(int, int, int);
    
};