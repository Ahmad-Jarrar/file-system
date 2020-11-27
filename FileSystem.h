#include "Dir.h"

class FileSystem {
public:
    Directory current_dir;

    FileSystem();

    void run(string);
    void mkdir(string);
    void cd(string);
    void ls();
    void rm(string);
    void mv(string, string);
    void create(string);
    void open(string);
    void append(string);

    // void allocate_block(Directory);
    void free_block(char);
};