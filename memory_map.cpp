#include "memory_map.h"

string block_map(){

    string s = "========================================BLOCK MAP========================================\n";
        + " # -> Occupied (Folder data)        * -> Occupied (File Data)            _ -> Unoccupied \n";
        + "=========================================================================================\n";
    for (size_t i = 0; i < ADDRESS_SPACE/BLOCK_SIZE; i++)
    {   
        if (i % 8 == 0)
            s+="\n\t\t";
        
        Header header((char)i);

        if (header.is_occupied)
            if (header.is_dir)
                s+= "#\t";
            else
                s+= "*\t";
        else
            s+= "_\t";
    }
    s+= "\n=========================================================================================\n";
    return s;
}

string disk_usage() {
    string s = "=========================================== Disk Usage(%) ==========================================\n"
        + string("              # -> Used                                                    * -> Unused              \n\n");
    
    int total = ADDRESS_SPACE/BLOCK_SIZE;
    int used = 0;
    for (size_t i = 0; i < total; i++) {   
        Header header((char)i);
        if (header.is_occupied)
            used++;
    }

    int used_percent = ((float)used/(float)total) * 100;
    for (size_t i = 0; i < used_percent; i++)
        s+= "#";

    for (size_t i = 0; i < 100-used_percent; i++)
        s+= "*";

    s += "\n____________________________________________________________________________________________________\n";
    return s;
}

string show_memory_map(Entry entry) {
    Header first_header = Header(entry.file_start);

    string s = "\t=====Start=====\n";
    while (true) {
        s+= first_header.stringify();
        if(first_header.next == 0)
            break;
        s+= "\t      ||      \n";
        s+= "\t      ||      \n";
        s+= "\t     \\  /     \n";
        s+= "\t      \\/      \n";
        first_header.read(first_header.next);
    }
    s += "\t=====End=====\n";
    return s;
}