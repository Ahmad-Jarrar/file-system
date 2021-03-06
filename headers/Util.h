#include <iostream>
#include <fstream>
#include <streambuf>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <mutex>
#include "config.h"


using namespace std;

extern mutex file_mtx;
extern mutex allocation_mtx;
extern mutex mode_mtx;

class Header {
public:
	char mode, next, block_no;
	bool is_occupied, is_dir;
	Header() {}
	Header(Header*);
	Header(char, char, char, bool, bool);
	Header(int);
	string stringify();
	void write(int);
	void write();
	void read(int);
	void set_mode(bool);
	void clear_mode();
	int get_mode();
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

string trim(string& str);

string escape(string str);

void write_file_entry(string file_name, char file_start, bool is_dir);

void write_block(Header header, string file_contents, char block_no, bool is_last);

string read_block_contents(char, char);

int find_empty_block(int);

Header find_last_header(Header);

Header find_header_no(Header, int);

Entry find_empty_entry_helper(int);

Entry search_entry_helper(int, string, bool, bool, bool);

bool is_empty_helper(int, bool);

string list_entry_helper(int, bool);

char allocate_extra_block(Header);

void clean_block(char block_no);

void clear_subsequent_blocks(Header);

int count_blocks(Header);

void delete_file(Entry);

string get_manual();

vector<string> split_string(string, char);