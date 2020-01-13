
#include "connectAPI.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <mutex>

mutex mt;
#define KEYLEN 4096
using namespace std;

void Client::SendCommand(string command, char* resp){
    write(sock, command.c_str(), command.length());
    return;
}


void Client::ReadLine(char* resp, bool print){
    memset(resp, 0, 1024);
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);
    /* Wait up to five seconds. */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    retval = select(sock+1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if (retval)
        read(sock, resp, 1024);
        /* FD_ISSET(0, &rfds) will be true. */
    if (print)
        printf("%s", resp);
    return ;
}

void Client::connect2Server(){
    struct sockaddr_in srv; 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    { 
        printf("Socket creation error \n"); 
        return ;
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(stoi(port));
    srv.sin_addr.s_addr = inet_addr(Ip.c_str());
    // Convert IPv4 and IPv6 addresses from text to binary form 

    if (connect(sock , (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        printf("Socket creation error, please config \n"); 
        return ;
    }

    char buf[100] = {0};
    read(sock, buf, 100);
    printf("%s\n", buf);
}

void Client::Terminate(){close(sock);}

void Client::Register(string Username){
    string command = "REGISTER#";
    command.append(Username);
    SendCommand(command, rep);
    ReadLine(rep, false);
    string response(rep);
    if (response.find("210 fail") != string::npos){
        cout << "ERROR: Register fail, Please try again!" << endl;
    }
    else if (response.find("100 ") != string::npos){
        cout << "Registered!" << endl;
    }
}


void Client::UpdateConfig(string IP, string PORT) {Ip = IP; port = PORT;}

void Client::GrabOnlineList(){
    string list("List");
    SendCommand(list, rep);
    cout << "Balance: ";
    ReadLine(rep, true);
    return;
}

void Client::GenerateKey(string User){
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    char plainText[] = "hello";
    unsigned char encrypte[KEYLEN/8] = {0};
    unsigned char decrypted[KEYLEN/8] = {0};
    BIGNUM *e = BN_new();
    RSA *rsa = RSA_new();
    BIO* public_pem = BIO_new_file(User.append("rsa_public.pem").c_str(), "w");
    BIO* private_pem = BIO_new_file(User.append("rsa_private.pem").c_str(), "w");

    if(BN_set_word(e, RSA_F4) == 0){
        perror("check failed on BN set.");
    }

    RSA_generate_key_ex(rsa, KEYLEN, e, NULL);
    EVP_PKEY* pkey(EVP_PKEY_new()); 
    if(EVP_PKEY_set1_RSA(pkey, rsa) == 0){
        perror("Conversion From RSA to PKEY fails.");
    }

    if(PEM_write_bio_RSAPublicKey(public_pem, rsa) == 0){
        perror("Write public key fails");
    }
    if(PEM_write_bio_RSAPrivateKey(private_pem, rsa, NULL, NULL, 0, NULL, NULL) == 0){
        perror("Write private key fails");
    }
    own_rsa = rsa;
    BIO_free(public_pem);
    BIO_free(private_pem);
    puts("generation completed");
}

string Client::ReadPublicKey(string User){
    
    ifstream read_pem_file;
    string pub_key;
    read_pem_file.open(User.append("rsa_public.pem").c_str());
    
    string line;
    if(read_pem_file.is_open()){
        printf("Public key read in\n");
        while(getline(read_pem_file, line)){

            pub_key.append(line);
            pub_key.append("\n");
        }
    }
    return pub_key;
}

int Encrypt_message(unsigned char* msg, int string_length, unsigned char* to, RSA* own_rsa){
    printf("msg to encrypt: %s", (char*)msg);
    int encrypted_length = RSA_private_encrypt(string_length , msg, to, own_rsa, RSA_PKCS1_PADDING);
    printf("Encrypted length %d\n", encrypted_length);
    // printf("Encrypted data: %s\n", (char*) to);
    return encrypted_length;
}

void Client::Login(string User, string port){
    string command("LOGIN");
    command.append(User.c_str());
    command.append("#");
    command.append(port);
    hosting_port = port;
    SendCommand(command, rep);
    ReadLine(rep, false);
    string response(rep);
    // message_thread = new thread(ChatServer, (void*)port.c_str());
    if (response.find("220") != string::npos){
        cout << "Login Fail, Please try again" << endl;
        return;
    }
    else{
        isLoggedIn = true;
        printf("Balance: ");
        ReadLine(rep, true);

        cout << "#######  Key Exchange and Encryption testing...  ########" << endl;
        // Sends server the public key once it logs in.
        GenerateKey(User);
        ChatServerArg *args = new ChatServerArg;
        args->hosting_port = hosting_port;
        args->Ip = Ip;
        args->port2connect = this->port;
        args->whoami = User;
        args->own_key = own_rsa;
        message_thread = new thread(ChatServer, args);



        string pub_key = ReadPublicKey(User);
        string test_message = "Testing message showing success\n";
        unsigned char encrypted[KEYLEN/8]  = {0};
        int enc_length = Encrypt_message((unsigned char* )(test_message.c_str()), test_message.length(), encrypted, own_rsa);
        SendCommand(pub_key, rep);

        SendCommand(to_string(enc_length), rep);
        ReadLine(rep, false);
        write(sock, encrypted, enc_length);
        cout << "#######  Key Exchange and Encryption testing...  ########" << endl;
    }
}

void Client::StartChatServer(string IP, string PORT, string amount){
    int sock;
    SetupConnection(sock,IP, PORT );
    // Encrypt_message
}

void Client::ServerLocation(){
    cout << "Original Ip: " << Ip << " port: " << port << endl;
}

void Client::GoOffline(){
    string exit("Exit");
    SendCommand(exit, rep);
    ReadLine(rep, false);
    string resp(rep);
    if (resp.find("Bye") != string::npos){
        cout << "Disconnected!" << endl;
        return;
    }
}

void SetupConnection(int& sock,string Ip, string port){
    struct sockaddr_in srv; 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    { 
        printf("Socket creation error \n"); 
        return ;
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(stoi(port));
    srv.sin_addr.s_addr = inet_addr(Ip.c_str());
    // Convert IPv4 and IPv6 addresses from text to binary form 

    if (connect(sock , (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        printf("Socket creation error, please config \n"); 
        return ;
    }
    return ;
}

void * ChatServer(void* pt){
    ChatServerArg* args = (ChatServerArg*)pt;
    string port = args->hosting_port;
    cout << "hosting port:" << port << endl;
    cout << "server port:" << args->port2connect << " IP:" << args->Ip << endl;
    int fd, newfd, opt=1;

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
    srv.sin_port = htons(stoi(port));
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind()
    if (bind(fd, (struct sockaddr *)&srv, sizeof(srv)) < 0){
        perror("bind"); exit(1);
    }

    // listen()
    if (listen(fd, 3) < 0){
        perror("listen");
        exit(1);
    }
    unsigned cli_len = sizeof(cli);
    char msg[512] = {0};
    while(1){
        //accept()
        newfd = accept(fd, (struct sockaddr *)&cli, &cli_len);
        int sock2server;
        SetupConnection(sock2server, args->Ip,args->port2connect);
        cout << "[INCOMING]: accepted new msg" << endl;
        read(newfd, msg, 512);
        string incoming_msg(msg);

        cout << "[INCOMING]: SENDER: " << msg << endl;
        for (int i=0; msg[i] != 0 ; i ++){
            if (msg[i] == '\n'){
                msg[i] = 0;
                break;
            }    
        }
        incoming_msg.insert(0, "TRANSFER_PAYMENT");
        strcpy(msg, incoming_msg.c_str());
        strcat(msg, "#");
        strcat(msg, args->whoami.c_str());
        write(sock2server, msg, 100);
        memset(msg, 0, 512);
        read(newfd, msg, 512);
        unsigned char encrypted[KEYLEN / 8] = {0};
        int enc_length = Encrypt_message((unsigned char *)msg, strlen(msg), encrypted, args->own_key);
        write(sock2server,(char*)encrypted, enc_length);

        // Creates Thread arg
        // Read from newfd
        // Initiate a new chat with server and send to it.
        // Send to server
    }
}