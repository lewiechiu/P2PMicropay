
#include "connectAPI.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;

void Client::SendCommand(string command, char* resp){
    write(sock, command.c_str(), command.length());
    return;
}
void Client::ReadLine(char* resp, bool print){
    memset(resp, 0, 1024);
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);
    /* Wait up to five seconds. */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    retval = select(sock+1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if (retval)
        read(sock, resp, 1024);
        /* FD_ISSET(0, &rfds) will be true. */
    if (print)
        printf("%s", resp);
    return ;
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
    printf("%s\n", buf);
}

void Client::Terminate(){close(sock);}

void Client::Register(string Username){
    string command = "REGISTER#";
    command.append(Username);
    SendCommand(command, rep);
    ReadLine(rep, false);
    string response(rep);
    if (stoi(response.substr(0,3)) == 210){
        cout << "ERROR: Register fail, Please try again!" << endl;
    }
    else{
        cout << "Registered!" << endl;
    }
}


void Client::UpdateConfig(string IP, string PORT) {Ip = IP; port = PORT;}

void Client::GrabOnlineList(){
    string list("List");
    list.push_back('\n');
    SendCommand(list, rep);
    cout << "Balance: ";
    ReadLine(rep, true);
    string response(rep);
    // cout << "*" << endl;
    if (response.find("#") == string::npos){
        ReadLine(rep, true);
        // cout << "**" << endl;
        string users(rep);
        if (users.length() <= 30){
            // The returned string does not contain User and IP
            users.replace(0, 27, "");
            int num_user = atoi(users.c_str()) + 1;
            for (int num = 0; num<num_user;num++){

                ReadLine(rep, true);
                // cout << "***" << endl;
            }
        }
        else{
            // cout << "***" << endl;
        }
    }
    
    return;
}
void Client::Login(string User, string port){
    string command(User);
    command.append("#");
    command.append(port);
    hosting_port = port;
    command.append("\n");
    SendCommand(command, rep);
    cout << "Balance: ";
    ReadLine(rep, true);
    string response(rep);
    // cout << "*" << endl;
    if (strcmp(rep, "220 AUTH_FAIL\n") == 0){
        cout << "Login Fail, Please try again" << endl;
        return;
    }
    else{
        isLoggedIn = true;
    }
    if (response.find("#") == string::npos){
        ReadLine(rep, true);
        cout << "**" << endl;
        string users(rep);
        if (users.length() <= 30){
            // The returned string does not contain User and IP
            users.replace(0, 27, "");
            int num_user = atoi(users.substr(0, users.find('\n')).c_str());;
            for (int num = 0; num<num_user;num++){

                ReadLine(rep, true);
                cout << "***" << endl;
            }
        }
        else{
            cout << "****" << endl;
        }
    }
    
    // else: contains all of them in one line.
    
}

void Client::StartChatServer(){
    
}

void Client::ServerLocation(){
    cout << "Original Ip: " << Ip << " port: " << port << endl;
}

void Client::GoOffline(){
    string exit("Exit\n");
    SendCommand(exit, rep);
    ReadLine(rep, false);
    string resp(rep);
    if (resp.find("Bye") != string::npos){
        cout << "Disconnected!" << endl;
        return;
    }
}

