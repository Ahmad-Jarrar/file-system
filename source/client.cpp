// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <string>
#include <iostream>
#define PORT 95
#define BUFF_SIZE 250

using namespace std;

int main(int argc, char const *argv[]) 
{ 
    int sock = 0; 
    string server_ip;
    struct sockaddr_in serv_addr; 
    char buffer[BUFF_SIZE] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
    cout << "Enter Server IP: ";
    getline(cin, server_ip);
    cout << "Connecting to server at " << server_ip << " Port " << PORT << endl;

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    bool exit = false;
    while(true) {
        string output = "";
        while (true)
        {   int chars_read;
            chars_read = read(sock, buffer, BUFF_SIZE);
            // Connection error
            if (chars_read < 1)
            {
                exit = true;
                break;
            }
            output += buffer;
            bzero(buffer, sizeof(buffer));
            if (output.back() == '\4')
            {
                output = output.substr(0, output.size()-1);
                break;
            }
        }
        // Error
        if (exit)
            break;
        
        cout << output;
        string input;
        getline(cin, input);
        if (!input.compare("exit"))
        {
            send(sock, "\25", strlen("\25"), 0);
            break;
        }
        else if (input.length() > 0) {
            input += "\4";
            const char * str = input.c_str();
            send(sock, str, strlen(str), 0);
        }
    }
    cout << "Connection closed" << endl;
    return 0; 
}