#include "connection.h"
#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ERROR_SOCKET -2
#define ERROR_CONNECT -3
#define ERROR_BIND -4
#define ERROR_LISTEN -5
#define ERROR_RECV -6
#define ERROR_SEND -7
#define QUEUE_LISTEN 10

using std::cerr;
using std::endl;

Connection::Connection(string server_ip, int server_port)
	: server_ip(server_ip), server_port(server_port), socket_descriptor(0)
{
}

Connection::~Connection()
{
	if (socket_descriptor)
		close(socket_descriptor);
}

int
Connection::do_connect(struct sockaddr_in* server_addr)
{
	int descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (descriptor == -1)
		errx(ERROR_SOCKET, "Fail in socket function!");

	bzero((char *) server_addr, sizeof(*server_addr));

	server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(server_port);
    server_addr->sin_addr.s_addr = inet_addr(server_ip.c_str());

    return descriptor;
}

void
Connection::client_connection()
{
	struct sockaddr_in server_addr;
	socket_descriptor = do_connect(&server_addr);

    if (connect(socket_descriptor,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		errx(ERROR_CONNECT, "Fail in connect function!");
}

void
Connection::server_connection()
{
	struct sockaddr_in server_addr;
	socket_descriptor = do_connect(&server_addr);

	if (bind(socket_descriptor, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) < 0)
		errx(ERROR_BIND, "Fail in bind function!");

	if (listen(socket_descriptor, QUEUE_LISTEN) < 0)
		errx(ERROR_LISTEN, "Fail in listen function!");
}

void
Connection::get_temperature()
{
	string message = "get_temperature";
	int size = message.size() + 1;

	if (send(socket_descriptor, &size, sizeof(size), 0) < 0)
		errx(ERROR_SEND, "Fail in send function!");

	if (send(socket_descriptor, "get_temperature", size, 0) < 0)
		errx(ERROR_SEND, "Fail in send function!");
}

void
Connection::accept_connections()
{
	struct sockaddr_in client;
	socklen_t client_len;
	
	while(true)
	{
		int client_id = accept(socket_descriptor, (struct sockaddr *) &client, &client_len);

		if (client_id < 0)
		{
			cerr << "Fail in accept function!" << endl;
			continue;
		}

		// pid_t pid = fork();

		// if (pid == 0)
		// 	receive_messages(client_id);
	}
}

void
Connection::receive_messages(int client_id)
{
	int length;

	if (recv(client_id, &length, sizeof(length), 0) <= 0)
		errx(ERROR_RECV, "Fail in recv function!");

	char* message = (char*) malloc(length);

	if (recv(client_id, message, length, 0) <= 0)
		errx(ERROR_RECV, "Fail in recv function!");

	free(message);
}
