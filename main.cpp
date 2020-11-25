#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "config.h"
#include "File.h"


using namespace std;

char buffer[ADDRESS_SPACE];


int main(int argc, char const *argv[]) {
	initialize();

	write_file("AB", true);
	write_file("BA", false);

	return 0;
}