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

	while(true) {
		string command;
		cout << ">> ";
		getline(cin, command);
		if (!command.compare("exit"))
			break;
		else {
			file_system.run(command);
		}
	}
	/*
	file_system.mkdir("dir1");
	file_system.cd("dir1");
	file_system.mkdir("dir2");
	file_system.mkdir("dir3");
	file_system.current_dir.add_entry(Entry("file1", 10, false, true));
	file_system.mkdir("dir4");
	file_system.mkdir("dir5");
	file_system.mkdir("dir6");
	file_system.current_dir.add_entry(Entry("file2", 10, false, true));
	file_system.mkdir("dir7");
	file_system.mkdir("dir8");
	file_system.mkdir("dir9");
	file_system.mkdir("dir10");
	file_system.mkdir("dir11");
	file_system.mkdir("dir12");
	file_system.mkdir("dir13");
	file_system.current_dir.add_entry(Entry("file3", 10, false, true));

	file_system.pwd();
	file_system.ls();

	file_system.rm("dir3");
	file_system.ls();

	file_system.mkdir("dir97");
	file_system.ls();

	file_system.rm("dir12");
	file_system.ls();

	file_system.cd("..");
	file_system.pwd();
	file_system.ls();
	*/
	return 0;
}