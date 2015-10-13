#include "connection.h"

static Connection* connection = nullptr;

Connection::Connection(string server_ip, int server_port)
	: server_ip(server_ip), server_port(server_port), server_descriptor(0)
{
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
Connection::do_connect()
{
	
}
