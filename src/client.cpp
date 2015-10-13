#include "screen.h"
#include "controller.h"

int main()
{
	bool status_air = false;
	double temperature = 0.0;

	bool execute = true;
	while(execute) {
		int option = Screen::show_menu(status_air, temperature);
		
		switch(option)
		{
			case 1:
				break;
			case 2:
				Controller::exit_program();
				execute = false;
				break;
			default:
				Controller::invalid_option();
				break;
		}
	}

	return 0;
}