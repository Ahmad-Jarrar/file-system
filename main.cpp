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

	return 0;
}