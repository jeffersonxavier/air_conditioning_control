#include "connection.h"
#include "controller.h"
#include <iomanip>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using std::cin;
using std::cout;
using std::endl;

#define WAIT 2

pthread_t temperature_thread;

static Controller* controller = nullptr;

//Init temperature with 0 and status air with undefined
Controller::Controller()
	:temperature(0), relative_humidity(0), status_air("Indefinido")
{
}

//Singleton to instanciate a Controller
Controller*
Controller::get_instance()
{
	if (controller == nullptr)
		controller = new Controller();

	return controller;
}

//Return actual temperature
float
Controller::get_temperature()
{
	return temperature;
}

//Set new temperature and call print in screen
void
Controller::set_temperature(float temp)
{
	temperature = temp;
	print_temperature();
}

//Print temperature in position x=3 and y=40, and move cursor to x=8 and y=8
void
Controller::print_temperature()
{
	printf("\033[%d;%dfTemperatura do Ambiente: %.2f\n\n", 3, 40, temperature);
	printf("\033[%d;%df", 9, 8);
	fflush(stdout);
}

//Clear screen
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

//Show options in screen
int
Controller::show_menu()
{
	clear();

	printf("\033[%d;%dfSistema de Controle de Ar Condicionado\n\n", 1, 20);
	printf("Ar Condicionado: %s", status_air.c_str());
	printf("\033[%d;%dfTemperatura do Ambiente: %.2f\t\t", 3, 40, temperature);
	printf("Umidade Relativa %.2f\n\n", relative_humidity);
	printf("1 - Ligar/Desligar Ar Condicionado!\n");
	printf("2 - Ver umidade relativa do ar\n");
	printf("3 - Sair\n\n");
	printf("Opcao: ");
	fflush(stdout);

	int option;
	cin >> option;

	return option;
}

//Cancel temperature thread and show message
void
Controller::exit_program()
{
	pthread_cancel(temperature_thread);
	cout << endl << "Saindo!" << endl;
}

//Show invalid option and wait a time
void
Controller::invalid_option()
{
	cout << endl << "Opcao Invalida!" << endl;
	sleep(WAIT);
}

//Send air_controll message to server and receive result
void
Controller::air_conditioning_control(Connection connection)
{
	connection.client_connection();

	string message = "air_control";
	int size = message.size() + 1;

	connection.send_message(connection.get_socket_descriptor(), size, message);

	string result = connection.receive(connection.get_socket_descriptor());

	if (result == "turn_on")
		status_air = "Ligado";
	else if (result == "turn_off")
		status_air = "Desligado";
}

void
Controller::update_relative_humidity(Connection connection)
{
	relative_humidity = connection.get_relative_humidity();
}

//Create thread to update temperature
void
Controller::temperature_controller(Connection connection)
{
	pthread_create(&temperature_thread, nullptr, &update_temperature, &connection);
}

//get temperature from server and update
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
