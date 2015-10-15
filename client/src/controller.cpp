#include "connection.h"
#include "controller.h"
#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

using std::cin;
using std::cout;
using std::endl;

#define WAIT 2

pthread_t temperature_thread;

static Controller* controller = nullptr;

Controller::Controller()
	:temperature(0), status_air(false)
{
}

Controller*
Controller::get_instance()
{
	if (controller == nullptr)
		controller = new Controller();

	return controller;
}

float
Controller::get_temperature()
{
	return temperature;
}

void
Controller::set_temperature(float temp)
{
	temperature = temp;
	print_temperature();
}

void
Controller::print_temperature()
{
	printf("\033[%d;%dfTemperatura do Ambiente: %.2f\n\n", 3, 40, temperature);
	printf("\033[%d;%df", 8, 8);
	fflush(stdout);
}

void
Controller::clear()
{
	pid_t pid = fork();

	if (pid == 0)
	{
		char **arguments = nullptr;
		execvp("clear", arguments);
	}
	else
		waitpid(pid, nullptr, 0);
}

int
Controller::show_menu()
{
	clear();

	printf("\033[%d;%dfSistema de Controle de Ar Condicionado\n\n", 1, 20);
	printf("Ar Condicionado: %s", (status_air ? "Ligado" : "Desligado"));
	printf("\033[%d;%dfTemperatura do Ambiente: %.2f\n\n", 3, 40, temperature);
	printf("1 - Ligar/Desligar Ar Condicionado!\n");
	printf("2 - Sair\n\n");
	printf("Opcao: ");
	fflush(stdout);

	int option;
	cin >> option;

	return option;
}

void
Controller::exit_program()
{
	pthread_cancel(temperature_thread);
	cout << endl << "Saindo!" << endl;
}

void
Controller::invalid_option()
{
	cout << endl << "Opcao Invalida!" << endl;
	sleep(WAIT);
}

void
Controller::air_conditioning_control(Connection connection)
{
	connection.client_connection();

	string message = "air_control";
	int size = message.size() + 1;

	connection.send_message(connection.get_socket_descriptor(), size, message);

	string action = not status_air ? "turn_on" : "turn_off";
	size = action.size() + 1;

	connection.send_message(connection.get_socket_descriptor(), size, action);
	string result = connection.receive(connection.get_socket_descriptor());

	if (result == "success")
		status_air = not status_air;
}

void
Controller::temperature_controller(Connection connection)
{
	pthread_create(&temperature_thread, nullptr, &update_temperature, &connection);
}

void*
update_temperature(void* conn)
{
	Connection *connection = (Connection*) conn;

	while (true)
	{
		connection->client_connection();
		float temperature = connection->get_temperature();

		Controller* controller = Controller::get_instance();

		if (controller->get_temperature() != temperature)
			controller->set_temperature(temperature);

		sleep(60);
	}

	return nullptr;
}
