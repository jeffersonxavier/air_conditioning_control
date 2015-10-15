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
#define ERROR_BIND -3
#define ERROR_LISTEN -4
#define ERROR_RECV -5
#define ERROR_SEND -6
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
		else
		{
			printf("Client connection %s\n", inet_ntoa(client.sin_addr));
			receive_messages(client_id);
		}

		close(client_id);
	}
}

void
Connection::receive_messages(int client_id)
{
	string message = receive(client_id);

	if (message == "get_temperature")
	{
		float temperature = 39.5;

		if (send(client_id, &temperature, sizeof(temperature), 0) < 0)
			cerr << "Fail in send function!" << endl;
	}
	else if (message == "air_control")
	{
		message = receive(client_id);

		if (message == "turn_on")
			printf("turn_on received!\n");
		else
			printf("turn_off received!\n");

		string response = "success";
		int size = response.size() + 1;
		send_message(client_id, size, response);
	}
}

void
Connection::send_message(int id, int size, string message)
{
	if (send(id, &size, sizeof(size), 0) < 0)
		errx(ERROR_SEND, "Fail in send function!");

	if (send(id, message.c_str(), size, 0) < 0)
		errx(ERROR_SEND, "Fail in send function!");
}

string
Connection::receive(int id)
{
	int size;
	if (recv(id, &size, sizeof(size), 0) <= 0)
		errx(ERROR_RECV, "Fail in first recv function!");

	char* message = (char*) malloc(size);

	if (recv(id, message, size, 0) <= 0)
		errx(ERROR_RECV, "Fail in second recv function!");

	string result = message;
	free(message);

	return result;
}