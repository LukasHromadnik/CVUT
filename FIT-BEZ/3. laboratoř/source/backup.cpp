#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        cout << "Usage: <binary> filename" << endl;
        exit(1);
    }

    
    string arg = argv[1];
//    string arg = "/Users/Luke/Desktop/lena512.bmp";
    
    arg = arg.substr(0, arg.length() - 4);
    char *dup = strdup(arg.c_str());
    char *filename = strtok(dup, ".");
    // cout << filename << endl;
    
    char source[100], outECB[100], outECBDec[100], outCBC[100], outCBCDec[100];
    sprintf(source, "%s.bmp", filename);
    // cout << source << endl;
    sprintf(outECB, "%s_ecb.bmp", filename);
    sprintf(outECBDec, "%s_ecb_dec.bmp", filename);
    sprintf(outCBC, "%s_cbc.bmp", filename);
    sprintf(outCBCDec, "%s_cbc_dec.bmp", filename);
    
    unsigned char buffer[15];
    
    FILE *inputFile = fopen(source, "rb");
    
    fread(buffer, sizeof(char), 15, inputFile);
    
    // Vypocet velikosti hlavicky
    int headerSize = buffer[13] * 256 * 256 * 256 + buffer[12] * 256 * 256 + buffer[11] * 256 + buffer[10];
    // cout << headerSize << endl;
    
    fseek(inputFile, 0L, SEEK_END);
    long int bodySize = ftell(inputFile) - headerSize;
    // cout << bodySize << endl;
    
    if (bodySize <= 0) {
        cout << "File is corrupted." << endl;
        exit(1);
    }
    
    rewind(inputFile);
    
    unsigned char *header = new unsigned char[headerSize];
    fread(header, headerSize, 1, inputFile);
    
    // cout << (int)header[10] << endl;
    
    unsigned char *body = new unsigned char[bodySize];
    fread(body, sizeof(unsigned char), bodySize, inputFile);
    
    fclose(inputFile);
    
    unsigned char key[EVP_MAX_KEY_LENGTH] = "Muj klic";  // klic pro sifrovani
    unsigned char iv[EVP_MAX_IV_LENGTH] = "vectorcvector";  // inicializacni vektor
    
    OpenSSL_add_all_ciphers();
    
    int bodyEncryptECBLength = 0;
    int bodyDecryptECBLength = 0;
    int tmpLength = 0;
    
    EVP_CIPHER_CTX ctx; // struktura pro kontext
    
    // ==========
    // ECB
    // ==========
    
    int bodyEncryptECBSize = bodySize * 1.1;
    unsigned char *bodyEncryptECB = new unsigned char[bodyEncryptECBSize];
    
    // Sifrovani
    EVP_EncryptInit(&ctx, EVP_des_ecb(), key, iv);  // nastaveni kontextu pro sifrovani
    EVP_EncryptUpdate(&ctx, bodyEncryptECB, &tmpLength, body, bodySize);  // sifrovani ot
    bodyEncryptECBLength += tmpLength;
    EVP_EncryptFinal(&ctx, bodyEncryptECB + bodyEncryptECBLength, &tmpLength);  // dokonceni (ziskani zbytku z kontextu)
    bodyEncryptECBLength += tmpLength;
    
    FILE *outputEncryptECBFile = fopen(outECB, "wb");
    fwrite(header, headerSize, 1, outputEncryptECBFile);
    fwrite(bodyEncryptECB, bodyEncryptECBLength, 1, outputEncryptECBFile);
    fclose(outputEncryptECBFile);
    
    // Desifrovani
    unsigned char *bodyDecryptECB = new unsigned char[bodyEncryptECBSize];
    
    EVP_DecryptInit(&ctx, EVP_des_ecb(), key, iv);  // nastaveni kontextu pro desifrovani
    EVP_DecryptUpdate(&ctx, bodyDecryptECB, &tmpLength, bodyEncryptECB, bodyEncryptECBLength);  // desifrovani st
    bodyDecryptECBLength += tmpLength;
    EVP_DecryptFinal(&ctx, bodyDecryptECB + bodyDecryptECBLength, &tmpLength);  // dokonceni (ziskani zbytku z kontextu)
    bodyDecryptECBLength += tmpLength;
    
    FILE *outputDecryptECBFile = fopen(outECBDec, "wb");
    fwrite(header, headerSize, 1, outputDecryptECBFile);
    fwrite(bodyDecryptECB, bodyDecryptECBLength, 1, outputDecryptECBFile);
    fclose(outputDecryptECBFile);
    
    // ==========
    // CBC
    // ==========
    
    int bodyEncryptCBCLength = 0;
    int bodyDecryptCBCLength = 0;
    
    int bodyEncryptCBCSize = bodySize * 1.1;
    unsigned char *bodyEncryptCBC = new unsigned char[bodyEncryptCBCSize];
    
    // Sifrovani
    EVP_EncryptInit(&ctx, EVP_des_cbc(), key, iv);  // nastaveni kontextu pro sifrovani
    EVP_EncryptUpdate(&ctx, bodyEncryptCBC, &tmpLength, body, bodySize);  // sifrovani ot
    bodyEncryptCBCLength += tmpLength;
    EVP_EncryptFinal(&ctx, bodyEncryptCBC + bodyEncryptECBLength, &tmpLength);  // dokonceni (ziskani zbytku z kontextu)
    bodyEncryptCBCLength += tmpLength;
    
    FILE *outputEncryptCBCFile = fopen(outCBC, "wb");
    fwrite(header, headerSize, 1, outputEncryptCBCFile);
    fwrite(bodyEncryptCBC, bodyEncryptCBCLength, 1, outputEncryptCBCFile);
    fclose(outputEncryptCBCFile);
    
    // Desifrovani
    unsigned char *bodyDecryptCBC = new unsigned char[bodyEncryptCBCSize];
    
    EVP_DecryptInit(&ctx, EVP_des_cbc(), key, iv);  // nastaveni kontextu pro desifrovani
    EVP_DecryptUpdate(&ctx, bodyDecryptCBC, &tmpLength, bodyEncryptCBC, bodyEncryptCBCLength);  // desifrovani st
    bodyDecryptCBCLength += tmpLength;
    EVP_DecryptFinal(&ctx, bodyDecryptCBC + bodyDecryptCBCLength, &tmpLength);  // dokonceni (ziskani zbytku z kontextu)
    bodyDecryptCBCLength += tmpLength;
    
    FILE *outputDecryptCBCFile = fopen(outCBCDec, "wb");
    fwrite(header, headerSize, 1, outputDecryptCBCFile);
    fwrite(bodyDecryptCBC, bodyDecryptCBCLength, 1, outputDecryptCBCFile);
    fclose(outputDecryptCBCFile);
    
    return 0;
}
