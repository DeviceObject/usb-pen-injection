#include "NetworkHeader.h"

// -----
//   PenClient.c
//		This program implements the connection between the two
//			computers using network sockets
int
main(int argc, char **argv)
{
	int sock;						/* Socket descriptor for client */
	struct sockaddr_in ServerAddr;	/* Local address */
	char *ServerHost;					/* Server address in dotted quad */
	char *ServerPortString;				/* Server port */
	unsigned short ServerPort; 		/* Server port */
	unsigned int clntLen;				/* Length of client address data structure */

	if(argc != 2)
	{
		printf("Usage: %s [<server IP>[:<port>]]", argv[0]);
		exit(1);
	}
	else
	{
		ServerHost = strtok(argv[1], ":");

		if((ServerPortString = strtok(NULL, ":")) != NULL)
			ServerPort = htons(atoi(ServerPortString));
	}

	printf("IP is %s, port is %d!\n", ServerHost, ntohs(ServerPort));

	struct WSAData data = { 0 };
	WORD wVersionRequested = MAKEWORD(1,1);

	if(WSAStartup(wVersionRequested, &data))
		DieWithError("WSAStartup() failed");

	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("sock() failed");

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family		= AF_INET;
	ServerAddr.sin_addr.s_addr	= inet_addr(ServerHost);
	ServerAddr.sin_port			= ServerPort;

	if(connect(sock, (struct sockaddr *) &ServerAddr, sizeof(ServerAddr)) < 0)
		DieWithError("connect() failed");

	for(;;)
	{
		char RecvBuffer[20];
		memset(RecvBuffer, 0, 20);
		if(recv(sock, RecvBuffer, 20, 0) < 0)
			break;

		RecvBuffer[19] = '\0';
		printf("%s\n", RecvBuffer);
	}


	closesocket(sock);

	WSACleanup();

	return(0);
}