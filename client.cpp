#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "connectAPI.h"
using namespace std;

int main(int argc, char const *argv[]) 
{ 
    // cout << "Welcome to P2P Micropay" << endl;
    // cout << "current server Ip:Port 127.0.0.1:8080" << endl;
    Client srv("127.0.0.1", "8080");
    srv.connect2Server();

    char resp[1024];
    char option = 'a';
    while(1){
        while (option != 'R' && option != 'L' && option != 'M' && option != 'C' && option != 'Q' && option != 'G'){
            if (!srv.LoginState()){

                cout << "press R to register" << endl;
                cout << "press M to Modify Server config" << endl;
                cout << "press L to login" << endl;
            }
            if (srv.LoginState()){
                cout << "press G to List the online clients" << endl;
                cout << "press Q to Quit" << endl;
                cout << "C: chat with user" << endl;
            }
            
            cin >>  option;
        }
        if (option == 'R' && !srv.LoginState()){
            string UserName;
            cout << "User name: ";
            cin >> UserName;
            UserName.append("\n");
            srv.Register(UserName);
            // Check server status
        }
        else if (option == 'L' && !srv.LoginState()){
            string UserName;
            string Port;
            cout << "User Name: ";
            cin >> UserName;
            cout << "Port Number: ";
            cin >> Port;
            
            srv.Login(UserName, Port);
            // srv.Listen();
        }
        else if (option == 'M' && !srv.LoginState()){
            // Modify connect, otherwise uses default.
            string IP, port;
            srv.ServerLocation();
            cout << "Ip: ";
            cin >> IP;
            cout << "Port: ";
            cin >> port;
            srv.UpdateConfig(IP, port);

            srv.connect2Server();

        }
        else if (option == 'G' && srv.LoginState()){
            // GET current list of clients
            srv.GrabOnlineList();
        }
        else if (option == 'C' && srv.LoginState()){
            // Chat with other client
            
            string IP, port, amount;
            cout << "Recipient Ip: ";
            cin >> IP;
            cout << "Recipient Port: ";
            cin >> port;
            cout << "Transferring: ";
            cin >> amount;
            srv.StartChatServer(IP, port, amount);
        }
        else if (option == 'Q' && srv.LoginState()){
            srv.GoOffline();
            srv.Terminate();
            break;
        }
        option=0;
    }
    
    return 0; 
    // int cnt = 0;
    // while(1){
    //     cnt ++ ;
    //     scanf("%s", resp);

    //     string cmd(resp);
    //     cmd.push_back('\n');
    //     srv.SendCommand(cmd, resp);
    //     if (cnt == 2){
    //         cout << "Reading another line" << endl;
    //         srv.ReadLine(resp);
    //     }
    // }
} 

