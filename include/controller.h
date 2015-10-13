#pragma once

class Controller
{
public:
	static void clear();	
	static int show_menu(bool status_air, double temperature);
	static void exit_program();
	static void invalid_option();

private:
	static void wait_time();
};