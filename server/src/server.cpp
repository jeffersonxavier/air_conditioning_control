#include "connection.h"
#include <err.h>
#include <unistd.h>

#define ERROR_PARAMS -1

int main(int argc, char const *argv[])
{
	if (argc < 2)
		errx(ERROR_PARAMS, "Invalid number of params");

	string server_ip = argv[1];

	pid_t pid = fork();

	if (pid != 0)
	{
		Connection connection_air(server_ip, AIR_CONDITIONING_PORT);
		connection_air.server_connection();
		connection_air.accept_connections();
	}
	else
	{
		Connection connection_temperature(server_ip, TEMPERATURE_PORT);
		connection_temperature.server_connection();
		connection_temperature.accept_connections();
	}

	return 0;
}