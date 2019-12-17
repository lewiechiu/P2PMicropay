#ifndef SRV_H
#define SRV_H


#include <string>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

enum status {SUCCESS, EXIST, UNDEFINED, ERROR};
enum Mode {REGISTER, LOGIN, QUIT, GET, PARSE_ERROR};
// Server returns one of the above.


struct User{
    string Name = "", Ip = "", Port = "";
    int balance = 10000;
    bool isOnline;
};

class Server {
    private:
        int User_cnt = 0, fd;
        char buf[512] = "";
        vector<User* > Users;
    public:
        status RegisterClient(string name);
        status GoOnline(string name, string port);
        status GoOffline(string name);
        void GetClientList();
        
};

void SendMsg(int& sock, char* msg);
void ReadMsg(int& sock, char* msg, bool print);

Mode parsing(string cmd);

void * client(void* sk);

#endif