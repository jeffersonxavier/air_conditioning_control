#pragma once

class Connection;

class Controller
{
public:
	static Controller* get_instance();
	void clear();	
	int show_menu(bool status_air);
	void exit_program();
	void invalid_option();
	void temperature_controller(Connection connection);

private:
	Controller();

	void wait_time();

	double temperature;
};

extern void* update_temperature(void* conn);