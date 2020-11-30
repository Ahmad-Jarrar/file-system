#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "config.h"
#include "FileSystem.h"

using namespace std;

int main(int argc, char const *argv[]) {
	FileSystem file_system;
	while(true) {
		string command;
		cout << ">> ";
		getline(cin, command);
		if (!command.compare("exit"))
			exit(-1);
		else {
			file_system.run(command);
		}
	}

	return 0;
}