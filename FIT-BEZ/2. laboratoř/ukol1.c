#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

int inc(char *c)
{
	if (c[0] == 0) {
		return 0;
	}
	
	if (c[0] == 'z') {
		c[0] = 'a';
		return inc(c + sizeof(char));
	}
	
	c[0]++;

	return 1;
}

int main(int argc, char *argv[])
{ 
	int i;

	// Pocet pismen v retezci
	int n = 5;
	int a, b;

	// char text[] = "Text pro hash.";
	char hashFunction[] = "sha1";  // zvolena hashovaci funkce ("sha1", "md5" ...)

	EVP_MD_CTX ctx;  // struktura kontextu
	const EVP_MD *type; // typ pouzite hashovaci funkce
	unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
	int length;  // vysledna delka hashe

	/* Inicializace OpenSSL hash funkci */
	OpenSSL_add_all_digests();
	
	/* Zjisteni, jaka hashovaci funkce ma byt pouzita */
	type = EVP_get_digestbyname(hashFunction);

	/* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
	if ( ! type) {
		printf("Hash %s neexistuje.\n", hashFunction);
		exit(1);
	}

	/* Provedeni hashovani */
	

	char *text = malloc((n + 1) * sizeof(char));
	for (a = 1; a <= n; a++) {
		for (b = 0; b < a; b++) {
			text[b] = 'a';
		}
		text[a] = 0;

		do {
			EVP_DigestInit(&ctx, type);  // nastaveni kontextu
			EVP_DigestUpdate(&ctx, text, n + 1);  // zahashuje text a ulozi do kontextu
			// printf("%d", strlen(text));
			EVP_DigestFinal(&ctx, hash, (unsigned int *) &length);  // ziskani hashe z kontextu

			/* Vypsani vysledneho hashe */
			if (hash[0] == 0xAA && hash[1] == 0xBB) {
			printf("Hash textu \"%s\" je: ", text);
			for (i = 0; i < length; i++) {
				printf("%02x", hash[i]);
			}

			printf("\n");
				
			}
			// printf("%s\n", text);
		} while (inc(text));
	}
	free(text);

	return 0;
}
