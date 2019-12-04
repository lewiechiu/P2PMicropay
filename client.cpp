#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "connectAPI.h"
using namespace std;

int main(int argc, char const *argv[]) 
{ 
    cout << "Welcome to P2P Micropay" << endl;
    cout << "current server Ip:Port 127.0.0.1:8080" << endl;
    Client srv("127.0.0.1", "8080");
    srv.connect2Server();
    char option = 'a';
    while(1){
        while (option != 'R' && option != 'L' && option != 'M' && option != 'C' && option != 'Q'){
            cout << "press R to register" << endl;
            cout << "press L to login" << endl;
            cout << "press M to Modify Server config" << endl;
            cout << "press Q to Quit" << endl;
            cout << "press A to connect to server" << endl;
            cout << "press G to List the online clients" << endl;
            cin >>  option;
        }
        if (option == 'R'){
            string UserName;
            cout << "User name: ";
            cin >> UserName;
            UserName.append("\n");
            srv.Register(UserName);
            // Check server status
        }
        else if (option == 'L'){
            string UserName;
            string Port;
            cout << "User Name: ";
            cin >> UserName;
            cout << "Port Number: ";
            cin >> Port;
            srv.Login(UserName, Port);
            // srv.Listen();
        }
        else if (option == 'M'){
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
        else if (option == 'G'){
            // GET current list of clients
            srv.List();
        }
        else if (option == 'C'){
            // Chat with other client
            
        }
        else if (option == 'Q'){
            srv.GoOffline();
            srv.Terminate();
            break;
        }
        else if (option == 'A'){
            srv.connect2Server();
        }
        option=0;
    }
    
    return 0; 
} 

