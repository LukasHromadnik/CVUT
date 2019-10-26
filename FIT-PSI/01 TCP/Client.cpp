//============================================================================
// Name        : Client.cpp
// Author      : Vikturek
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // htons(), htonl()
#include <strings.h> // bzero()
#include <sys/types.h>
#include <netdb.h>
#include <string.h>

#include <stdio.h>

using namespace std;

#define BUFFER_SIZE 10240

int main(int argc, char **argv)
{
	if (argc < 3) {
		cerr << "Usage: client address port" << endl;
		return -1;
	}

	int port = atoi(argv[2]);

	// Vytvoreni koncoveho bodu spojeni
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
	serverAddr.sin_port = htons(port);
	struct hostent *host;
	host = gethostbyname(argv[1]); // <netdb.h>
	memcpy(&serverAddr.sin_addr, host->h_addr,
	host->h_length); // <string.h>

	// Pripojeni ke vzdalenemu serveru
	if (connect(s, (struct sockaddr *) &serverAddr, sockAddrSize) < 0) {
		perror("Nemohu spojit!");
		close(s);
		return -1;
	}

	char login[BUFFER_SIZE] = "Robot345\r\n";
	char password[BUFFER_SIZE] = "674\r\n";

	int i = 0;


	while (true) {
		char buffer[BUFFER_SIZE];
		int bytesRead = recv(s, buffer, BUFFER_SIZE, 0);
		buffer[bytesRead] = '\0';
		cout << buffer;

		if (i == 0) {
			i++;
			cout << "> " << login;
			send(s, login, strlen(login), 0);
			continue;
		} else if (i == 1) {
			i++;
			cout << "> " << password;
			send(s, password, strlen(password), 0);
			continue;
		} else if (i == 2) {
			char input[BUFFER_SIZE] = "INFO \r \n\r \n \r\nINFO \r\nX";
			i++;
			cout << "> " << input;
			send(s, input, strlen(input), 0);
			continue;
		}

		cout << "> ";
		cin.getline(buffer, BUFFER_SIZE);
		cout << "Sending: " << "\"" << buffer << "\"" << endl;

		buffer[strlen(buffer)] = '\r';
		buffer[strlen(buffer)] = '\n';
		buffer[strlen(buffer)] = '\0';

		if (send(s, buffer, strlen(buffer), 0) < 0) {
			perror("Nemohu odeslat data!");
			close(s);
			return -3;
		}
	}

	close(s);
	return 0;
}
