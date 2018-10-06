#include <stdio.h>
#include <unistd.h>

int main()
{
  int id = fork();

  if (id < 0)
  {
    return -1;
  }
  else if (id == 0)
  {
    while (1)
    {
      sleep(1);
    }
  }
  return 0;
}
