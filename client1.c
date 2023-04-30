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
	// КЛИЕНТ КОННЕКТИТСЯ
	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	printf("connected to the server\n");
	
	while(1) {
		bzero(buffer, 1024);
		// strcpy(buffer, "Hello, this is client");
		printf("message: ");
		// scanf("%s", buffer);
		fgets(buffer, 1024, stdin);
		buffer[strlen(buffer) - 1] = '\0';
		// printf("%s\n", buffer);
		send(sock, buffer, strlen(buffer), 0);
		if (strcmp(buffer, "The End") == 0) {
			close(sock);
			printf("diconnected from the server\n");
			return 0;
		}
	}
}
