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

Connection::Connection(string server_ip, int server_port)
	: server_ip(server_ip), server_port(server_port), client_descriptor(0), server_descriptor(0)
{
}

Connection::~Connection()
{
	if (client_descriptor)
		close(client_descriptor);
	if (server_descriptor)
		close(server_descriptor);
}

void
Connection::client_connection()
{
	struct sockaddr_in server_addr;
	client_descriptor = do_connect(&server_addr);

    if (connect(client_descriptor,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
    	close(client_descriptor);
		errx(ERROR_CONNECT, "Fail in connect function!");    	
    }
}

void
Connection::server_connection()
{
	struct sockaddr_in server_addr;
	server_descriptor = do_connect(&server_addr);
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
