#include "memory_map.h"

void block_map(){

    cout << "========================================BLOCK MAP========================================" << endl;
    cout << " # -> Occupied (Folder data)        * -> Occupied (File Data)            _ -> Unoccupied " << endl;
    cout << "=========================================================================================" << endl;
    for (size_t i = 0; i < ADDRESS_SPACE/BLOCK_SIZE; i++)
    {   
        if (i % 8 == 0)
        {   
            cout << endl;
            cout << "\t\t";
        }
        
        Header header((char)i);

        if (header.is_occupied)
            if (header.is_dir)
                cout << "#\t";
            else
                cout << "*\t";
        else
            cout << "_\t";
    }
    cout << endl;
    cout << "=========================================================================================" << endl;
    
}

void disk_usage() {
    cout << "=========================================== Disk Usage(%) ==========================================" << endl;
    cout << "              # -> Used                                                    * -> Unused              " << endl;
    cout << endl;
    
    int total = ADDRESS_SPACE/BLOCK_SIZE;
    int used = 0;
    for (size_t i = 0; i < total; i++)
    {   
        Header header((char)i);
        if (header.is_occupied)
            used++;
    }

    int used_percent = ((float)used/(float)total) * 100;
    for (size_t i = 0; i < used_percent; i++)
    {
        cout << "#";
    }

    for (size_t i = 0; i < 100-used_percent; i++)
    {
        cout << "*";
    }

    cout << endl;
    cout << "____________________________________________________________________________________________________" << endl;
}

void show_memory_map(Entry entry) {
    Header first_header = Header(entry.file_start);

    cout << "\t=====Start=====" << endl;
    while (true) {
        first_header.print();
        if(first_header.next == 0)
            break;
        cout << "\t      ||      " << endl;
        cout << "\t      ||      " << endl;
        cout << "\t     \\  /     " << endl;
        cout << "\t      \\/      " << endl;
        first_header.read(first_header.next);
    }
    cout << "\t=====End=====" << endl;
}