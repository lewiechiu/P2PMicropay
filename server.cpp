#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <vector>
#include "srv.h"
#include <thread>
#include "ThreadPool.h"
using namespace std;



int main(int argc, const char** argv) {
    // CMD args
    bool status = false, present = false, msg_detail = false;
    int opt = 1;
    ThreadPool pool(5);
    if (argc <= 1){
        perror("arg missing");
        exit(1);
    }
    if (argc >2){
        if (argv[2] == "-d"){
            // Show Reg, Login, Leave
            cmd_state = FUNCTION;
        }
        else if(argv[2] == "-s"){
            // Show List + above
            cmd_state = LIST;
        }
        else{
            // Show msg between cli and server
            cmd_state = FULL;
        }
    }

    int fd, newfd;

    struct sockaddr_in srv;
    struct sockaddr_in cli;

    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("socket");
        exit(1);
    }
    // Uses socket option and prevents bind error.
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
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
    cout << "Server Ready..." << endl;
    unsigned cli_len = sizeof(cli);

    while(1){
        //accept()
        newfd = accept(fd, (struct sockaddr *)&cli, &cli_len);
        
        // Creates Thread arg
        thread_arg *args = new thread_arg;
        args->ip = inet_ntoa(cli.sin_addr);
        args->sock = newfd;

        pool.enqueue(client, args);
    }
    return 0;
}