#ifndef SRV_H
#define SRV_H


#include <string>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

enum status {SUCCESS, EXIST, UNDEFINED, ERROR};
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
        void SetFD(int &sock);
        void ReadMsg(char* );
        void SendMsg(char* );
        status RegisterClient(string name);
        status GoOnline(string name, string port);
        void GetClientList();
        
};

#endif