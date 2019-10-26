#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include <arpa/inet.h>
using namespace std;


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage %s privkey.pem input.bin\n", argv[0]);
        return 1;
    }

    RSA *rsa_pkey = NULL;
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_CIPHER_CTX ctx;
    unsigned char buffer[4096] = {0};
    unsigned char buffer_out[4096 + EVP_MAX_IV_LENGTH] = {0};
    size_t len;
    int len_out;
    unsigned char *ek;
    unsigned int eklen;
    unsigned char iv[EVP_MAX_IV_LENGTH];
    FILE * file;
    FILE * g_data;
    file = fopen(argv[1],"rb");
    g_data = fopen(argv[2],"rb");
   
    if(g_data == NULL){
        cout<<"cannot open file with message"<<endl;
        exit (1);
    }
    if(file == NULL){
        cout<<"cannot open file with private key"<<endl;
    }
    
    //nacteni klice
    if(!PEM_read_RSAPrivateKey(file, &rsa_pkey, NULL, NULL)){
        cout<<"error while reading key"<<endl;
        exit (1);
    }
    if(!EVP_PKEY_assign_RSA(pkey, rsa_pkey)){
        cout<<"error while assgning key"<<endl;
        exit (1);
    }
    
    uint32_t eklen_n;
    EVP_CIPHER_CTX_init(&ctx);
    ek = (unsigned char *)malloc(EVP_PKEY_size(pkey));

    //nacte delku klice
    if(fread(&eklen_n, sizeof eklen_n, 1, g_data)!=1){
        cout<<"cannot read"<<endl;
        exit(1);
    }

    
    eklen = ntohl(eklen_n);
    //zkontroluje delku klice
    if (eklen > EVP_PKEY_size(pkey))
    {
        cout<<"wrong key length"<<endl;
        exit(1);
    }
    
    //nacte klic
    if(fread(ek, eklen, 1, g_data)!=1){
        cout<<"cannot read"<<endl;
        exit(1);
    }
    
    //nacte IV
    if(fread(iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc()), 1, g_data)!=1){
        cout<<"cannot read"<<endl;
        exit(1);
    }

    //init
    if(!EVP_OpenInit(&ctx, EVP_aes_128_cbc(), ek, eklen, iv, pkey)){
        cout<<"open init failed"<<endl;
        exit(1);
    }
    
    //nacte zpravu
    len = fread(buffer, 1, sizeof(buffer), g_data);
    //desifruje zpravu
    int totalLen=0;
    
    if(!EVP_OpenUpdate(&ctx, buffer_out, &len_out, buffer, len)){
        cout<<"open update failed"<<endl;
        exit(1);
    }
        
    totalLen+=len_out;
    //final
    if(!EVP_OpenFinal(&ctx, &buffer_out[totalLen], &len_out)){
        cout<<"open update failed"<<endl;
        exit(1);
    }
    
    totalLen+=len_out;
    buffer_out[totalLen]='\0';
    
    EVP_PKEY_free(pkey);
    free(ek);
    
    fclose(g_data);
    
    string output((const char *)buffer_out);
    cout << output << endl;
    return 0;
}