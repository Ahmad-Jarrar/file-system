#include <streambuf>
#include "Util.h"

class File {
public:
    char file_start;
    char file_name[30];
    bool is_dir;
    Header first_header;
};


void write_contents(Header first_header, string file_contents, char file_start);

void write_file(string file_contents, bool is_dir);