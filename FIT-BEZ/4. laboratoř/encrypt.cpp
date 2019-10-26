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
        printf("Usage echo \"input\" | %s pubkey.pem output.bin\n", argv[0]);
        return 1;
    }

    string lineInput;
    string temp;
    while (cin >> temp) {
        //cout << temp << " ";
        lineInput += temp;
    }
    //cout << endl;
    //cout << lineInput << endl;


    int len_out;
    unsigned char buffer_out[4096];
    const char * message = lineInput.c_str();
    int len = strlen(message);
    uint32_t eklen_n;
    
    EVP_CIPHER_CTX ctx;
    FILE * file;
    FILE * g_data;
    //otevreni souboru
    file = fopen(argv[1],"rb");
    g_data = fopen(argv[2],"wb");
    
    if(g_data == NULL){
        cout<<"cannot open file with message"<<endl;
        exit (1);
    }
    if(file == NULL){
        cout<<"cannot open file with public key"<<endl;
    }
    //klice
    EVP_PKEY *pkey = EVP_PKEY_new();
    RSA *rsa_pkey = NULL;
    
    //nacteni klice
    if(!PEM_read_RSA_PUBKEY(file, &rsa_pkey, NULL, NULL)){
        cout<<"error while reading key"<<endl;
        exit (1);
    }
    if(!EVP_PKEY_assign_RSA(pkey, rsa_pkey)){
        cout<<"error while assigning key"<<endl;
        exit (1);
    }
    
    unsigned char * my_ek = (unsigned char *) malloc(EVP_PKEY_size(pkey)); // allocate space for encrypted symmet. key
    int my_ekl; // enc. sym. key length
    unsigned char iv[EVP_MAX_IV_LENGTH] = "vectorvector"; // buffer for the init. vector
    
    //inicializace kontextu
    EVP_CIPHER_CTX_init(&ctx);
    
    //INIT
    if(!EVP_SealInit(&ctx, EVP_aes_128_cbc(), &my_ek, &my_ekl, iv, &pkey, 1)){
        cout<<"seal init failed"<<endl;
        exit (1);
    }
    
    //The htonl() function converts the "unsigned integer hostlong" from host byte order to network byte order.
    eklen_n = htonl(my_ekl);
    //zapis delku klice
    if(fwrite(&eklen_n, sizeof eklen_n, 1, g_data)!=1){
        cout<<"cannot write"<<endl;
        exit(1);
    }
    //zapis klic 
    if(fwrite(my_ek, my_ekl, 1, g_data)!=1){
        cout<<"cannot write"<<endl;
        exit (1);
    }
    
    //zapis IV
    //EVP_CIPHER_iv_length() and EVP_CIPHER_CTX_iv_length() 
    //return the IV length of a cipher when passed an EVP_CIPHER or EVP_CIPHER_CTX .
    // It will return zero if the cipher does not use an IV . 
    //The constant EVP_MAX_IV_LENGTH is the maximum IV length for all ciphers.
    if(fwrite(iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc()), 1, g_data)!=1){
        cout<<"cannot write"<<endl;
        exit(1);
    }

    //zasifrovani + zapis do souboru
    //EVP_SealUpdate() and EVP_SealFinal() return 1 for success and 0 for failure.
    //EVP_EncryptUpdate() encrypts inl bytes from the buffer in and writes the encrypted version to out. This function can be called multiple times to encrypt successive blocks of data. The amount of data written depends on the block alignment of the encrypted data: as a result the amount of data written may be anything from zero bytes to (inl + cipher_block_size - 1) so out should contain sufficient room. 
    //The actual number of bytes written is placed in outl.
    if(!EVP_SealUpdate(&ctx, buffer_out, &len_out, (const unsigned char *)message, len)){
        cout<<"seal update failed"<<endl;
        exit(1);
    }
    if(len_out > 0){
        if(fwrite(buffer_out, len_out, 1, g_data)!=1){
            cout<<"cannot write"<<endl;
            exit(1);
        }
    }

    //final + zapis
    if(!EVP_SealFinal(&ctx, buffer_out, &len_out)){
        cout<<"seal final failed"<<endl;
        exit(1);
    }
    if(fwrite(buffer_out, len_out, 1, g_data)!=1){
        cout<<"cannot write"<<endl;
        exit(1);
    }
    
    EVP_PKEY_free(pkey);
    free(my_ek);
    fclose(g_data);
    return 0;
}