#include "Dir.h"

class FileSystem {
public:
    Directory current_dir;

    FileSystem();

    void run(string);
    void mkdir(string); // Done
    void cd(string);    // Done
    void ls();          // TODO
    void rm(string);    // TODO
    void mv(string, string); // TODO
    void create(string);    // TODO
    void open(string);      // TODO
    void append(string);    // TODO
    void pwd();             // DONE
    void initialize();      // DONE


    // void allocate_block(Directory);
    void free_block(char);
};