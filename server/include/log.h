#pragma once

#include <iostream>

using std::string;

class Log
{
public:
	~Log();

	static Log* get_instance();
	void write(string message);

private:
	Log();

	void open();

	string folder_log;
	string file_name;
};