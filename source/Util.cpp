// #include <bitset>
#include "../headers/Util.h"

using namespace std;

mutex file_mtx;
mutex allocation_mtx;

/*=========================================================================================================================
                                            HEADER CLASS DEFINITIONS
=========================================================================================================================*/

Header::Header(char block_no, char prev, char next, bool is_occupied, bool is_dir) {
	this->is_dir = is_dir;
	this->block_no = block_no;
	this->prev = prev;
	this->next = next;
	this->is_occupied = is_occupied;
}

Header::Header(Header* header) {
	this->block_no = header->block_no;
	this->is_dir = header->is_dir;
	this->is_occupied = header->is_occupied;
	this->next = header->next;
	this->prev = header->prev;
}

Header::Header(int block_no) {
	this->block_no = block_no;
	read(block_no);
}

void Header::write(int block_no) {
	const char buffer[2] = { this->prev, (char) (this->next | (this->is_occupied ? IS_OCCUPIED : 0) | (this->is_dir ? IS_DIR : 0)) };
	
    file_mtx.lock();
    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
	file.seekp(block_no << 8);
	file.write(buffer, 2);
	file.close();
    file_mtx.unlock();
}

void Header::write() {
    write(block_no);
}

void Header::read(int block_no) {
    file_mtx.lock();
	fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
	file.seekg(block_no << 8);
	file >> prev;
	file >> next;
    file.close();
    file_mtx.unlock();

	is_occupied = (bool)(next & IS_OCCUPIED);
	is_dir = (bool)(next & IS_DIR);
	next = next & H_NEXT_MASK;
    this->block_no = block_no;
}

string Header::stringify() {

    char block_no_str[3];
    char block_address_str[5];
    char next_str[3];
    char prev_str[3];

    sprintf(block_no_str, "%2d", (int)block_no);
    sprintf(block_address_str, "%4x", (((int)block_no) << 8));
    sprintf(next_str, "%2d", (int)next);
    sprintf(prev_str, "%2d", (int)prev);
    string bl = block_no_str;

	string header_string = "\n==========Header  info==========\n|Block no.: " + bl + "                 |\n" 
                 + "|Block Address: " + block_address_str + "           |\n" + 
                 + "|Prev Block: " + prev_str + " Next Block: " + next_str + " |\n" +
                 + "|is_dir: " + to_string(is_dir) + " is_occupied: " + to_string(is_occupied) + "      |"
                 + "\n================================\n";
    return header_string;
}

/*=========================================================================================================================
                                            ENTRY CLASS DEFINITIONS
=========================================================================================================================*/

Entry::Entry(string file_name, char file_start, bool is_dir, bool is_occupied) {
    this->file_start = file_start;
    this->file_name = file_name;
    this->is_dir = is_dir;
    this->is_occupied = is_occupied;
}

void Entry::read(int entry_no, int block_no) {
    this->block_no = block_no;
    read(entry_no);
}

void Entry::read(int entry_no) {
    this->entry_no = entry_no;

    file_mtx.lock();
	fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
	file.seekg((block_no << 8) + 2 + entry_no*31);
    char buffer[30];

	file.read(buffer, 30);
	file.read(&file_start, 1);
    file.close();
    file_mtx.unlock();

	is_occupied = (bool)(file_start & IS_OCCUPIED);
	is_dir = (bool)(file_start & IS_DIR);
	file_start = file_start & H_NEXT_MASK;
    file_name = *(new string(buffer));
    file_name = trim(file_name);
}

void Entry::write() {
    file_mtx.lock();
    stringify();

    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    file.seekp((((int)block_no) << 8) + 2 + entry_no*31);
    file.write(buffer, 31);
    file.close();
    file_mtx.unlock();
}

void Entry::clear() {
    this->is_occupied = false;
    write();
}

void Entry::stringify() {
    char last_char = this->file_start;
    last_char = last_char | (this->is_occupied ? IS_OCCUPIED : 0) | (this->is_dir ? IS_DIR : 0);
    sprintf(buffer, "%30s%c", this->file_name.c_str(), last_char);
}

void Entry::print() {
	cout << endl << "===Entry info===\n"<< "Block no.: " << (int)block_no << " Entry no.: "
        << (int)entry_no << endl << "File name: " << file_name << endl << "is_dir: " << is_dir
        << " is_occupied: " << is_occupied << "\n======\n";
}


/*=========================================================================================================================
                                        MISELLANEOUS LOW LEVEL FUNCTIONS
=========================================================================================================================*/


void write_file_entry(string file_name, char file_start, bool is_dir) {
    // file_name = file_name.substr(0, 30); TODO: VALIDATE LENGTH OF FILE NAME UPON ENTRY
    file_start = file_start | (is_dir ? IS_DIR : 0);
    int padding = 30 - (int)file_name.length();
    for (int i = 0; i < padding; i++) {
        file_name += (char)0;
    }
}

string trim(string& str) {
    const auto strBegin = str.find_first_not_of(" ");
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(" ");
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

string escape(string str) {
    string to = "";
    size_t start_pos = 0;
    while((start_pos = str.find('\\', start_pos)) != std::string::npos) {
        switch (str[start_pos+1])
        {
            case 'n':
                to = "\n";
                break;
            case '\\':
                to = "\\";
                break;
            case '\'':
                to = "\'";
                break;
            case 't':
                to = "\t";
                break;
            default:
                break;
        }
        str.replace(start_pos, 2, to);
        start_pos += (int)to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    return str;
}

void write_block(Header header, string file_contents, char block_no, bool is_last) {
    header.write(block_no);

    file_mtx.lock();
    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    file.seekp((((int)block_no) << 8) + 2);
    file << file_contents;
    if(is_last) file << '\0';
    file.close();
    file_mtx.unlock();
}

string read_block_contents(char block_no, char start) {
    char buffer[BLOCK_SIZE - 2];

    file_mtx.lock();
    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    file.seekg((((int)block_no) << 8) + 2 + start);
    file.read(buffer, BLOCK_SIZE - 2);
    file_mtx.unlock();
    return string(buffer);
}

int find_empty_block(int start_block = 0) {
    Header header;
    do {
        ++start_block;
        if (start_block > ADDRESS_SPACE / BLOCK_SIZE - 1)
            throw (start_block);
        header.read(start_block);
    } while(header.is_occupied);
    return start_block;
}

Header find_last_header(Header first_header) {
    Header header(&first_header);
    while(header.next != 0)
        header.read(header.next);
    return header;
}

Header find_header_no(Header header, int header_no) {
    int count = 0;
	while(true) {
        if(header_no == count)
            break;
        if (header.next == 0)
            throw(-1);      // required header not found
        count++;
        header.read(header.next);
    }
    return header;
}

Entry find_empty_entry_helper(int block_no) {
    Entry entry;
    int entry_no = 0;
    do {
        if (entry_no > 7)
            throw (entry_no);
        entry.read(entry_no, block_no);
        ++entry_no;
    } while(entry.is_occupied);
    entry.read(entry_no-1);
    return entry;
}

Entry search_entry_helper(int block_no, string file_name, bool dir_only, bool file_only, bool first_block) {
    Entry entry;
    int entry_no = first_block ? 1 : 0;
    while(true) {
        if (entry_no > 7)
            throw (entry_no);
        entry.read(entry_no, block_no);
        ++entry_no;
        if(!(entry.file_name.compare(file_name) || (dir_only ? !entry.is_dir : false) || (file_only ? entry.is_dir : false))
            && entry.is_occupied)
            break;
    } 
    entry.read(entry_no-1);
    return entry;
}

bool is_empty_helper(int block_no, bool first_block) {
    Entry entry;
    int entry_no = first_block ? 1 : 0;
    do {
        if (entry_no > 7)
            return true;
        entry.read(entry_no, block_no);
        ++entry_no;
    } while(!entry.is_occupied);
    return false;
}

string list_entry_helper(int block_no, bool first_block) {
    Entry entry;
    string list_string = "";
    for(int entry_no = first_block ? 1 : 0; entry_no < 8; entry_no++) {
        entry.read(entry_no, block_no);
        if(entry.is_occupied) {
            list_string += entry.file_name;
            if (entry.is_dir)
                list_string += "/";
            list_string += "\t";
        }
    }
    list_string += "\n";
    return list_string;
}

char allocate_extra_block(Header first_header) {
    allocation_mtx.lock();
	int new_block_no = find_empty_block(0);
	Header last_header = find_last_header(first_header);
	
    clean_block(new_block_no);
	last_header.next = new_block_no;
	last_header.write(last_header.block_no);
    last_header.read(last_header.block_no);

	Header new_last_header = Header(new_block_no, last_header.block_no, 0, last_header.is_occupied, last_header.is_dir);
	new_last_header.write(new_last_header.block_no);

    allocation_mtx.unlock();
    return new_last_header.block_no;
}

void clean_block(char block_no) {
    file_mtx.lock();
    fstream file(DATA_FILE, ios::binary | ios::out | ios::in);
    file.seekp((((int)block_no) << 8));
    for(int i = 0; i < BLOCK_SIZE; i++)
        file << '\0';
    file.close();
    file_mtx.unlock();
}

void clear_subsequent_blocks(Header header) {
    while(true) {
        if(header.next == 0)
            break;
        header.read(header.next);
        header.is_occupied = false;
        header.write();
    }
}

int count_blocks(Header header) {
	int count = 0;
	while(true) {
        count++;
        if(header.next == 0)
            break;
        header.read(header.next);
    }
    return count;
}

void delete_file(Entry entry) {
    Header first_header = Header(entry.file_start);

    while (true) {
        first_header.is_occupied = false;
        first_header.write();
        if(first_header.next == 0)
            break;
        first_header.read(first_header.next);
    }
    entry.clear();
}

vector<string> split_string(string s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


string get_manual() {
    string s = "__________________________________________________________________________________________________________________________\n"
        + string("                                                  Manual for File System\n")
        + "--------------------------------------------------------------------------------------------------------------------------\n"
        + "Command\t\tOpt Flags" + "\tUsage\t\t\tDescription\n\n"
        + "ls\t\t-a\t\tls [flags]\t\tLists The files and folders in current directory.\n\t\t\t\t\t\t\tUse \'-a\' to view all sub dirs.\n"
        + "mkdir" + "\t\t" + "\t\tmkdir [name]" + "\t\tCreates a folder in the current directory.\n"
        + "mkfile" + "\t\t" + "\t\tmkfile [name]" + "\t\tCreates a file in the current directory.\n"
        + "view" + "\t\t" + "\t\tview" + "\t\t\tView Stats of the File System.\n"
        + "cd" + "\t\t" + "\t\tcd [destination]" + "\tChange current directory.\n"
        + "rm" + "\t\t-r" + "\t\trm [flag] [name]" + "\tRemove File/folder from current directory.\n\t\t\t\t\t\t\tUse \'-r\' to delete recursively.\n"
        + "mv" + "\t\t" + "\t\tmv [source] [dest]" + "\tMove File/Folder from source to destination.\n"
        + "pwd" + "\t\t" + "\t\tpwd" + "\t\t\tPrint present working directory.\n"
        + "map" + "\t\t" + "\t\tmap [file_name]" + "\t\tShow which blocks in memory a file/folder occupies.\n"
        + "open" + "\t\t" + "\t\topen [file_name]" + "\tOpen file to read/write.\n"
        + "close" + "\t\t" + "\t\tclose" + "\t\t\tClose currently open file.\n"
        + "read" + "\t\t[start] [size]" + "\tread [flags]" + "\t\tRead entire opened file.\n\t\t\t\t\t\t\tUse options to read from an offset or limit file size.\n"
        + "write" + "\t\t[start] [size]" + "\twrite [start] [size]" + "\tWrite to opened file. It overwrites existing file.\n\t\t-s [content]\t   [options] [content]\tUse options to modify existing file content.\n"
        + "append" + "\t\t-s [contents]" + "\tappend [flags] [content]" + "  Write to opened file. It writes to the end of existing file.\n"
        + "trunc" + "\t\t" + "\t\ttrunc [size]" + "\t\tTruncate opened file to size.\n"
        + "man" + "\t\t" + "\t\tman" + "\t\t\tOpen Manual\n"

        + "--------------------------------------------------------------------------------------------------------------------------\n";
    return s;
}