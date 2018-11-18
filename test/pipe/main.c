#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if (ret < 0)
    {
        perror("pipe");
        return 1;
    }

    pid_t id = fork();
    if (id == 0)
    {
        close(fd[0]);
        const char *msg = "hello child\n";
        while (1)
        {
            sleep(1);
            write(fd[1], msg, strlen(msg));
        }
    }
    else 
    {
        close(fd[1]);
        char buf[64];
        while (1)
        {
            ssize_t s = read(fd[0], buf, sizeof(buf) - 1);
            if (s > 0)
            {
                buf[s] = '\0';
                printf("parent saying:%s\n", buf);
            }
        }
    }

    return 0;
}
