//============================================================================
// Name        : Server.cpp
// Author      : Lukáš Hromadník
// Version     : 2.1
// Description : New buffer, fix test #12
//============================================================================

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // htons(), htonl()
#include <strings.h> // bzero()
#include <string.h>

#include <vector>
#include <deque>
#include <sstream>

using namespace std;

#define TIMEOUT 5
#define BUFFER_SIZE 10240

// =================================================================
// =================================================================

class Buffer
{
public:
	Buffer(int socket) : m_Socket(socket) {}
	unsigned char getByte();
	int socket() { return m_Socket; }

private:
	int loadData();

	deque<unsigned char> m_Data;
	int m_Socket;
};

unsigned char Buffer::getByte()
{
	if (m_Data.empty()) {
		int returnValue = this->loadData();

		if (returnValue == 1) {
			return '\0';
		}
	}
	
	unsigned char result = m_Data.front();

	m_Data.pop_front();

	return result;
}

int Buffer::loadData()
{
	ssize_t bytesRead = 1;
	int returnValue = 1;
	int recieveSuccess = 0;

	unsigned char result[BUFFER_SIZE];

	do {
		bytesRead = recv(m_Socket, result, sizeof(result), 0);
		
		if (bytesRead > 0) {
			if ( ! recieveSuccess) {
				returnValue = 0;
				recieveSuccess = 1;	
			}

			for (int i = 0; i < bytesRead; i++) {
				unsigned char letter = result[i];
				m_Data.push_back(letter);
			}
		}

		// if (bytesRead < 120 && bytesRead > 110) {
		// 	printf("%s\n", result);
		// }

		if (bytesRead < BUFFER_SIZE) {
			break;
		}

	} while (bytesRead > 0);

	return returnValue;
}

// =================================================================
// =================================================================

void send(int socket, string message)
{
	if (send(socket, message.c_str(), message.length(), 0) == -1) {
		perror("Chyba pri zapisu");
		close(socket);
		exit(1);
	}
}

void sendMessage(int code, int socket)
{
	string message;
	
	switch (code) {
		case 200:
			message = "200 LOGIN\r\n";
			break;
		case 201:
			message = "201 PASSWORD\r\n";
			break;
		case 202:
			message = "202 OK\r\n";
			break;
		case 300:
			message = "300 BAD CHECKSUM\r\n";
			break;
		case 500:
			message = "500 LOGIN FAILED\r\n";
			break;
		case 501:
			message = "501 SYNTAX ERROR\r\n";
			break;
		case 502:
			message = "502 TIMEOUT\r\n";
			break;
	}

	send(socket, message);
}

vector<unsigned char> loadMessageInVector(Buffer *buffer)
{
	vector<unsigned char> message;
	int state = 0;

	while (state != 2) {
		unsigned char letter = buffer->getByte();

			message.push_back(letter);
		// if (letter != '\r' && letter != '\n') {
		// }

		if (letter == '\r' && state == 0) {
			state = 1;
		} else if (letter == '\r' && state == 1) {

		} else if (letter == '\n' && state == 1) {
			state = 2;
		} else {
			state = 0;
		}
	}

	message.pop_back();
	message.pop_back();

	return message;
}

string loadMessage(Buffer *buffer)
{
	vector<unsigned char> result = loadMessageInVector(buffer);

	return string(result.begin(), result.end());
}

int makeLogin(Buffer *buffer)
{
	int socket = buffer->socket();

	sendMessage(200, socket);
	vector<unsigned char> login = loadMessageInVector(buffer);
	string loginString = string(login.begin(), login.end());
	sendMessage(201, socket);
	string password = loadMessage(buffer);

	string loginPrefix = loginString.substr(0, 5);
	if (loginPrefix != "Robot") {
		return 1;
	}

	int checksum = 0;
	for (int i = 0; i < (int) login.size(); i++) {
		checksum += (int)login[i];
		// checksum += login[i];
	}

	stringstream ss;
	ss << checksum;
	string checksumString = ss.str();

	if (checksumString != password) {
		return 1;
	}

	return 0;
}

int loadInfo(Buffer *buffer)
{
	string loaded = loadMessage(buffer);
	string message = 'I' + loaded;

	if (message.substr(0, 5) != "INFO ") {
		return 1;
	}

	return 0;
}

int loadPhoto(Buffer *buffer)
{
	unsigned char photoPrefix[4] = { 'O', 'T', 'O', ' ' };

	for (int i = 0; i < 4; i++) {
		unsigned char letter = buffer->getByte();
		if (letter != photoPrefix[i]) {
			return 1;
		}
	}

	stringstream ss;
	while (true) {
		unsigned char letter = buffer->getByte();
		if (letter == ' ') {
			break;
		}
		if (letter != '0' && letter != '1' && letter != '2' && letter != '3' && letter != '4' && letter != '5' && letter != '6' && letter != '7' && letter != '8' && letter != '9') {
			return 1;
		}
		ss << letter;
	}

	int dataCount = atoi(ss.str().c_str());
	
	vector<unsigned char> image;
	long imageChecksum = 0;
	for (int i = 0; i < dataCount; i++) {
		unsigned char letter = buffer->getByte();
		image.push_back(letter);
		imageChecksum += (int)letter;
	}

	vector<unsigned char> checksumVector;
	for (int i = 0; i < 4; i++) {
		checksumVector.push_back(buffer->getByte());
	}

	long checksum = checksumVector[0] * 256 * 256 * 256 +
					checksumVector[1] * 256 * 256 +
					checksumVector[2] * 256 +
					checksumVector[3];

	if (checksum != imageChecksum) {
		return 2;
	}

	return 0;
}

// =================================================================
// =================================================================

int main (int argc, char **argv)
{
	if (argc < 2) {
		cerr << "Usage: server port" << endl;
		return -1;
	}

	// Vytvoreni koncoveho bodu spojeni
	int l = socket(AF_INET, SOCK_STREAM, 0);
	if (l < 0) {
		perror("Nemohu vytvorit socket!");
		return -1;
	}

	int port = atoi(argv[1]);
	if (port == 0) {
		cerr << "Usage: server port" << endl;
		close(l);
		return -1;
	} else if (port < 3000 || port > 3999) {
		cerr << "Port must be in range from 3000 to 3999" << endl;
		close(l);
		return -1;
	}

	struct sockaddr_in adresa;
	bzero(&adresa, sizeof(adresa));
	adresa.sin_family = AF_INET;
	adresa.sin_port = htons(port);
	adresa.sin_addr.s_addr = htonl(INADDR_ANY);

	// Prirazeni socketu k rozhranim
	if (bind(l, (struct sockaddr *) &adresa, sizeof(adresa)) < 0) {
		perror("Problem s bind()!");
		close(l);
		return -1;
	}

	// Oznacim socket jako pasivni
	if (listen(l, 10) < 0) {
		perror("Problem s listen()!");
		close(l);
		return -1;
	}

	struct sockaddr_in vzdalena_adresa;
	socklen_t velikost;

	enum ApplicationState { LOGIN, INFO };

	while (true) {
		
		// Cekam na prichozi spojeni
		int socket = accept(l, (struct sockaddr *) &vzdalena_adresa, &velikost);
		if (socket < 0) {
			perror("Problem s accept()!");
			close(l);
			return -1;
		} else {

			pid_t pid = fork();

			if (pid == 0) {
				struct timeval timeout;
				timeout.tv_usec = 0;
				timeout.tv_sec = TIMEOUT;
				fd_set sockets;
				int retval;

				ApplicationState application = LOGIN;
				Buffer *buffer = new Buffer(socket);

				while (true) {
					FD_ZERO(&sockets);
					FD_SET(socket, &sockets);
					retval = select(socket + 1, &sockets, &sockets, NULL, &timeout);

					if (retval < 0) {
						perror("Chyba v select()");
						close(socket);
						close(l);
						return -1;
					}

					if ( ! FD_ISSET(socket, &sockets)) {
						// Zde je jasne, ze funkce select() skoncila cekani kvuli timeoutu.
						cout << "Connection timeout!" << endl;
						close(socket);
						close(l);
						return 0;
					}

					if (application == LOGIN) {
						retval = makeLogin(buffer);
						
						if (retval > 0) {
							sendMessage(500, socket);
							close(socket);
							close(l);
							return -1;
						}
						
						sendMessage(202, socket);
						application = INFO;
					} else if (application == INFO) {
						unsigned char firstLetter = buffer->getByte();
						int returnValue = 0;

						if (firstLetter == 'I') {
							returnValue = loadInfo(buffer);
						} else if (firstLetter == 'F') {
							returnValue = loadPhoto(buffer);
						} else {
							returnValue = 1;
						}

						if (returnValue == 1) {
							sendMessage(501, socket);
							close(socket);
							close(l);
							return -1;
						} else if (returnValue == 2) {
							sendMessage(300, socket);
						} else {
							sendMessage(202, socket);
						}
					}
				}

				close(socket);
				return 0;
			}
		}
	}

	close(l);
	return 0;
}
