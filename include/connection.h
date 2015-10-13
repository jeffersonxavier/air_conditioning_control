#pragma once

#include <iostream>

#define TEMPERATURE_PORT 8080
#define AIR_CONTROLLER_PORT 3000

using std::string;

class Connection
{
public:
	Connection(string server_ip, int server_port);
	~Connection();
	void client_connection();
	void server_connection();
	void accept_connections();
	void receive_messages(int client_id);
	void get_temperature();

private:
	string server_ip;
	int server_port;
	int socket_descriptor;

	int do_connect(struct sockaddr_in* server_addr);
};