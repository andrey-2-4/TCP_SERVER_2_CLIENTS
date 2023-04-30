#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

int main() {
	char *ip;
	int port;
	
	ip = malloc(256); // без этого нельзя нормально считать ip
	printf("server ip: ");
	scanf("%255s", ip);
	printf("server port: ");
	scanf("%d", &port);
	
	int sock;
	struct sockaddr_in addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket error: ");
		exit(1);
	}
	
	printf("TCP server socket created\n");
	
	memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(ip);
	// КЛИЕНТ КОННЕКТИТСЯ К СЕРВЕРУ
	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	printf("connected to the server\n");
	
	bzero(buffer, 1024);
	int recvMsgSize;
	/* Receive message from server from client1 */
	if ((recvMsgSize = recv(sock, buffer, sizeof(buffer), 0)) < 0) {
		perror("recv() failed");
		exit(1);
	}
	
	while (recvMsgSize > 0)      /* zero indicates end of transmission */
	{
		printf("server: client1: %s\n", buffer);
		bzero(buffer, 1024);
		/* See if there is more data to receive */
		if ((recvMsgSize = recv(sock, buffer, sizeof(buffer), 0)) < 0) {
			perror("recv() failed");
			exit(1);
		}
	}
	
	close(sock);
	printf("diconnected from the server\n");
	return 0;	
}
