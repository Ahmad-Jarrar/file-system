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

void show_all_files() {

}

void show_memory_map(Entry entry) {
    Header first_header = Header(entry.file_start);

    cout << "=====Start=====" << endl;
    while (true) {
        first_header.print();
        if(first_header.next == 0)
            break;
        cout << "      ||      " << endl;
        cout << "      ||      " << endl;
        cout << "     \\  /     " << endl;
        cout << "      \\/      " << endl;
        first_header.read(first_header.next);
    }
    cout << "=====End=====" << endl;
}