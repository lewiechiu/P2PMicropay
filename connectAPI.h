#ifndef CONNECTAPI_H
#define CONNECTAPI_H

#include <string>
#include <thread>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
using namespace std;

void* ChatServer(void* pt);
void init_openssl();
void cleanup_openssl();
void SetupConnection(int& sock, string, string);
int Encrypt_message(unsigned char* , int , unsigned char*, RSA*);
struct ChatServerArg
{
    string  hosting_port; // To start on and listen
    string  port2connect; // Port used to connect to server;
    string  Ip;           // To server
    string  whoami;       // Used to tell server who are you
    RSA*    own_key;      //Used to encrypt incoming message
};

class Client{
    private:
        int sock;
        char rep[1024] = {0};
        string port;
        string hosting_port;
        string Ip;
        bool isEncrypte=false;
        bool isLoggedIn = false;
        thread *message_thread;
        RSA* own_rsa;

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
        void StartChatServer(string IP, string PORT, string amount);
        void GenerateKey(string User);
        void SendCommand(string command, char* resp);
        void ReadLine(char* resp, bool print);
        string ReadPublicKey(string User);
        
        // Setup connection to server
        bool LoginState() {return isLoggedIn;};
    
};

#endif