
#include "connectAPI.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

void Client::SendCommand(string command, char* resp){

    memset(resp, 0, 1024);
    write(sock, command.c_str(), command.length());
    read(sock, resp, 1024);
    puts(resp);
    return;
}

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

    char buf[100] = {0};
    read(sock, buf, 100);
    cout << buf << endl;
}

void Client::Terminate(){close(sock);}

void Client::Register(string Username){
    string command = "REGISTER#";
    command.append(Username);
    SendCommand(command, rep);

    string response(rep);
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
    command.append("\n");
    cout << command << endl;
    SendCommand(command, rep);

    string response(rep);
    if (response.find("220 AUTH_FAIL") != string::npos){
        cout << "Login Fail, Please try again" << endl;
        return;
    }
    else{
        int cnt = 0;
        while(response.find('\n')!=string::npos){
            response.replace(response.find('\n'), 1, "_");
        }
        cout << response << endl;
        
    }
}

void Client::StartChatServer(){
    
}

void Client::ServerLocation(){
    cout << "Original Ip: " << Ip << " port: " << port << endl;
}

void Client::GoOffline(){
    string exit("Exit\n");
    SendCommand(exit, rep);
    string resp(rep);
    cout << resp << endl;
    if (resp.find("Bye") != string::npos){
        cout << "Disconnected!" << endl;
        return;
    }
}

void Client::List(){
    string list("List\n");
    SendCommand(list, rep);
    string resp(rep);
    cout << resp << endl;
    return;
}