// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#define ADD 1
#define ABS 2
#define MUL 3
#define NOT 4

int get_next_space(char* str, int start){
    int i;
    for(i = start; str[i] != ' ' && i < strlen(str); i++);
    return i == strlen(str) ? -1 : i;
}

int get_int(char* str, int start){
    int i, res = 0;
    for(i = start; i < strlen(str) && str[i] >= '0' && str[i] <= '9'; i++){
        res *= 10;
        res += (str[i] - '0');
    }
    return res;
}

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	//char buffer[1024] = {0};
	char *hello = "Hello";
    char *del;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	char msg[1024] = {0}; // receive from client
	char ans[1024] = {0}; // store "rlt" val as char array
	long num1 = 0; // store the number for some operation
	long num2 = 0; // store the number for some operation if there's number2
	long rlt = 0; // store the result of num1 and num2 after some operation
	char* p;

	while(1){
		valread = read(new_socket, msg, 1024); // read "msg" from client
		if(msg[0] == 'a' && msg[1] == 'd' && msg[2] == 'd'){ //if add
			num1 = strtol(msg + 4, &p, 10); //get num1
			num2 = strtol(p, &p, 10); // get num2
			rlt = num1 + num2; // add
			sprintf(ans, "%ld", rlt); // change "rlt" which is type of long int to string
			//printf("%s\n", ans);
			send(new_socket, ans, sizeof(ans), 0);
			//printf("%ld\n", rlt);
		}else if(msg[0] == 'a' && msg[1] == 'b' && msg[2] == 's'){ //if abs
			num1 = strtol(msg + 4, &p, 10);
			if(num1 < 0) rlt = num1 * (-1); // abs
			else rlt = num1;
			sprintf(ans, "%ld", rlt);
			send(new_socket, ans, sizeof(ans), 0);
			//printf("%ld\n", rlt);
			//printf("%s\n", ans);
		}else if(msg[0] == 'm' && msg[1] == 'u' && msg[2] == 'l'){ // if mul
			num1 = strtol(msg + 4, &p, 10);
			num2 = strtol(p, &p, 10);
			rlt = num1 * num2; // mul
			sprintf(ans, "%ld", rlt);
			send(new_socket, ans, sizeof(ans), 0);
			//printf("%ld\n", rlt);
		}else if(msg[0] == 'k' && msg[1] == 'i' && msg[2] == 'l' && msg[3] == 'l'){ //if kill
			break; // then it will close "new_socket"
		}else{ // other condition
			send(new_socket, hello, strlen(hello), 0);
		}
		for(int i=0; i<15; i++){
			msg[i] = '\0';
		}
	}
	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
return 0;
}
