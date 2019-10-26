#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // htons(), htonl()
#include <strings.h> // bzero()
#include <string.h>
#include <netdb.h>

#include <iostream>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

#define BUFFER_SIZE 1

using namespace std;

int main(int argc, char const *argv[])
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		perror("Nemohu vytvorit socket!");
		return -1;
	}

	socklen_t sockAddrSize;
	struct sockaddr_in serverAddr;
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero((char *) &serverAddr, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(443);
	struct hostent *host;
	host = gethostbyname(string("fit.cvut.cz").c_str()); // <netdb.h>
	memcpy(&serverAddr.sin_addr, host->h_addr, host->h_length); // <string.h>

	// Pripojeni ke vzdalenemu serveru
	if (connect(s, (struct sockaddr *) &serverAddr, sockAddrSize) < 0) {
		perror("Nemohu spojit!");
		close(s);
		return -1;
	}

	SSL_library_init();

	SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());

	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1);

	SSL *ssl = SSL_new(ctx);
	SSL_set_fd(ssl, s);

	SSL_connect(ssl);

	X509 *certificate = SSL_get_peer_certificate(ssl);
	FILE *fileWithCertificate = fopen("cert.pem", "w+");
	PEM_write_X509(fileWithCertificate, certificate);
	BIO *o = BIO_new_fp(stdout, BIO_NOCLOSE);
	X509_print_ex(o, certificate, XN_FLAG_COMPAT, X509_FLAG_COMPAT);

	string request = "GET /student/odkazy HTTP/1.0\r\n\r\n";
	SSL_write(ssl, request.c_str(), request.length());

	int bytesRead = 1;

	unsigned char buffer[BUFFER_SIZE + 1];

	FILE *htmlPage = fopen("index.html", "w+");
	int headerState = 0;
	bool endOfHeader = false;
	
	while (bytesRead > 0) {
		bytesRead = SSL_read(ssl, &buffer, BUFFER_SIZE);
		buffer[bytesRead] = '\0';

		if (buffer[0] == '\r' && headerState == 0) {
			headerState = 1;
		} else if (buffer[0] == '\n' && headerState == 1) {
			endOfHeader = true;
		} else if (buffer[0] == '\r' && headerState == 1) {
		
		} else {
			headerState = 0;
		}

		// cout << buffer << endl;
		if (endOfHeader) {
			fwrite(buffer, sizeof(unsigned char), bytesRead, htmlPage);
		}
	}

	SSL_shutdown(ssl);

	close(s);

	SSL_free(ssl);
	SSL_CTX_free(ctx);
	
	return 0;
}

