#include "main.h"

void signal_handler_child_end(int signal)
{
  pid_t pid;
  int status;

  do
  {
    pid = waitpid(-1, &status, WNOHANG);
    if(pid == -1)
    {
      if(errno == ECHILD)
        break;
      error("Cannot waitpid(%d %s)", errno, strerror(errno));
    }
  }
  while(pid != 0);
}
