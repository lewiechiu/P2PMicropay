#include "srv.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
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
    
    cout << "Register name: " <<  name << endl;
    
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
    for (int i = 0;i< Users.size(); i++){
        
        if (Users[i]->Name == name){
            Users[i]->isOnline = false;
        }
    }
    return SUCCESS;
}

void Server::GetClientList(string name, stringstream &out){
    for (int i = 0;i< Users.size(); i++){
        
        if (Users[i]->Name == name){
            out << Users[i]->balance << endl;
        }
    }
    out << "currently online client count: ";
    int cnt = 0;
    for (int i=0;i<Users.size(); i++){
        if (Users[i]->isOnline)
            cnt ++;
    }
    out << cnt << endl;
    for (int i = 0;i< Users.size(); i++){
        if (Users[i]->isOnline){
            out << Users[i]->Name << "#" << Users[i]->Port << endl;
        }
    }
}





Mode parsing(string cmd){

}


void SendMsg(int& sock,const char* msg){
    string msg_(msg);
    msg_.push_back('\n');
    cout << "<-- " << msg_ ;
    write(sock, msg_.c_str(), msg_.length());
    return;
}

void ReadMsg(int& sock,char* msg, bool print){
    memset(msg, 0, 1024);
    read(sock, msg, 1024);
    if (print){
        cout << "--> " << msg << endl;
    }
    return ;
}

void* client(int sock){
    SendMsg(sock, "hello!");
    char buf[1024] = {0};
    bool logged_in = false;
    string name;
    while(1){

        ReadMsg(sock,buf, true);
        string cmd(buf);
        if(logged_in){
            if (cmd.find("List") != string::npos){
                stringstream out;
                SERVER.GetClientList(name, out);
                SendMsg(sock, out.str().c_str());
            }
            else if (cmd.find("Exit")!= string::npos){
                cmd.erase(0, 5);
                SERVER.GoOffline(name);
                SendMsg(sock, "Goodbye");
                close(sock);
                break;
            }
        }
        else{
            if (cmd.find("LOGIN") != string::npos){
                cmd.replace(0, 5, "");
                name = cmd.substr(0, cmd.find('#'));
                cmd.replace(0, name.length() + 1, "");
                if (SERVER.GoOnline(name, cmd) == SUCCESS){
                    logged_in = true;
                    SendMsg(sock, "100 OK\n");
                    stringstream out;
                    SERVER.GetClientList(name, out);
                    SendMsg(sock, out.str().c_str());
                }
                else{
                    SendMsg(sock, "220 AUTH_FAIL\n");
                }
            }
            else if (cmd.find("REGISTER") != string::npos ){
                status response = SERVER.RegisterClient(cmd);
                if (response == SUCCESS){
                    SendMsg(sock, "100 OK\n");
                }
                else if (response == EXIST){
                    SendMsg(sock, "210 fail\n");
                }
            }
        }
        cmd.clear();
    }
}