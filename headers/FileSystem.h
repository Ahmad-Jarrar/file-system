#include "memory_map.h"

class FileSystem {
public:
    Directory current_dir;
    File* current_file;
    bool file_open;
    ostream* out_stream;

    FileSystem();

    void initialize();      // done

    string run(string);
    void run_script(ifstream&);

    string mkdir(string);     // Done
    Directory cd(string, Directory);        // Done
    string ls();              // Done
    string rm(string);        // Done
    string rm(string, bool);  // Done
    void mv(string, string);// TODO
    string pwd();             // done
    string view();
    string map(string);
    string man();
    
    string mkfile(string);    // Done
    void open(string);      // done
    void close();           // done
    string read(int start, int size);
    void write(string, int);
    void append(string);
};

void thread_wrapper(FileSystem, string);