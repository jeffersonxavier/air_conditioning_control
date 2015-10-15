#include "log.h"
#include <err.h>
#include <cstdio>
#include <ctime>
#include <sys/stat.h>

#define ERROR_FOLDER -2
#define ERROR_OPEN -3

static Log* log = nullptr;
FILE* log_file = nullptr;

Log::Log()
	: folder_log("/var/log/air_conditioning_server/"),
	file_name("/var/log/air_conditioning_server/log.out")
{
	mkdir(folder_log.c_str(), 0777);
}

Log*
Log::get_instance()
{
	if (log == nullptr)
		log = new Log();

	return log;
}

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

void
Log::open()
{
	log_file = fopen(file_name.c_str(), "a+");

	if (not log_file)
		errx(ERROR_OPEN, "Fail in open file!");
}