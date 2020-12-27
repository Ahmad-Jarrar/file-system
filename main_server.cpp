// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <thread>
#include <vector>
#include "config.h"
#include "FileSystem.h"
#define PORT 95 

void make_new_socket(int socket_id) {
    FileSystem file_system;
    char buffer[1024] = {0}; 
    while(read(socket_id, buffer, 1024) > 0) {
        printf("Client sent: %s\n", buffer);
        string command(buffer);
        string output = file_system.run(command);
        cout << output.c_str() << endl;
        send(socket_id , output.c_str(), strlen(output.c_str()) , 0 );
        bzero(buffer, sizeof(buffer));
    }
}


int main(int argc, char const *argv[]) { 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }

    vector<thread> threads;
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        threads.push_back(thread(make_new_socket, new_socket));
    }
    
    for(int i = 0; i < threads.size(); i++)
        threads[i].join();

    printf("Hello message sent\n"); 
    return 0;   
}