#pragma once

class Connection;

class Controller
{
public:
	static Controller* get_instance();
	void clear();	
	int show_menu();
	void exit_program();
	void invalid_option();
	void temperature_controller(Connection connection);

	float get_temperature();
	void set_temperature(float temp);
	void air_conditioning_control(Connection connection);

private:
	Controller();

	void print_temperature();

	float temperature;
	bool status_air;
};

extern void* update_temperature(void* conn);