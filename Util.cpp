// #include <iostream>
#include "Util.h"

using namespace std;

Util::Util() {
	cout << "Util constructor" << endl;
}

Util::~Util() {
	cout << "Util destructor" << endl;
}

int Util::create(string filename) {
	cout << filename << endl;
	return 0;
}