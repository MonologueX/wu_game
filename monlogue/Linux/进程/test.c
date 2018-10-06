#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int value = 100;
  pid_t id = fork();
  if (id < 0)
  {
    printf("create chiled is process failed!!!\n");
    return -1;
  }
  else if (id == 0)
  {
    printf("This is a child !!! child = %d\n", getpid());
  }
  else 
  {
    sleep(1);
    printf("This is parent !!! parent = %d\n");
  }
  return 0;
}
