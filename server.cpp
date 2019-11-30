#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

struct User{
    string Name;
    string Ip;
    string Port;
};

int main(int argc, const char** argv) {
    int fd;
    // socket()
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }

    vector<User> OnLineUser;
    vector<User> Offline;


    struct sockaddr_in srv;
    struct sockaddr_in cli;
    int nbytes;
    char buf[512] = "";
    unsigned cli_len = sizeof(cli);

    srv.sin_family = AF_INET;
    srv.sin_port = htons(stoi(argv[1]));
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind()
    if (bind(fd, (struct sockaddr *)&srv, sizeof(srv)) < 0){
        perror("bind"); exit(1);
    }

    // listen()
    if (listen(fd, 100) < 0){
        perror("listen");
        exit(1);
    }
    int cnt = 0;
    cout << "listening on port:" << argv[1] << endl;
    fd = accept(fd, (struct sockaddr *)&cli, &cli_len);
    while(1){
        //accept()
        if (fd <0 ){
            perror("accept");
            exit(1);
        }
        cout << "recv from " << inet_ntoa(cli.sin_addr) << ":" << cli.sin_port << endl;
        if ((nbytes = read(fd, buf , sizeof(buf))) < 0 ){
            perror("read");
            exit(1);
        }
        string command(buf);
        cout << "msg from:" << inet_ntoa(cli.sin_addr) << " " << buf << endl;
        if (command.find("REGISTER") != string::npos){
            
            write(fd, "100 OK", 7);
        }
        else{

        }
        memset( buf, '\0', sizeof(char*));
        break;
    }
    // close(newfd);
    

    // read()
    




    close(fd);
    return 0;
}