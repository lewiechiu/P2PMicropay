#include "srv.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <vector>
#include <mutex>
using namespace std;

Server SERVER;
status Server::GoOnline(string name, string port){

}

status Server::RegisterClient(string name){
    name.erase(0, 9);
    while(name.find('\n')!= string::npos)
        name.replace(name.find('\n'), 1, "");
    
    cout << "new user name: " <<  name << endl;
    
    for (int i=0;i<Users.size(); i++){
        if (Users[i]->Name == name){
            cout << "Exist, failed to create" << endl;
            return EXIST;
        }
    }
    cout << "not exist, allow to create" << endl;
    User* new_client = new User;
    new_client->Name = name;
    Users.push_back(new_client);
    cout <<"User size: " << Users.size() << endl;
    for (int i=0;i<Users.size(); i++){
        cout << Users[i]->Name << "#" << Users[i]->balance << endl;
    }
    return SUCCESS;
}

status Server::GoOffline(string name){
    
    
    return SUCCESS;
}

void Server::GetClientList(){

}





Mode parsing(string cmd){

}


void SendMsg(int& sock, char* msg){
    string msg_(msg);
    msg_.push_back('\n');
    write(sock, msg, msg_.length());
    return;
}

void ReadMsg(int& sock,char* msg, bool print){
    memset(msg, 0, 1024);
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    retval = select(sock+1, &rfds, NULL, NULL, &tv);
    if (retval)
        read(sock, msg, 1024);
    if (print)
        printf("%s", msg);
    return ;
}

void* client(int* sock){
    char buf[512] = {0};
    ReadMsg(*sock,buf, true);
    string cmd(buf);
    if (cmd.find("REGISTER") != string::npos){
        SERVER.RegisterClient(cmd);
    }
    else if (cmd.find("LOGIN") != string::npos){
        SERVER.GoOnline(cmd.substr(0, cmd.find('#')), cmd.substr(cmd.find('#') + 1, cmd.length()));
    }
    else if (cmd.find("List") != string::npos){
        SERVER.GetClientList();
    }
    else if (cmd.find("Exit")!= string::npos){
        cmd.erase(0, 5);
        SERVER.GoOffline(cmd);
    }
}