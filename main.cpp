#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "config.h"
#include "FileSystem.h"


using namespace std;

// char buffer[ADDRESS_SPACE];


int main(int argc, char const *argv[]) {

	FileSystem file_system;
	file_system.mkdir("dir1");
	file_system.mkdir("dir2");
	file_system.current_dir.add_entry(Entry("file1", 10, false, true));
	file_system.pwd();
	file_system.cd("dummy");
	file_system.pwd();
	file_system.cd("file1");
	file_system.pwd();
	file_system.cd("dir1");
	file_system.pwd();

	file_system.cd("..");
	file_system.pwd();

	return 0;
}