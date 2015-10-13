#pragma once

#include <iostream>
using std::string;

class Connection
{
public:
	static Connection* initialize(string server_ip, int server_port);
	static void do_connect();

private:
	Connection(string server_ip, int server_port);

	string server_ip;
	int server_port;
	int server_descriptor;
};