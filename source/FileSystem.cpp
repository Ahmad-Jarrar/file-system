#include "../headers/FileSystem.h"



FileSystem::FileSystem(){
    initialize();
    Header root_header(0);
	current_dir = *(new Directory(0, "/", true, root_header, Entry("/", 0, true, true)));
    file_open = false;
    out_stream = &cout;
}

void FileSystem::initialize() {
    struct stat buffer;   
    
    if(!(stat (DATA_FILE, &buffer) == 0)) {
		ofstream my_file(DATA_FILE, ios::binary);
		char c = 0;
		for (int i = 0; i < ADDRESS_SPACE; i++)
			my_file.write(&c, 1);
		my_file.close();


        Header root_header(0);
        root_header.is_dir = root_header.is_occupied = true;
        root_header.write(0);
        root_header.read(root_header.block_no);
        Directory root(0, "/", true, root_header, Entry("/", 0, true, true));
		root.write();
	}
}


string FileSystem::mkdir(string file_name) {

    if ((int)file_name.length() > 30) 
    {
        return "File names cannot exceed 30 Characters!\n";
    }

    try {
        current_dir.find_entry(file_name);
        return "Folder already exists\n";
    }
    catch(int err) {
        allocation_mtx.lock();
        char new_block = (char)find_empty_block(0);
        Header header(new_block, 0, 0, true, true);
        Directory dir(new_block, file_name, true, header, current_dir.entrify());
        dir.write();
        allocation_mtx.unlock();
        current_dir.add_entry(file_name, new_block, true, true);
    }
    return "";    
}

string FileSystem::open(string file_name, string mode) {
    if(file_open) {
        return "A file is already open. Close it to open new file\n";
    }
    bool is_read;
    if (!mode.compare("-r"))
        is_read = true;
    else if (!mode.compare("-w"))
        is_read = false;
    else
        return "Use -r to read, -w to write";

    try {
        Entry entry = current_dir.find_entry(file_name);
        if (!entry.is_dir) {
            current_file = new File(entry);
            current_file->open(is_read);
            file_open = true;
        }
        else
            return "File of name " + file_name + " not found\n";
    }
    catch (int err) {
        return file_name + " not found\n";
    }
    catch (const char* err) {
        return string(err);
    }
    return "";
}

string FileSystem::close() {
    if(file_open) {
        current_file->close();
        delete current_file;
        file_open = false;
    }
    return "";
}

string FileSystem::read(int start, int size) {
    if(file_open)
        return current_file->read(start, size) + "\n";
    else
        return "No file opened\n";
}

string FileSystem::write(string file_contents, int start) {
    if(file_open) {
        try {
           current_file->write(file_contents, start);
        }
        catch(const char* err) {
            return *err + "\n";
        }
    }
    else
        return "No file opened\n";
    return "";
}

string FileSystem::append(string file_contents) {
    if(file_open)
    {   
        try {
           current_file->write(file_contents, (int)current_file->read().length());
        }
        catch(const char* err) {
            return *err + "\n";
        }
    }
    else
        return "No file opened\n";
    return "";
}

string FileSystem::pwd() {
    return current_dir.file_name+"\n";
}

Directory FileSystem::cd(string dir_name, Directory dir){
    Entry entry;

    if (!dir_name.compare("..")) {
        return dir.parent_dir;
    }
    
    try {
        entry = dir.find_entry(dir_name, true, false);
        return Directory(entry);
    }
    catch(int err) {
        throw(dir_name + " does not exist\n");
    }
}

string FileSystem::ls() {
    return current_dir.list_contents();
}

string FileSystem::rm(string file_name) {
    return rm(file_name, false);
}

string FileSystem::rm(string file_name, bool recursive) {
    if (!file_name.compare("/"))
        return "ROOT DIRECTORY: DO NOT ATTEMPT TO DELETE OR DIRE THINGS WILL HAPPEN\n";

    try {
        Entry entry = current_dir.find_entry(file_name);
        if (entry.is_dir) {
            Directory directory(entry);

            if (!directory.is_empty() && !recursive)
                return "Directory not empty. Use -r flag to delete recursively\n";
            else
                directory.clear();
        }
        
        delete_file(entry);
        return "";
    }
    catch(string err) {
        return file_name + " not found\n";
    }
}

string FileSystem::mv(string source, string destination) {

    Directory source_dir;
    Directory dest_dir;

    vector<string> source_path = split_string(source, '/');
    vector<string> dest_path = split_string(destination, '/');

    if (source[0] == '/')
    {
        source_dir = Directory(Entry("/", 0, true, true));
        source_path[0] = "..";
    }
    else
    {
        source_dir = Directory(&current_dir);
    }
    

    if (destination[0] == '/')
    {
        dest_dir = Directory(Entry("/", 0, true, true));
        dest_path[0] = "..";
    }
    else
    {
        dest_dir = Directory(&current_dir);
    }
    
    

    for(int i = 0; i < (int)source_path.size()-1; i++) {
        try {
            source_dir = cd(source_path[i], source_dir);
        }
        catch(string err) {
            return err;
        }
    }

    for(int i = 0; i < (int)dest_path.size()-1; i++) {
        try{
            dest_dir = cd(dest_path[i], dest_dir);
        }
        catch(string err) {
            return err;
        }
    }

    Entry entry = source_dir.find_entry(source_path[source_path.size() - 1]);
    entry.clear();  entry.is_occupied = true;
    entry.file_name = dest_path[dest_path.size()-1];
    dest_dir.add_entry(entry);
    return "";
}

string FileSystem::mkfile(string file_name) {
    try {
        current_dir.find_entry(file_name);
        return "File already exists\n";
    }
    catch(int err) {
        File file(file_name);
        try {
            file.create();
        }
        catch(string err) {
            return err;
        }
        current_dir.add_entry(file_name, file.file_start, false, true);
        return "";
    }
}

string FileSystem::view() {
    string out_string = disk_usage();
    out_string += "\n\n";
    out_string += block_map();
    return out_string;
}

string FileSystem::man() {
    return get_manual();
}

string FileSystem::map(string file_name) {
    try {
        return show_memory_map(current_dir.find_entry(file_name));
    }
    catch (int err) {
        return "File not found\n";
    }
}

string FileSystem::run(string command) {
    vector<string> tokens = split_string(command, ' ');
    string out_string = "";
    
    if (tokens.size() == 0)
        out_string += "";

    if (!tokens[0].compare("ls") && tokens.size() < 2) {
        out_string += ls();
    }
    else if (!tokens[0].compare("ls")  && !tokens[1].compare("-a")) {
        out_string += current_dir.list_structure();
    }
    else if (!tokens[0].compare("mkdir")) {
        out_string += mkdir(tokens[1]);
    }
    else if (tokens.size() > 2 && !tokens[0].compare("rm") && !tokens[1].compare("-r")) {
        if (tokens[2].size() == 0)
            out_string += "Invalid Command! for help type 'man'\n";
        else
            out_string += rm(tokens[2], true);
    }
    else if (tokens.size() > 1 && !tokens[0].compare("rm") && tokens[1].compare("-r")) {
        if (tokens[1].size() == 0)
            out_string += "Invalid Command! for help type 'man'\n";
        else
            out_string += rm(tokens[1]);
    }
    else if (!tokens[0].compare("cd")) {
        if (tokens[1].size() == 0)
            out_string += "Invalid Command! for help type 'man'\n";
        
        try {
            Directory curr_dir = Directory(&current_dir);
            vector<string> path = split_string(tokens[1], '/');

            if (tokens[1][0] == '/') {
                while(curr_dir.parent_dir.file_name != "/")
                    curr_dir = curr_dir.parent_dir;
                path[0] = "..";
            }

            for(int i = 0; i < (int)path.size(); i++) {
                curr_dir = cd(path[i], curr_dir);
            }
            current_dir = curr_dir;
        }
        catch(string err) {
            out_string += err;
        }
    }
    else if (!tokens[0].compare("pwd")) {
        out_string += pwd();
    }
    else if (!tokens[0].compare("map")) {
        if (tokens.size() < 2 || tokens[1].size() == 0)
            out_string += "Invalid Command! for help type 'man'\n";
        else
            out_string += map(tokens[1]);
    }
    else if (!tokens[0].compare("view")) {
        out_string += view();
    }
    else if (!tokens[0].compare("man")) {
        out_string += man();
    }
    else if (!tokens[0].compare("mkfile")) {
        if (tokens.size() < 2 || tokens[1].size() == 0)
            out_string += "Invalid Command! for help type 'man'\n";
        else
            out_string += mkfile(tokens[1]);
    }
    else if (!tokens[0].compare("open") && tokens.size() > 2) {
        out_string += open(tokens[2], tokens[1]);
    }
    else if (!tokens[0].compare("close")) {
        out_string += close();
    }
    else if (!tokens[0].compare("read")) {
        int start, size;
        if (tokens.size() == 1) {
            start = 0;
            size = MAX_INT;
        }
        else if (tokens.size() == 2) {
            start = stoi(tokens[1]);
            size = MAX_INT;
        }
        else if (tokens.size() == 3) {
            start = stoi(tokens[1]);
            size = stoi(tokens[2]);
        }
        
        out_string += read(start, size);
    }
    else if (!tokens[0].compare("write")) {
        string file_contents;
        
        int start;
        
        if (tokens.size() >= 3) {
            if (!tokens[1].compare("-s")) {
                file_contents = command.substr(tokens[0].size()+tokens[1].size()+2);
            }
            else if (!tokens[2].compare("-s")) {
                start = stoi(tokens[1]);
                file_contents = command.substr(tokens[0].size()+tokens[1].size()+tokens[2].size()+3);
            }
            else {
                out_string += "Invalid Arguments\n";
            }
        }

        out_string += write(file_contents, start);
    }
    else if (!tokens[0].compare("append")) {
        string file_contents;

        if (tokens.size() >= 2) {
            if (!tokens[1].compare("-s")) {
                file_contents = command.substr(tokens[0].size()+tokens[1].size()+2);
            }
            else {
                out_string += "Invalid Arguments\n";
            }
        }

        out_string += append(file_contents);
    }
    else if (!tokens[0].compare("trunc")) {
        if (!file_open) {
            out_string += "No file open\n";
        }
        current_file->truncate(stoi(tokens[1]));
    }
    else if (!tokens[0].compare("mvwf")) {
        if (!file_open) {
            out_string += "No file open\n";
        }
        try
        {
            current_file->move_within_file(stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]));
        }
        catch(string err)
        {
          out_string += err;   
        }
    }
    else if (!tokens[0].compare("mv")) {
        if (tokens.size() < 2 || !tokens[1].size() || tokens[2].size())
        {
            out_string += "Invalid Command! for help type 'man'\n";
        }
        
        out_string += mv(tokens[1], tokens[2]);
    }
    else
    {
        out_string += "Invalid Command! for help type 'man'\n";
    }
    return out_string;
}

void FileSystem::run_script(ifstream& file) {
    while(file.peek() != EOF) {
		string command;
		getline(file, command);
		if (!command.compare("exit"))
			exit(-1);
		else {
            *out_stream << run(command);
		}
	}
}

void thread_wrapper(FileSystem file_system, string file_name) {
    struct stat buffer; 
    
    ifstream file(file_name);  
    ofstream out_file(file_name.substr(0, file_name.size()-4) + "_out.txt");
    file_system.out_stream = &out_file;
    if(!(stat (file_name.c_str(), &buffer) == 0)) {
        out_file << file_name << " does not exists\n";
    }
    else {    
        file_system.run_script(file);
    }
    file.close();
    out_file.close();
}