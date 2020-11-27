#include "File.h"

class Directory {
public:
    char file_start;
    string file_name;
    bool is_dir;
    int total_blocks;
    Header first_header;
    Entry parent_dir;

    Directory() {}
    Directory(char, string, bool, Header, Entry);
    Directory(Entry);
    // void create_file(string file_contents);

    Entry find_empty_entry();

    void add_entry(string, char, bool, bool);

    Entry entrify();

    void add_entry(Entry);

    Entry find_entry(string);
    Entry find_entry(string, bool, bool);

    void remove_entry(string);
    void remove_entry(Entry);

    void write();
    void print();
};