#pragma once

#include <iostream>

#define TEMPERATURE_PORT 8080
#define AIR_CONDITIONING_PORT 3000

using std::string;

class Connection
{
public:
	Connection(string server_ip, int server_port);
	~Connection();

	void server_connection();
	void accept_connections();
	void receive_messages(int client_id);
	
	void send_message(int id, int size, string message);
	string receive(int client_id);

private:
	string server_ip;
	int server_port;
	int socket_descriptor;
	string client_ip;
	float temperature;
	bool status_air;

	int do_connect(struct sockaddr_in* server_addr);
	float get_uart_temperature();

	int open_uart();
	int write_uart(int uart_descriptor, unsigned char buffer);
	int read_uart(int uart_descriptor, void* buffer, int size);
};