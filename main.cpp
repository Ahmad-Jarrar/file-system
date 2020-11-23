#include <iostream>
#include <stdlib.h>
#include "Util.h"

using namespace std;

int main(int argc, char const *argv[])
{
	if (argc > 1) {
		cout << "You have entered the command: " << argv[1] << endl;
		Util util;
		util.create(argv[1]);
	}


	return 0;
}