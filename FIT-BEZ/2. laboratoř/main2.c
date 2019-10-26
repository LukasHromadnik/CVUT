/* 
 * File:   main.c
 * Author: stembvac
 *
 * Created on March 5, 2015, 1:02 PM
 */

#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>

int main(int argc, char * argv []) {

    // OT = ot1                     abcdefghijklmnopqrstuvwxyz0123
    // ST k predchozimu OT   = st1  30e8f1220568afb321128ef0b27cb146dca0a9b0e6e37c4ca2c3a238614b
    // ST ktery jsem obdrzel = st2  26efff23057aa9af3c1080fdb660ae59dfa6b4abe1e16351a9d7f568271d


    // nacteni otevreneho textu
    unsigned char ot1[1024];
    strncpy((char *) ot1, argv[1], 30);
    printf("ot1 = %s\n", ot1);

    // nacteni sifroveho textu 1
    unsigned char st1[1024]; // sifrovany text
    strncpy((char *) st1, argv[2], 61);
    printf("st1 = %s\n", st1);

    // nacteni sifroveho textu 2
    unsigned char st2[1024]; // sifrovany text
    strncpy((char *) st2, argv[3], 61);
    printf("st2 = %s\n", st2);

    // prevedeni Sifrovych textu do dec...
    int dec_st1[30];
    int dec_st2[30];
    char dva[4];
    dva[3] = '\0';    
    int i, number;
    char *ptr = '\0';
    
    for (i = 0; i <= 60; i += 2) {
        dva[0] = st1[i];
        dva[1] = st1[i + 1];
        number = (int) strtol(dva, &ptr, 16);
        dec_st1[i / 2] = number;

        dva[0] = st2[i];
        dva[1] = st2[i + 1];
        number = (int) strtol(dva, &ptr, 16);
        dec_st2[i / 2] = number;
    }


    // ============================================================
    // Kontrolni vypisy po prevedeni z HEX do DEC

    printf("\n");
    printf("dec_st1: ");
    for (i = 0; i < 30; i++) {
        printf("%d ", dec_st1[i]);
    }
    printf("\n");
    printf("dec_st2: ");
    for (i = 0; i < 30; i++) {
        printf("%d ", dec_st2[i]);
    }
    printf("\n");


    // ============================================================
    // XORovani
    char ot2[30];
    printf("Tajna zprava: ");
    for (i = 0; i < 30; i++) {
        ot2[i] = (ot1[i] ^ dec_st1[i] ^ dec_st2[i]);
        printf("%c", ot2[i]);
    }
    printf("\n");

    /*
        unsigned char st[1024] = "dkghdhsdhs";
        unsigned char key[EVP_MAX_KEY_LENGTH] = "jhfsadesadkqpwow"; // klic pro sifrovani
        unsigned char iv[EVP_MAX_IV_LENGTH] = "inicial. vektor"; // inicializacni vektor
        const char cipherName[] = "RC4";
        const EVP_CIPHER * cipher;

        OpenSSL_add_all_ciphers();
        // sifry i hashe by se nahraly pomoci OpenSSL_add_all_algorithms() 
        cipher = EVP_get_cipherbyname(cipherName);
        if (!cipher) {
            printf("Sifra %s neexistuje.\n", cipherName);
            exit(1);
        }

        int otLength = strlen((const char*) ot1);
        int stLength = 0;
        int tmpLength = 0;

        EVP_CIPHER_CTX ctx; // struktura pro kontext

        printf("OT: %s\n", ot1);

        // Sifrovani
        EVP_EncryptInit(&ctx, cipher, key, iv); // nastaveni kontextu pro sifrovani
        EVP_EncryptUpdate(&ctx, st, &tmpLength, ot1, otLength); // sifrovani ot
        stLength += tmpLength;
        EVP_EncryptFinal(&ctx, st + stLength, &tmpLength); // dokonceni (ziskani zbytku z kontextu)
        stLength += tmpLength;

        printf("Zasifrovano %d znaku.\n", stLength);

        // Desifrovani 
        EVP_DecryptInit(&ctx, cipher, key, iv); // nastaveni kontextu pro desifrovani
        EVP_DecryptUpdate(&ctx, ot1, &tmpLength, st, stLength); // desifrovani st
        otLength += tmpLength;
        EVP_DecryptFinal(&ctx, ot1 + otLength, &tmpLength); // dokonceni (ziskani zbytku z kontextu)
        otLength += tmpLength;

        // Vypsani zasifrovaneho a rozsifrovaneho textu.
        printf("ST: ");
        for (i = 0; i < stLength; i++) {
            printf("%02x", st[i]);
        }
        printf("\nDT: %s\n", ot1);
     */

    exit(0);
}