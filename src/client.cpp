#include "connection.h"
#include "screen.h"
#include "controller.h"
#include <cstdlib>
#include <err.h>

#define ERROR_PARAMS -1
int main(int argc, char const *argv[])
{
	if (argc < 2)
		errx(ERROR_PARAMS, "Invalid number of params");

	bool status_air = false;
	double temperature = 0.0;

	string server_ip = argv[1];
	int server_port = atoi(argv[2]);

	Connection* connection = Connection::initialize(server_ip, server_port);
	connection->client_connection();

	bool execute = true;
	while(execute) {
		int option = Screen::show_menu(status_air, temperature);
		
		switch(option)
		{
			case 1:
				break;
			case 2:
				Controller::exit_program();
				execute = false;
				break;
			default:
				Controller::invalid_option();
				break;
		}
	}

	return 0;
}