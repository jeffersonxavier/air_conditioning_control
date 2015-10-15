#include "connection.h"
#include "log.h"
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

Log* logger = nullptr;

Connection::Connection(string server_ip, int server_port)
	: server_ip(server_ip), server_port(server_port), socket_descriptor(0), client_ip(""),
		temperature(0.0), status_air(false)
{
	logger = Log::get_instance();
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
	{
		logger->write("Fail in socket function!");
		errx(ERROR_SOCKET, "Fail in socket function!");
	}

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
	{
		logger->write("Fail in bind function!");
		errx(ERROR_BIND, "Fail in bind function!");
	}

	if (listen(socket_descriptor, QUEUE_LISTEN) < 0)
	{
		logger->write("Fail in listen function!");
		errx(ERROR_LISTEN, "Fail in listen function!");
	}

	logger->write("Startup Server Complet!");
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
			logger->write("Fail in accept function!");
			continue;
		}
		else
		{
			client_ip = inet_ntoa(client.sin_addr);
			logger->write("Client connection with ip: " + client_ip);

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
		logger->write("Request temperature from client " + client_ip);
		temperature = 39.5;

		if (send(client_id, &temperature, sizeof(temperature), 0) < 0)
			logger->write("Fail in send temperature!");
	}
	else if (message == "air_control")
	{
		logger->write("Request air_control from client " + client_ip);
		status_air = not status_air;
		
		string status;
		if (status_air)
		{
			logger->write("Turning on air conditioning");
			status = "turn_on";
		}
		else
		{
			logger->write("Turning off air conditioning");
			status = "turn_off";
		}

		string response = status;
		int size = response.size() + 1;
		send_message(client_id, size, response);
	}
}

void
Connection::send_message(int id, int size, string message)
{
	if (send(id, &size, sizeof(size), 0) < 0)
	{
		logger->write("Fail in send message size ");
		return;
	}

	if (send(id, message.c_str(), size, 0) < 0)
		logger->write("Fail in send message " + message);
}

string
Connection::receive(int id)
{
	int size;
	if (recv(id, &size, sizeof(size), 0) <= 0)
		logger->write("Fail in receive message size from client " + client_ip);

	char* message = (char*) malloc(size);

	if (recv(id, message, size, 0) <= 0)
		logger->write("Fail in receive message from client " + client_ip);

	string result = message;
	free(message);

	return result;
}