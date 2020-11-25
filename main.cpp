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

	string in;
	cout << "Enter file contents: " << endl;
	getline(cin, in);
	write_file(in, true);
	// write_file("BAC", false);

	return 0;
}