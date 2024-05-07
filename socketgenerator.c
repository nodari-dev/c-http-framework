#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ERROR_DURING_SOCKET_PROCESSING -1

int generate_socket(struct sockaddr_in *address){
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd == -1){
		printf("Couldn't create a socket'\n");
		return -1;
	}

	printf("Created a socket\n");

	if(bind(socketfd, (struct sockaddr *)address,  sizeof(*address)) != 0){
		printf("Couldn't bind a socket\n");
		return -1;
	}

	printf("Socket has been binded\n");

	if(listen(socketfd, SOMAXCONN) != 0){
		printf("this shit ain't gonna listen'\n");
		return -1;
	}
	
	return socketfd;
}
