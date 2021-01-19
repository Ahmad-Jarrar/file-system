#include "../headers/Connection.h"

#define BUFF_SIZE 250

Connection::Connection(int socket_id, string ip_address) {
    this->socket_id = socket_id;
    this->ip_address = ip_address;
    // Get username
    
    char name_buffer[50] = {0};
    string prompt = ip_address+"#\nusername: \4";
    cout << prompt << endl;
    send(socket_id , prompt.c_str(), strlen(prompt.c_str()) , 0 );
    bzero(name_buffer, sizeof(name_buffer));
    read(socket_id, name_buffer, 50);
    this->username = string(name_buffer);
    username = username.substr(0, username.size()-1);
    cout << "New connection created for " << username << endl;
}

void Connection::start() {
    char buffer[BUFF_SIZE] = {0};
    string prompt = username+"@"+ip_address+"# \4";
    send(socket_id , prompt.c_str(), strlen(prompt.c_str()) , 0 );
    
    bool exit = false;
    while (true)
    {
        string input = "";
        while(true) {
            int chars_read = read(socket_id, buffer, BUFF_SIZE);
            input += string(buffer);
            bzero(buffer, sizeof(buffer));
            // Connection Error or exit command
            if (chars_read < 1 || input[input.size()-1] == '\25')
            {
                exit = true;
                break;
            }
            if (input[input.size()-1] == '\4')
            {
                input = input.substr(0, input.size()-1);
                break;   
            }
        }
        // Error
        if (exit)
            break;

        cout << "request: " << input << endl;
        string output = file_system.run(input) + prompt;
        cout << "response: " << output.c_str() << endl;
        send(socket_id , output.c_str(), strlen(output.c_str()) , 0 );
    }
    file_system.close();
    cout << "Connection with "<< username << " closed!\nThread exiting ..."<< endl;
}