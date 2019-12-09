#include "srv.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

void Server::SetFD(int &sock){fd = sock;}

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

void Server::GetClientList(){

}

void Server::ReadMsg(char* msg = nullptr){
    if (msg == nullptr){
        // Use buf as ptr
        memset( buf, 0, 512);
        if (read(fd, buf , 512) < 0){
            perror("read foul");
            return;
        }
        for (int i=0;buf[i]!=0; i++)
            printf("%c", buf[i]);
    }
    else{
        if (read(fd, msg , 512) < 0){
            perror("read foul");
            return;
        }
        for (int i=0;msg[i]!=0; i++)
            printf("%c", msg[i]);
        printf("*");
    }
}

void Server::SendMsg(char* msg){
    string msg_(msg);
    msg_.push_back('\n');
    write(fd, msg, msg_.length());
    return;
}