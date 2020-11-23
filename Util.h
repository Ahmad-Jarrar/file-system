#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

using namespace std;

class Util
{
private:
	string storage_path;

public:
	Util();
	~Util();


	int make_dir(string filename);
};