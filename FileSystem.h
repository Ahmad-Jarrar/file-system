#include "Dir.h"

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
    void create(string);    // TODO
    void open(string);      // TODO
    void append(string);    // TODO
    void pwd();             // DONE
    void initialize();      // DONE


    // void allocate_block(Directory);
    void free_block(char);
};