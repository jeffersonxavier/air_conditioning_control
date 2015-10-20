#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

char server_ip[20] = "15.0.62.18";
int server_port = 4001;

int client_connection();
void send_message(int id, int size, char message[]);
char* receive(int id);

int main(int argc, char const *argv[])
{
	int descriptor = client_connection();

	char message[4] = "3298";

	send_message(descriptor, 4, message);
	char* result = receive(descriptor);

	printf("%s\n", result);
	return 0;
}

int do_connect(struct sockaddr_in* server_addr)
{
	int descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (descriptor == -1)
		errx(-1, "Fail in socket function!");

	bzero((char *) server_addr, sizeof(*server_addr));

	server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(server_port);
    server_addr->sin_addr.s_addr = inet_addr(server_ip);

    return descriptor;
}

int client_connection()
{
	struct sockaddr_in server_addr;
	int socket_descriptor = do_connect(&server_addr);

    if (connect(socket_descriptor,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		errx(-1, "Fail in connect function!");

	return socket_descriptor;
}

void send_message(int id, int size, char message[])
{
	if (send(id, &size, sizeof(size), 0) < 0)
		errx(-1, "Fail in send function!");

	if (send(id, message, size, 0) < 0)
		errx(-1, "Fail in send function!");
}

char* receive(int id)
{
	int size;
	if (recv(id, &size, sizeof(size), 0) <= 0)
		errx(-1, "Fail in first recv function!");

	char* message = (char*) malloc(size);

	if (recv(id, message, size, 0) <= 0)
		errx(-1, "Fail in second recv function!");

	return message;
}