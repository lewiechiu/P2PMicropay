#ifndef CONNECTAPI_H
#define CONNECTAPI_H

#include <string>
using namespace std;

class Client{
    private:
        int sock;
        char rep[1024] = {0};
        string port;
        string hosting_port;
        string Ip;
        bool isEncrypte=false;
        bool isLoggedIn = false;
    public:
        Client(string IP, string PORT){port = PORT; Ip = IP; };
        void ServerLocation();
        void connect2Server();
        void UpdateConfig(string IP, string PORT);
        void Terminate();
        void Register(string);
        void Login(string User, string port);
        void GoOffline();
        void GrabOnlineList();
        void StartChatServer();
        void SendCommand(string command, char* resp);
        void ReadLine(char* resp, bool print);

        // Setup connection to server
        bool LoginState() {return isLoggedIn;};
    
};

#endif