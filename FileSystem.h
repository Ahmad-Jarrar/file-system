#include "memory_map.h"

class FileSystem {
public:
    Directory current_dir;

    FileSystem();

    void run(string);
    void mkdir(string); // Done
    void cd(string);    // Done
    void ls();          // Done
    void rm(string);    // Done
    void rm(string, bool);  // Done
    void stat_(string);     // Done
    void mv(string, string);// TODO
    void mkfile(string);    // TODO
    void open(string);      // TODO
    void append(string);    // TODO
    void pwd();             // DONE
    void initialize();      // DONE
    void view();
    void map(string);


    // void allocate_block(Directory);
    void free_block(char);
};