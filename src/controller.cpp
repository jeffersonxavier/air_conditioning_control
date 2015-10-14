#include "connection.h"
#include "controller.h"
#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;

#define WAIT 2

static Controller* controller = nullptr;

Controller::Controller()
	: temperature(0)
{
}

Controller*
Controller::get_instance()
{
	if (controller == nullptr)
		controller = new Controller();

	return controller;
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
Controller::show_menu(bool status_air)
{
	clear();

	cout << "\t\tSistema de Controle de Ar condicionado" << endl << endl;
	cout << "Ar Condicionado: " << (status_air ? "Ligado" : "Desligado");
	cout << "\t\t";
	cout << "Temperatura do Ambiente: " << fixed << setprecision(1) << temperature << endl << endl;

	cout << "1 - Ligar/Desligar Ar Condicionado!" << endl;
	cout << "2 - Sair" << endl << endl;
	cout << "-> ";

	int option;
	cin >> option;

	return option;
}

void
Controller::exit_program()
{
	clear();

	cout << "Saindo!" << endl;
	wait_time();
}

void
Controller::invalid_option()
{
	clear();

	cout << "Opcao Invalida!" << endl;
	cout << "Espere!" << endl;
	wait_time();
}

void
Controller::wait_time()
{
	for (int i = WAIT; i >= 1; --i)
	{
		cout << i << "..." << endl;
		sleep(1);
	}
}

void*
Controller::temperature_controller(Connection connection)
{
	connection.client_connection();
	temperature = connection.get_temperature();

	return nullptr;
}
