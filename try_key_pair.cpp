#include <iostream>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <fstream>
#define KEYLEN 4096
using namespace std;


int main() {
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    char plainText[] = "hello";
    unsigned char encrypte[KEYLEN/8] = {0};
    unsigned char decrypted[KEYLEN/8] = {0};
    BIGNUM *e = BN_new();
    RSA *rsa = RSA_new();
    BIO* public_pem = BIO_new_file("rsa_public.pem", "w");
    BIO* private_pem = BIO_new_file("rsa_private.pem", "w");

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
    
    BIO_free(public_pem);
    BIO_free(private_pem);
    puts("generation completed");

    ifstream read_pem_file;
    string pub_key;
    read_pem_file.open("rsa_public.pem");
    
    string line;
    if(read_pem_file.is_open()){
        printf("Public key read in\n");
        while(getline(read_pem_file, line)){

            pub_key.append(line);
            pub_key.append("\n");
        }
    }
    read_pem_file.close();
    printf("Finish reading public key\n");
    int encrypted_length = RSA_private_encrypt(strlen(plainText),(unsigned char*)plainText, encrypte, rsa, RSA_PKCS1_PADDING);

    RSA* rsa_ = NULL;
    pub_key.push_back('\0');
    cout << pub_key << endl;
    BIO *key = BIO_new_mem_buf(pub_key.c_str(),-1);
    if (key == nullptr){
        perror("reading failes");
    }
    printf("BIO done\n");
    PEM_read_bio_RSAPublicKey(key, &rsa_, NULL, NULL);

    printf("Encrypted length %d\n", encrypted_length);
    int decrypted_length = RSA_public_decrypt(encrypted_length, encrypte, decrypted, rsa_, RSA_PKCS1_PADDING);
    printf("Decrypted length: %d\n",decrypted_length);
    printf("message: %s", decrypted);
    return 0;
}