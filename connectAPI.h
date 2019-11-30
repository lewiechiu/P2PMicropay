#ifndef CONNECTAPI_H
#define CONNECTAPI_H

#include <string>
using namespace std;

class Client{
    private:
        int sock;
        string port;
        string hosting_port;
        string Ip;
        bool isEncrypte=false;
    public:
        Client(string IP, string PORT){port = PORT; Ip = IP; };
        void ServerLocation();
        void connect2Server();
        void UpdateConfig(string IP, string PORT);
        void Terminate();
        void Register(string);
        void Login(string User, string port);
        void GoOffline();
        void List();
        void Listen();

        // Setup connection to server
        
    
};

#endif