/**
 * Author: Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>

using namespace std;

enum CIPHER_METHOD
{
	ECB,
	CBC
};

string getFilename(string file)
{
	return file.substr(0, file.length() - 4);
}

CIPHER_METHOD getCihperMethod(string method)
{
	if (method == "ecb") {
		return ECB;
	} else if (method == "cbc") {
		return CBC;
	} else {
		perror("Cipher method must be: cbc or ecb.");
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	// Test vstupnich dat
	if (argc < 3) {
		cout << "Usage: <binary> method filename" << endl;
		exit(1);
	}

	// Nacteni sifrovaci metody
	CIPHER_METHOD method = getCihperMethod(argv[1]);

	// Nacteni jmena souboru
	string appendix = ".bmp";
	string filename = getFilename(argv[2]);
	string source = filename + appendix;
	string outputSource = filename + "_" + ((method == ECB) ? "ecb" : "cbc") + appendix;

	// Nacteni vstupniho souboru
	ifstream inFile(source, ios::in | ios::binary | ios::ate);

	// Inicializace bufferu
	char *buffer = new char[15];

	// Nastaveni pointeru na zacatek souboru
	inFile.seekg(0, ios::beg);

	// Nacteni dat v hlavicce az po 14 bajt
	inFile.read(buffer, 14);

	// Vypocet velikosti dat v hlavicce
	int headerSize = buffer[13] * 256 * 256 * 256 + buffer[12] * 256 * 256 + buffer[11] * 256 + buffer[10];

	// Uvolneni buffer + alokace na velikost hlavicky
	free(buffer);
	buffer = new char[headerSize];

	// Otevreni vystupniho souboru
	ofstream outFile (outputSource, ofstream::binary);

	// Vlozeni hlavicky do noveho souboru
	inFile.seekg(0, ios::beg);
	inFile.read(buffer, headerSize);
	outFile.write(buffer, headerSize);

	// Nastaveni pointeru na zacatek obrazovych dat
	inFile.seekg(headerSize, ios::beg);

	// Klic pro sifrovani
	unsigned char key[EVP_MAX_KEY_LENGTH] = "Muj klic";

	// Incializaci vektor
	unsigned char initialVector[EVP_MAX_IV_LENGTH] = "vectorcvector";

	// Nacteni OpenSSL
	OpenSSL_add_all_ciphers();

	// Vytvoreni stukruty pro kontext
	EVP_CIPHER_CTX ctx;

	// Nastaveni kontextu pro sifrovani
	EVP_EncryptInit(&ctx, (method == ECB) ? EVP_des_ecb() : EVP_des_cbc(), key, initialVector);

	// Inicializace prostredku pro sifrovani
	unsigned char encryptedBody[2 * sizeof(unsigned char)];
	int encryptedBodyLength = 0;

	// Nacteny znak
	char load;

	// Dokud mam data sifruji
	while (inFile.get(load)) {

		// Nactu jeden byte
		unsigned char letter[2];
		letter[0] = static_cast<unsigned char>(load);

		// Sifrovani
		EVP_EncryptUpdate(&ctx, encryptedBody, &encryptedBodyLength, letter, 1);

		// Ulozeni zasifrovanych dat
		outFile.write((char *)&encryptedBody, encryptedBodyLength);
	}

	// Dokonceni sifrovani - ziskani zbytku z kontextu
	EVP_EncryptFinal(&ctx, encryptedBody, &encryptedBodyLength);

	// Ulozeni zbytku
	outFile.write((char *)&encryptedBody, encryptedBodyLength);

	// Zavreni souboru
	inFile.close();
	outFile.close();

	return 0;
}
