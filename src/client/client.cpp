#include "connection.h"
#include "controller.h"
#include <cstdlib>
#include <err.h>

#define ERROR_PARAMS -1

int main(int argc, char const *argv[])
{
	if (argc < 2)
		errx(ERROR_PARAMS, "Invalid number of params");

	bool status_air = false;

	string server_ip = argv[1];

	Connection connection_temperature(server_ip, TEMPERATURE_PORT);

	Controller* controller = Controller::get_instance();
	controller->temperature_controller(connection_temperature);

	bool execute = true;
	while(execute) {
		int option = controller->show_menu(status_air);
		
		switch(option)
		{
			case 1:
				break;
			case 2:
				controller->exit_program();
				execute = false;
				break;
			default:
				controller->invalid_option();
				break;
		}
	}

	return 0;
}