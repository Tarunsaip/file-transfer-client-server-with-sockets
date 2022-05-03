#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 80
#define PORT 3010

// Function to handle client
void handle_client(int connfd)
{
	char pattern[MAX], serverFilePath[MAX];
	int n;
	bzero(pattern, MAX);
	bzero(serverFilePath, MAX);
	// to get the file from client
	read(connfd, serverFilePath, sizeof(serverFilePath));
	printf("File from client:\t %s \n", serverFilePath);
	// to get the search pattern sent from client
	read(connfd, pattern, sizeof(pattern));
	printf("Pattern from client:\t %s \n", pattern);
	

	int 	fd[2];
	int k=pipe(fd);//create a pipe
	if(k == -1)
		exit(1); 
	//child process
	if(fork() == 0){
		close(fd[0]); // child dont't need fd[0]
		// to find the lines with given pattern in file and color it red
		char *args[]={"grep", "--color=auto", "-w", pattern, serverFilePath, NULL};
		execvp(args[0],args);
	}
	else {
		char output[MAX];
		close(fd[1]);//parent don't need fd[1]
		read(fd[0], &output, MAX);
		write(connfd, output, sizeof(output));
	}
}

//main function
int main()
{
	int socketfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// Create Socket
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1) {
		printf("Socket couldn't be created...\n");
		exit(0);
	}
	else
		printf("Socket creation Successfull...\n");
	bzero(&servaddr, sizeof(servaddr));

	//Assign internet address 
	servaddr.sin_family = AF_INET;
	//Assign IP Address
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//Assign Port number
	servaddr.sin_port = htons(PORT);

	// Binding the socket to given IP address and port number
	if ((bind(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Socket couldn't be Binded...\n");
		exit(0);
	}
	else
		printf("Socket Binding Successfull...\n");

	// server is listening 
	if ((listen(socketfd, 7)) != 0) {
		printf("Server Listen failed...\n");
		exit(0);
	}
	else
		printf("Server Listening for clients...\n");
	len = sizeof(cli);

	Refunction:;
	printf("\nWaiting for Client...\n");
	// Accept from client
	connfd = accept(socketfd, (struct sockaddr*)&cli, &len);
	if (connfd < 0) {
		printf("Server Accept failed...\n");
		exit(0);
	}
	else
		printf("Server connected with the client successfully...\n");

	// Function to handle the client
	handle_client(connfd);
	goto Refunction;

	// In the end close the socket
	close(socketfd);
}
