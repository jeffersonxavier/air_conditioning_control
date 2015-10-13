#include "screen.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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