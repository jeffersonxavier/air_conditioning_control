#include "screen.h"
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

void
Screen::clear()
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
Screen::show_menu(bool status_air, double temperature)
{
	Screen::clear();

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
