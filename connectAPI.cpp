
#include "connectAPI.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

using namespace std;

void Server::connect2Server(){
    struct sockaddr_in srv; 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    { 
        printf("\n Socket creation error \n"); 
        exit(1);
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = inet_addr(Ip.c_str());
    // Convert IPv4 and IPv6 addresses from text to binary form 

    if (connect(sock , (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        perror("Connection Failed");
        exit(1);
    }
}

void Server::Terminate(){close(sock);}

void Server::Register(string Username){
    char buf[256];
    string command = "REGISTER ";
    command.append(Username);
    write(sock, command.c_str(), command.length());
    read(sock, buf, 256);
    string response(buf);
    // cout << response << "*" << endl;
    if (stoi(response.substr(0,3)) == 210){
        cout << "ERROR: Register fail, Please try again!" << endl;
    }
    else{
        cout << "Registered!" << endl;
    }
}