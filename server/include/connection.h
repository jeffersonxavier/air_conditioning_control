#pragma once

#include <iostream>

#define TEMPERATURE_PORT 3007
#define AIR_CONDITIONING_PORT 8007

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
	float get_uart_relative_humidity();
	bool control_uart_air(bool action);

	int open_uart();
	int write_uart(int uart_descriptor, unsigned char buffer[], int size);
	int read_uart(int uart_descriptor, void* buffer, int size);
};