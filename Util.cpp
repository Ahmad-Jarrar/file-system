// #include <iostream>
#include "Util.h"

using namespace std;

Util::Util() {
	storage_path = "./storage/";
	// cout << "Util constructor" << endl;
}

Util::~Util() {
	// cout << "Util destructor" << endl;
}

int Util::make_dir(string dir_name) {
	// cout << dir_name << "===" << endl;

	// dir_name = storage_path + dir_name;

	const char* file_path = (storage_path + dir_name).c_str();

	// int ret_flag = ;

	if (!mkdir(file_path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH )) 
		cout << "Successfully created " << "./storage/" << dir_name << endl;
	else {
		switch(errno) {
			case EEXIST:
				cout << "Directory of that name already exists" << endl;
				break;
			default:
				cout << "Error" << endl;
				break;
		}
	}

	return 0;
}