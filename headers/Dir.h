#include "Util.h"

class Directory {
public:
    char file_start;
    string file_name;
    bool is_dir;
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
    
    string list_contents();

    string list_structure();
    string list_structure(string);

    Entry entrify();    

    Entry find_entry(string);
    Entry find_entry(string, bool, bool);

    void remove_entry(string);
    void remove_entry(Entry);

    void clear();

    bool is_empty();

    void write();
    string stringify();
};