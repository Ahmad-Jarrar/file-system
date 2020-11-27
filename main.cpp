#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "config.h"
#include "Dir.h"


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
	root.add_entry("newstest1 entry", 7, false, true);
	root.add_entry("newstest2 entry", 7, false, true);
	root.add_entry("newstest3 entry", 7, false, true);
	root.add_entry("newstest4 entry", 7, false, true);
	root.add_entry("newstest5 entry", 7, false, true);
	root.add_entry("newstest6 entry", 7, false, true);
	root.add_entry("newstest7 entry", 7, false, true);
	root.add_entry("newstest8 entry", 7, false, true);
	root.add_entry("newstest9 entry", 7, false, true);

	Entry entry;
	cout << endl << endl;
	entry.read(5,0);
	entry.print();



	return 0;
}