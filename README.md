# P2P Micropaymet System
## Compiled by L.C.
This project is attempting to build a command line interface supporting P2P micropay.

## How to run the program
1. First, you need to run makefile by 
```code{}
make
```

2. Run client program by
```code{}
./client
```

3. Run server program by
```code{}
./server
```
4. The program runs under the environment of **Ubuntu 18.04**. with *g++* installed.

## System structure
There are two programs, *server.cpp*, *srv.h/srv.cpp* to support the server function.
* *server.cpp* served as the backbone for server function.
* *srv.cpp/.h* are all the specific handling functions' definition and implementation.

Here are some helper functions and data structures.
```{}
void SendMsg(int& sock,const char* msg);
void ReadMsg(int& sock, char* msg, bool print);
void *client(thread_arg* arg);
```

* Send and Read message is used for sending and receiving message from socket.
* The *client* function is used to handle the incoming connection.

Now, we will dive into srv implementation.
```code{c++}
struct User{
    string Name = "", Ip = "", Port = "";
    int balance = 10000;
    bool isOnline;
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
```

*  Using *map* as storing structure is to speed up the time needed for searching, inserting, and deletion. 
* RegisterClient will register clients into the *Users* map data structure. Before registering the client *name*, it will first lookup the existence in *Users*. If it exists, it will return a register fail status message to *client*.
* GoOnline takes the client's name, incoming IP, and port. Update the state of *user* having *name*. If it doesn't exist, it will return an *210 fail* to client.
* GoOffline, it will search for the clients name, and make *isOnline=false*.
* GetClientList, searches for user name and output the string to *stringstream out*. This *out* variable is then converted to character array and sent.

Reference for generating the RSA key pair. https://stackoverflow.com/questions/5927164/how-to-generate-rsa-private-key-using-openssl

Encrypt, decrypt
http://hayageek.com/rsa-encryption-decryption-openssl-c/