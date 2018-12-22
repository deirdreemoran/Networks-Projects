/**********************************************
Name: Deirdre Moran
Program: chatclient.c
Description: Client side of server/client
			socket chat
**********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Error function for reporting errors
void error(const char *msg)
{
	perror(msg);
	exit(0);
}

/*******************************************************
* getUserName - gets user name from user
********************************************************/
void getUserName(char * buffer)
{
	// prompt user for username
	printf("Please enter username: ");
	fgets(buffer, 9, stdin);
	// erase newline character
	buffer[strcspn(buffer, "\n")] = '\0';
}


/*****************************************************
* Main begins
*****************************************************/
int main(int argc, char *argv[])
{
	char * clientHandle = (char*)malloc(256 * sizeof(char));

	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[500];
	char buffer2[500];
	char buffer3[500];
	char sigQuit[] = "\\quit";

    // Check usage & args
	if (argc < 2)
	{
		fprintf(stderr,"USAGE: %s hostname port\n", argv[0]);
		exit(0);
	}

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(argv[2]);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverHostInfo = gethostbyname(argv[1]);

	if (serverHostInfo == NULL)
	{
		fprintf(stderr, "CLIENT: ERROR, no such host\n");
		exit(0);
	}
	// create address
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);

	// Set up the socket file descriptor
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	// if socket does not open, ERROR
	if (socketFD < 0)
	{
		error("CLIENT: ERROR opening socket");
	}

	// get client handle
	memset(clientHandle, '\0', sizeof(clientHandle));
	getUserName(clientHandle);

	while(1){
		printf("%s> ", clientHandle);
		memset(buffer, '\0', sizeof(buffer));
		memset(buffer3, '\0', sizeof(buffer3));
		fgets(buffer, sizeof(buffer) - 1, stdin);
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strncmp(buffer, sigQuit, sizeof(buffer)) == 0){
			close(socketFD);
			break;
		}
		strcat(buffer3, clientHandle);
		strcat(buffer3, "> ");
		strcat(buffer3, buffer);

		// Connect to server
		if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
			error("CLIENT: ERROR connecting");
		// Send message to server
		charsWritten = send(socketFD, buffer3, strlen(buffer3), 0);

		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");

		// Get return message from server
		memset(buffer2, '\0', sizeof(buffer2));
		charsRead = recv(socketFD, buffer2, sizeof(buffer2) - 1, 0);

		if (charsRead < 0) error("CLIENT: ERROR reading from socket");
		if (strncmp(buffer2, sigQuit, sizeof(buffer2)) == 0){
			//
				close(socketFD);
				error("CONNECTION TERMINATED BY SERVER");
			//	break;
			}
		printf("%s\n", buffer2);
		// Close the socket
		close(socketFD);
		// Create the socket once again
		socketFD = socket(AF_INET, SOCK_STREAM, 0);
}
close(socketFD);
	return 0;
}

