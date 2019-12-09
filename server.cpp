#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <vector>
#include "srv.h"
#include <thread>
using namespace std;

int main(int argc, const char** argv) {
    // CMD args
    bool status = false, present = false, msg_detail = false;
    
    if (argc <= 1){
        perror("arg missing");
        exit(1);
    }
    if (argc >2){
        if (argv[2] == "-d"){
            // Show Reg, Login, Leave
            status = true;
        }
        else if(argv[2] == "-s"){
            // Show List + above
            status = present = true;
        }
        else{
            // Show msg between cli and server
            status = present = msg_detail = true;
        }
    }

    int fd, newfd;
    // socket()
    Server SERVER;
    char buf[512] = {0};

    struct sockaddr_in srv;
    struct sockaddr_in cli;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }
    srv.sin_family = AF_INET;
    srv.sin_port = htons(8080);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind()
    if (bind(fd, (struct sockaddr *)&srv, sizeof(srv)) < 0){
        perror("bind"); exit(1);
    }

    // listen()
    if (listen(fd, 10) < 0){
        perror("listen");
        exit(1);
    }
    int cnt = 0;

    unsigned cli_len = sizeof(cli);
    newfd = accept(fd, (struct sockaddr *)&cli, &cli_len);
    SERVER.SetFD(newfd);
    SERVER.SendMsg("Hello!!");
    cout << "Client connected" << endl;

    while(1){
        //accept()
        
        SERVER.ReadMsg(buf);
        // Start to process!
        string command(buf);
        if (command.find("REGISTER#") != string::npos){
            cout << "comm: " <<  command << endl;
            if (SERVER.RegisterClient(command) == EXIST){
                SERVER.SendMsg("210 FAIL");
            }
            else{
                SERVER.SendMsg("100 OK");
            }
        }
        else if (command.find("Exit") != string::npos){
            close(newfd);

        }
        command.clear();
    }
    // close(newfd);
    

    // read()
    return 0;
}