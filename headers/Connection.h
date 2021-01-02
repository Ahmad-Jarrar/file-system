#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <sys/types.h> 
#include <netdb.h>
#include <arpa/inet.h> 
#include "FileSystem.h"

class Connection {
public:
    string ip_address; 
    int socket_id;
    string username;
    FileSystem file_system;

    Connection(int, string);
    void start();
    
};