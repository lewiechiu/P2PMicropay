#ifndef SRV_H
#define SRV_H


#include <string>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
using namespace std;

enum status {SUCCESS, EXIST, UNDEFINED, ERROR};
enum Mode {REGISTER, LOGIN, QUIT, GET, PARSE_ERROR};
enum printing {FULL, LIST, FUNCTION};
// Server returns one of the above.


struct User{
    string Name = "", Ip = "", Port = "";
    int balance = 10000;
    bool isOnline;
    char* pub_key;
};

struct thread_arg{
    int sock;
    string ip;
};

class Server {
    private:
        map<string, User*> Users;
    public:
        status RegisterClient(string name);
        status GoOnline(string name, string port, string ip);
        status GoOffline(string name);
        void GetClientList(string name, stringstream &out);
        
};

void SendMsg(int& sock,const char* msg);
void ReadMsg(int& sock, char* msg, bool print);
void Decrypt_message(string user, unsigned char * enc_data,int data_len,RSA* rsa, unsigned char *decrypted);
void ReadKeyByUser(string name, RSA* rsa);
Mode parsing(string cmd);

void * client(thread_arg* arg);

#endif