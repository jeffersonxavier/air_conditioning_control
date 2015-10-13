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

private:
	string server_ip;
	int server_port;
	int client_descriptor;
	int server_descriptor;

	int do_connect(struct sockaddr_in* server_addr);
};