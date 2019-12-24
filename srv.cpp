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

// enum status {SUCCESS, EXIST, UNDEFINED, ERROR};
// enum Mode {REGISTER, LOGIN, QUIT, GET, PARSE_ERROR};

Server SERVER;
status Server::GoOnline(string name, string port, string ip){
    map<string, User*>::iterator itr;
    itr = Users.find(name);
    if (itr == Users.end())
        return status::ERROR;
    else if (itr->second->isOnline)
        return status::ERROR;
    else{
        itr->second->isOnline = true;
        itr->second->Port = port;
        itr->second->Ip = ip;
    }
        
    return status::SUCCESS;
}

status Server::RegisterClient(string name){
    name.erase(0, 9);
    while(name.find('\n')!= string::npos)
        name.replace(name.find('\n'), 1, "");
    cout << "**** Register User ****" << endl;
    cout << "name: " <<  name << endl;
    map<string, User*>::iterator itr;
    itr = Users.find(name);

    if (itr == Users.end()){
        User* new_client = new User;
        new_client->Name = name;
        Users.insert(pair<string, User*>(name, new_client) );
        cout <<"Current user size: " << Users.size() << endl;
        cout << "***** Finish Register Process ******" << endl;
        return SUCCESS;
    }
    else{
        cout << "FATAL ERROR: name exists! " << endl;
        cout << "***** Finish Register Process ******" << endl;
        return EXIST;
    }
    
}

status Server::GoOffline(string name){
    map<string, User*>::iterator itr;
    itr = Users.find(name);
    itr->second->isOnline = false;
    return SUCCESS;
}

void Server::GetClientList(string name, stringstream &out){
    map<string, User*>::iterator itr;
    itr = Users.find(name);
    out << itr->second->balance << endl;
    out << "currently online client count: ";
    int cnt = 0;
    for (auto i = Users.begin() ; i != Users.end() ; i++){
        if (i->second->isOnline)
            cnt ++;
    }
    out << cnt << endl;
    for (auto i = Users.begin() ; i != Users.end() ; i++){
        if (i->second->isOnline)
            out << i->second->Name << "#" << i->second->Ip << "#" << i->second->Port << endl;
    }
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

void* client(thread_arg* arg){
    int sock = arg->sock;
    SendMsg(sock, "hello!");
    char buf[1024] = {0};
    bool logged_in = false;
    string name;
    string IP_addr = arg->ip;
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
                if (SERVER.GoOnline(name, cmd, IP_addr) == SUCCESS){
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
                else {
                    SendMsg(sock, "210 fail\n");
                }
            }
        }
        cmd.clear();
    }
}