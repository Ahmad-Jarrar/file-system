#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <thread>
#include "../headers/config.h"
#include "../headers/FileSystem.h"

using namespace std;

int main(int argc, char const *argv[]) {
	if (argc == 1) {
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
	}
	else if (argc > 1) {
		FileSystem file_systems[argc - 1];
		thread threads[argc - 1];

		int i;
		for(i = 0; i < argc - 1; i++)
			threads[i] = thread(thread_wrapper, file_systems[i], argv[i + 1]);
		for(i = 0; i < argc - 1; i++)
			threads[i].join();
	}
	return 0;
}