#include "memory_map.h"

class FileSystem {
public:
    Directory current_dir;
    File* current_file;
    bool file_open;

    FileSystem();

    void initialize();      // done

    void run(string);
    void run_script(string);

    void mkdir(string);     // Done
    Directory cd(string, Directory);        // Done
    void ls();              // Done
    void rm(string);        // Done
    void rm(string, bool);  // Done
    void mv(string, string);// TODO
    void pwd();             // done
    void view();
    void map(string);
    void man();

    void stat_(string);     // Done
    
    void mkfile(string);    // Done
    void open(string);      // done
    void close();           // done
    void read(int start, int size);
    void write(string, int);
    void append(string);
};