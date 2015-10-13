#pragma once

#include <iostream>
using std::string;

class Connection
{
public:
	static Connection* initialize(string server_ip, int server_port);
	void client_connection();
	~Connection();

private:
	Connection(string server_ip, int server_port);

	string server_ip;
	int server_port;
	int client_descriptor;
};