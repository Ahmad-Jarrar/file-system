#include <iostream>
#include <stdlib.h>
#include <cstring>
#include "Util.h"

using namespace std;

int main(int argc, char const *argv[])
{
	// a command is entered
	if (argc > 2) {
		if (strcmp(argv[1], "mkdir") == 0) {
			Util util;
			util.make_dir(argv[2]);
		}
	}
	// no command is entered
	else
		cout << "Usage: ./filesystem [command] [subcommand]" << endl;


	return 0;
}