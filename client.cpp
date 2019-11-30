#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "connectAPI.h"
using namespace std;

int main(int argc, char const *argv[]) 
{ 
    Server srv(argv[1] , stoi(argv[2]));
    srv.connect2Server();
    char option = 'a';
    if (argc <=2){
        perror("argument missing!");
        exit(1);
    }
    while(1){
        while (option != 'R' && option != 'L'){
            cout << "press R to register or L to login" << endl;
            cin >>  option;
        }
        if (option == 'R'){
            string UserName;
            cout << "User name: ";
            cin >> UserName;
            srv.Register(UserName);
            // Check server status
        }
        else if (option == 'L'){
            // Log in
            // Setup connection
            // Ask for User name and port
            // Server response handling
        }
        else if (option == 'Q'){
            srv.Terminate();
            break;
        }
        option=0;
    }
    
    return 0; 
} 

