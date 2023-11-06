#define _GNU_SOURCE 1
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<poll.h>
#include<fcntl.h>
#define BUFFER_SIZE 64

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("param error!\n");
        return -1;
    }

    int port = atoi(argv[2]);
    char *ip = argv[1];

    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in soc_addr;
    soc_addr.sin_family = AF_INET;
    soc_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, (void *)&soc_addr.sin_addr);

    if(connect(soc_fd, (struct sockaddr *)&soc_addr, sizeof(soc_addr)) < 0)
    {
        close(soc_fd);
        return -1;
    }

    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret != -1);

    struct pollfd fds[2];
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = soc_fd;
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;

    char read_buf[BUFFER_SIZE];

    while(1)
    {
        if(poll(fds, 2, -1) < 0)
        {
            printf("poll fail\n");
            return -1;
        }
        if(fds[1].revents & POLLRDHUP)
        {
            printf("server close the connection!\n");
            break;
        }
        else if(fds[1].revents & POLLIN)
        {
            memset(read_buf, '\0', BUFFER_SIZE);
            recv(fds[1].fd, read_buf, BUFFER_SIZE - 1, 0);
            printf("%s\n", read_buf);
        }

        if(fds[0].revents & POLLIN)
        {
            splice(0, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
            splice(pipefd[0], NULL, soc_fd, NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
        }
    }
    close(soc_fd);
    return 0;
}
