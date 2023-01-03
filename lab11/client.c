// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char *hello = "Hello\n";
	char buffer[1024] = {0};

   	usleep(500000);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	
	FILE* ptr;
	ptr = fopen("in", "r"); // read file
	FILE* ptr2;
	ptr2 = fopen("out", "w"); // open new file
	char op[1024] = {0}; // get single char
	char msg[1024] = {0}; // one command like "add 1 26"
	char out[1024] = {0}; // result sent from server

	while(1){
		fgets(op, 2, ptr);
		if(strcmp(op, "\n")){
			strcat(msg, op); // concatenate "op" ans "msg" to form a command
		}else{
			send(sock, msg, sizeof(msg), 0); // send msg to server
			if(msg[0] == 'a' && msg[1] == 'd' && msg[2] == 'd'){
				valread = read(sock, out, 1024); // read result from server
				printf("%s\n", out);
				fprintf(ptr2, "%s\n", out); // print result to file "out"
			}else if(msg[0] == 'a' && msg[1] == 'b' && msg[2] == 's'){
				valread = read(sock, out, 1024); // read result from server
				printf("%s\n", out);
				fprintf(ptr2, "%s\n", out); // print result to file "out"
			}else if(msg[0] == 'm' && msg[1] == 'u' && msg[2] == 'l'){
				valread = read(sock, out, 1024); // read result from server
				printf("%s\n", out);
				fprintf(ptr2, "%s\n", out); // print result to file "out"
			}else if(msg[0] == 'k' && msg[1] == 'i' && msg[2] == 'l' && msg[3] == 'l'){
				break; // if the command is "kill" -> break
			}else{
				valread = read(sock, out, 1024); // read result from server
				printf("%s\n", out);
				fprintf(ptr2, "%s\n", out); // print result to file "out"
			}	
			int i;
			for(i=0; i<15; i++){
				msg[i] = '\0'; // initialize "msg" again
			}
		}
	}
	close(client_fd);
	return 0;
}
