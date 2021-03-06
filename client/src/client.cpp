#include "connection.h"
#include "controller.h"
#include <cstdlib>
#include <err.h>

#define ERROR_PARAMS -1

int main(int argc, char const *argv[])
{
	if (argc < 2)
		errx(ERROR_PARAMS, "Invalid number of params");

	string server_ip = argv[1];

	Connection connection_environment(server_ip, TEMPERATURE_PORT);
	Connection connection_air(server_ip, AIR_CONDITIONING_PORT);

	Controller* controller = Controller::get_instance();
	controller->temperature_controller(connection_environment);

	bool execute = true;
	while(execute) {
		int option = controller->show_menu();
		
		switch(option)
		{
			case 1:
				controller->air_conditioning_control(connection_air);
				break;
			case 2:
				controller->update_relative_humidity(connection_environment);
				break;
			case 3:
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