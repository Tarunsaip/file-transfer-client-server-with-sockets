#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 80
#define PORT 3010
// function to connect with client
void handle_server(int socketfd, char *argv[])
{
	char buff[MAX];
	int n;
	bzero(buff, sizeof(buff));
	write(socketfd, argv[3], sizeof(buff)); // file2
	write(socketfd, argv[1], sizeof(buff)); // pattern
	bzero(buff, sizeof(buff));
	read(socketfd, buff, sizeof(buff));
	
	int fd[2];
	int k=pipe(fd);
	if(k == -1)
		exit(1); 
	if(fork() == 0){
		close(fd[0]);
		char *args[]={"grep", "--color=auto", "-w", argv[1], argv[2], argv[3], NULL};
		execvp(args[0],args);
	}
	else {
		char output[MAX];
		close(fd[1]);//since parent does not use fd[1]
		read(fd[0], &output, MAX);
	        printf("\n");
	}
}

int main(int argc, char *argv[])
{
	int socketfd, connfd;
	struct sockaddr_in servaddr, cli;
	if(argc !=4) {
		printf("Call model: %s <pattern> <file1> <file2>\n", argv[0]);
		exit(0);

	}

	// socket creation
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1) {
		printf("Socket Creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	//Assign address
	servaddr.sin_family = AF_INET;
	//Assign local IP address
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//Assign port number
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Couldn't connect to the Server...\n");
		exit(0);
	}
	else
		printf("Connection successfull..\n");

	// function to connect with server
	handle_server(socketfd, argv);

	// close the socket
	close(socketfd);
}
