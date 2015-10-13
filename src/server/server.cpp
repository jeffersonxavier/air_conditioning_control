#include "connection.h"
#include <err.h>

#define ERROR_PARAMS -1

int main(int argc, char const *argv[])
{
	if (argc < 1)
		errx(ERROR_PARAMS, "Invalid number of params");

	string server_ip = argv[1];
	Connection connection_temperature(server_ip, TEMPERATURE_PORT);
	Connection connection_air(server_ip, AIR_CONTROLLER_PORT);

	return 0;
}