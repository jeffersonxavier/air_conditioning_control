#include "screen.h"
#include "controller.h"

int main()
{
	bool status_air = false;
	double temperature = 0.0;
	
	int option = Screen::show_menu(status_air, temperature);

	switch(option)
	{
		case 1:
		case 2:
			Controller::exit_program();
		default:
			Controller::invalid_option();
			Screen::show_menu(true, 0);
			break;
	}

	return 0;
}