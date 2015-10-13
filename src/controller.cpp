#include "controller.h"
#include "screen.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

#define WAIT 2

void
Controller::exit_program()
{
	Screen::clear();

	cout << "Saindo!" << endl;
	wait_time();
	exit(0);
}

void
Controller::invalid_option()
{
	Screen::clear();

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