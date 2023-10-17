#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include <fcntl.h>

//#define _GNU_SOURCE 
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("param error!\n");
        return -1;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (soc_fd < 0)
    {
        perror("socket fail");
        return -1;
    }

    struct sockaddr_in soc_addr;
    soc_addr.sin_family = AF_INET;
    soc_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, (void *)&soc_addr.sin_addr);

    if(bind(soc_fd, (struct sockaddr *)&soc_addr, sizeof(soc_addr))<0)
    {
        perror("bind fail");
        close(soc_fd);
        return -1;
    }

    if(listen(soc_fd, 5) < 0)
    {
        perror("listen fail");
        close(soc_fd);
        return -1;
    }

    struct sockaddr_in acc_addr;
    socklen_t acc_addr_len = sizeof(acc_addr);
    int acc_fd = accept(soc_fd, (struct sockaddr *)&acc_addr, &acc_addr_len);
    if(acc_fd < 0)
    {
        perror("accept fail");
        close(soc_fd);
        return -1;
    }

    int pipefd[2];
    pipe(pipefd);
    ssize_t ret = splice(acc_fd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE|SPLICE_F_NONBLOCK);
    assert(ret != -1);
    ret = splice(pipefd[0], NULL, acc_fd, NULL, 32768, SPLICE_F_MORE|SPLICE_F_NONBLOCK);
    assert(ret != -1);
    close(acc_fd);
    close(soc_fd);
    return 0;
}

