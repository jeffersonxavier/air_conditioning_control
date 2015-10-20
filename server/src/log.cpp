#include "log.h"
#include <err.h>
#include <cstdio>
#include <ctime>
#include <sys/stat.h>

#define ERROR_FOLDER -2
#define ERROR_OPEN -3

static Log* log = nullptr;
FILE* log_file = nullptr;

//Init folder to log file and log file name
Log::Log()
	: folder_log("air_conditioning_log/"),
	file_name("air_conditioning_log/log.out")
{
	mkdir(folder_log.c_str(), 0777);
}

//Singleton to instanciate log
Log*
Log::get_instance()
{
	if (log == nullptr)
		log = new Log();

	return log;
}

//write date and a message in log file
void
Log::write(string message)
{
	open();

	long t_date;
	time(&t_date);

	string date = ctime(&t_date);
	date = date.substr(0, date.size() - 2); //remove newlines

	fprintf(log_file, "%s - %s\n", date.c_str(), message.c_str());

	fclose(log_file);
}

//Open file to append
void
Log::open()
{
	log_file = fopen(file_name.c_str(), "a+");

	if (not log_file)
		errx(ERROR_OPEN, "Fail in open file!");
}