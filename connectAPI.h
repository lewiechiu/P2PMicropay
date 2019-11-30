#ifndef CONNECTAPI_H
#define CONNECTAPI_H

#include <string>
using namespace std;

class Server{
    private:
        int sock;
        int port;
        string Ip;
        bool isEncrypte=false;
    public:
        Server(string IP, int PORT){port = PORT; Ip = IP; };
        void connect2Server();
        void Terminate();
        void Register(string);
        void Login()
        // Setup connection to server
        
    
};

#endif