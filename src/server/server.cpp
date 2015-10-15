#include "connection.h"
#include <err.h>

#define ERROR_PARAMS -1

int main(int argc, char const *argv[])
{
	if (argc < 2)
		errx(ERROR_PARAMS, "Invalid number of params");

	string server_ip = argv[1];
	Connection connection_temperature(server_ip, TEMPERATURE_PORT);

	connection_temperature.server_connection();
	connection_temperature.accept_connections();

	return 0;
}