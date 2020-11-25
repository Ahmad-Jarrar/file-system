#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "config.h"
#include "File.h"


using namespace std;

// char buffer[ADDRESS_SPACE];


int main(int argc, char const *argv[]) {
	initialize();

	// string in;
	// cout << "Enter file contents: " << endl;
	// getline(cin, in);
	// write_file(in, true);
	// // write_file("BAC", false);
	Header root_header(0);
	Directory root(0, "root", true, root_header);
	root.add_entry("abdullahfile", 4, false, true);

	Entry entry;
	entry.read(1,0);
	entry.print();



	return 0;
}