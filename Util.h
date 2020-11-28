#include <iostream>
#include <fstream>
#include <streambuf>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include "config.h"


using namespace std;

class Header {
public:
	char prev, next, block_no;
	bool is_occupied, is_dir;
	Header() {}
	Header(Header*);
	Header(char, char, char, bool, bool);
	Header(int);
	void print();
	void write(int);
	void write();
	void read(int);
};

class Entry {
public:
	char file_start, block_no, entry_no;
    char buffer[31];
    string file_name;
	bool is_occupied, is_dir;
	Entry() {}
	Entry(string, char, bool, bool);
	// Entry(int);
	void stringify();
	void write();

	void clear();
	void read(int);
    void read(int, int);
    void print();
};

void read_data();

string trim(string& str);

void write_file_entry(string file_name, char file_start, bool is_dir);

void write_block(Header header, string file_contents, char block_no, bool is_last);

int find_empty_block(int);

Header find_last_header(Header);

Entry find_empty_entry_helper(int);

Entry search_entry_helper(int, string, bool, bool);

bool is_empty_helper(int, bool);

void list_entry_helper(int, bool);

void allocate_extra_block(Header);

void delete_file(Entry);

void parse_path(string);

vector<string> split_string(string);