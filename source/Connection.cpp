#include "../headers/Connection.h"

Connection::Connection(int socket_id, string ip_address) {
    this->socket_id = socket_id;
    this->ip_address = ip_address;
    // Get username
    
    char name_buffer[50] = {0};
    string prompt = ip_address+"#\nusername: ";
    cout << prompt << endl;
    send(socket_id , prompt.c_str(), strlen(prompt.c_str()) , 0 );
    bzero(name_buffer, sizeof(name_buffer));
    read(socket_id, name_buffer, 50);
    printf("Username: %s\n", name_buffer);
    this->username = string(name_buffer);
    cout << "New connection created for " << username << endl;
}

void Connection::start() {
    char buffer[1500] = {0};
    string prompt = username+"@"+ip_address+"# ";
    send(socket_id , prompt.c_str(), strlen(prompt.c_str()) , 0 );
    
    while(read(socket_id, buffer, 1500) > 0) {
        printf("Client sent: %s\n", buffer);
        string command(buffer);
        string output = file_system.run(command) + prompt;
        cout << output.c_str() << endl;
        send(socket_id , output.c_str(), strlen(output.c_str()) , 0 );
        bzero(buffer, sizeof(buffer));
    }
    cout << "Connection with "<< username << " closed!\nThread exiting ..."<< endl;
}