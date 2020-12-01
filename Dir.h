#include "Util.h"

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
    Directory(Directory*);
    // void create_file(string file_contents);

    Entry find_empty_entry();

    void add_entry(string, char, bool, bool);
    void add_entry(Entry);
    
    void list_contents();

    void list_structure();
    void list_structure(string);

    Entry entrify();    

    Entry find_entry(string);
    Entry find_entry(string, bool, bool);

    void remove_entry(string);
    void remove_entry(Entry);

    void clear();

    bool is_empty();

    void write();
    void print();
};