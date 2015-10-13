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

static Connection* connection = nullptr;

Connection::Connection(string server_ip, int server_port)
	: server_ip(server_ip), server_port(server_port), client_descriptor(0)
{
}

Connection::~Connection()
{
	if (client_descriptor)
		close(client_descriptor);
}

Connection*
Connection::initialize(string server_ip, int server_port)
{
	if (connection == nullptr)
	{
		connection = new Connection(server_ip, server_port);
	}

	return connection;
}

void
Connection::client_connection()
{
	client_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (client_descriptor == -1)
		errx(ERROR_SOCKET, "Fail in socket function!");

	struct sockaddr_in server_addr;
	bzero((char *) &server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    if (connect(client_descriptor,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
    	close(client_descriptor);
		errx(ERROR_CONNECT, "Fail in connect function!");    	
    }
}
