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
    string mv(string, string);// Done
    string pwd();             // done
    string view();
    string map(string);
    string man();
    
    string mkfile(string);    // Done
    string open(string, string);      // done
    string close();           // done
    string read(int start, int size);
    string write(string, int);
    string append(string);
};

void thread_wrapper(FileSystem, string);