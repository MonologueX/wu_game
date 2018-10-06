#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
  uint64_t i = 0;
  
  int id = -1;
  id = fork();
  if (id < 0)
  {
    return -1;
  }
  else if (id == 0)
  {
    sleep(30);
    exit(0);
  }
  while(1)
  {
    sleep(1);
    i++;
  }
  return 0;
}
