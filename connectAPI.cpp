
#include "connectAPI.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

using namespace std;


void Client::connect2Server(){
    struct sockaddr_in srv; 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    { 
        printf("Socket creation error \n"); 
        return ;
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(stoi(port));
    srv.sin_addr.s_addr = inet_addr(Ip.c_str());
    // Convert IPv4 and IPv6 addresses from text to binary form 

    if (connect(sock , (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        printf("Socket creation error, please config \n"); 
        return ;
    }
}

void Client::Terminate(){close(sock);}

void Client::Register(string Username){
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


void Client::UpdateConfig(string IP, string PORT) {Ip = IP; port = PORT;}

void Client::Login(string User, string port){
    string command(User);
    command.append("#");
    command.append(port);
    hosting_port = port;
    // cout << command;
    write(sock, command.c_str(), command.length());
    char buf[10000] = {0};
    read(sock, buf, 10000);
    cout << buf << endl;
    string response(buf);
    if (response.find("220 AUTH_FAIL") != string::npos){
        cout << "Login Fail, Please try again" << endl;
        return;
    }
    else{

    }
}

void Client::Listen(){
    
}

void Client::ServerLocation(){
    cout << "Original Ip: " << Ip << " port: " << port << endl;
}

void Client::GoOffline(){
    write(sock, "Exit", 5);
    char response[10] = {0};
    read(sock, response, 10);
    string resp(response);
    if (resp.find("Bye") != string::npos){
        cout << "Disconnected!" << endl;
        return;
    }
}

void Client::List(){
    write(sock, "List", 5);
    char resp[10000] = {0};
    read(sock, resp, 10000);
    cout << resp << endl;
    return;
}