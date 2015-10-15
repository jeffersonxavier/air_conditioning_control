#pragma once

#include <iostream>

#define TEMPERATURE_PORT 8080
#define AIR_CONDITIONING_PORT 8080

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
	float get_temperature();
	
	int get_socket_descriptor();
	void send_message(int id, int size, string message);
	string receive(int client_id);

private:
	string server_ip;
	int server_port;
	int socket_descriptor;

	int do_connect(struct sockaddr_in* server_addr);
};