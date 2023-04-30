#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

int main() {
	char *ip = "127.0.0.1";
	int port = 5567;
	
	int server_sock, client_sock1, client_sock2;
	struct sockaddr_in server_addr, client_addr1, client_addr2;
	socklen_t addr_size1, addr_size2;
	char buffer[1024];
	int n;
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0) {
		perror("socket error: ");
		exit(1);
	}
	
	printf("server ip: %s\n", ip);
	printf("TCP server socket created\n");
	
	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	// ИМЕННО СЕРВЕРУ НУЖНО БИНДИТЬСЯ К ПОРТУ
	n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (n < 0) {
		perror("bind error: ");
		exit(1);
	}
	printf("bind to the port number: %d\n", port);
	
	listen(server_sock, 5);
	printf("Listening...\n");
	
	
	
	// (клиент1 заканчивает и все заканчивают тогда)
	addr_size1 = sizeof(client_addr1);
	client_sock1 = accept(server_sock, (struct sockaddr*)&client_addr1, &addr_size1);
	printf("client1 connected\n");
	
	addr_size2 = sizeof(client_addr2);
	client_sock2 = accept(server_sock, (struct sockaddr*)&client_addr2, &addr_size2);
	printf("client2 connected\n");
	
	
	
	bzero(buffer, 1024);
	//recv(client_sock1, buffer, sizeof(buffer), 0);
	//printf("client1: %s\n", buffer);
	int recvMsgSize;
	/* Receive message from client1 */
	if ((recvMsgSize = recv(client_sock1, buffer, sizeof(buffer), 0)) < 0) {
		perror("recv() failed");
		exit(1);
	}

	while (recvMsgSize > 0)      /* zero indicates end of transmission */
	{
		printf("client1: %s\n", buffer);
		// send(client_sock2, buffer, strlen(buffer), 0);
		if (send(client_sock2, buffer, strlen(buffer), 0) != strlen(buffer)) {
			perror("send() failed");
			exit(1);
		}
		bzero(buffer, 1024);
		/* See if there is more data to receive */
		if ((recvMsgSize = recv(client_sock1, buffer, sizeof(buffer), 0)) < 0) {
			perror("recv() failed");
			exit(1);
		}
	}
	
	close(client_sock1);
	printf("client1 disconected\n\n");
	
	close(client_sock2);
	printf("client2 disconected\n\n");

	return 0;
}
