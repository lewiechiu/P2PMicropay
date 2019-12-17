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
    for (int i = 0;i< Users.size(); i++){
        if (Users[i]->Name == name){
            Users[i]->isOnline = true;
            Users[i]->Port = port;
            return SUCCESS;
        }
    }
    return status::ERROR;
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
    cout << msg_ ;
    write(sock, msg_.c_str(), msg_.length());
    return;
}

void ReadMsg(int& sock,char* msg, bool print){
    memset(msg, 0, 1024);
    read(sock, msg, 1024);
    return ;
}

void* client(void* sk){
    int *sock = (int*)sk;
    SendMsg(*sock, "hello!");
    char buf[1024] = {0};
    bool logged_in = false;
    while(1){

        ReadMsg(*sock,buf, true);        
        string cmd(buf);
        if (cmd.find("REGISTER") != string::npos ){
            status response = SERVER.RegisterClient(cmd);
            if (response == SUCCESS){
                char msg[] = "100 OK\n";
                SendMsg(*sock, msg);
            }
            else if (response == EXIST){
                char msg[] = "210 fail\n";
                SendMsg(*sock, msg);
            }
            else{
                SendMsg(*sock, "Don't know what happened\n");
            }
        }
        else if (cmd.find("LOGIN") != string::npos){
            if (SERVER.GoOnline(cmd.substr(0, cmd.find('#')), cmd.substr(cmd.find('#') + 1, cmd.length())) == SUCCESS){
                logged_in = true;
            }
        }
        else if (cmd.find("List") != string::npos){
            SERVER.GetClientList();
        }
        else if (cmd.find("Exit")!= string::npos){
            cmd.erase(0, 5);
            SERVER.GoOffline(cmd);
        }
    }
}