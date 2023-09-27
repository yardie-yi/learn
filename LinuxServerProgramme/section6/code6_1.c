#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("param error!");
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


    struct sockaddr_in bind_addr;
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = ntohs(port);
    inet_pton(AF_INET, ip, &bind_addr.sin_addr);
    if (bind(soc_fd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0)
    {
        perror("bind faile");
        close(soc_fd);
        return -1;
    }

    listen(soc_fd, 5);

    int accept_fd;
    struct sockaddr_in accept_addr;
    socklen_t accept_addr_len = sizeof(accept_addr);
    accept_fd = accept(soc_fd, (struct sockaddr *)&accept_addr, &accept_addr_len);
    if(accept_fd < 0)
    {
        perror("accept fail");
        close(soc_fd);
        return -1;
    }


    close(1);
    dup(accept_fd);

    printf("abcd\n");

    close(accept_fd);
    close(soc_fd);
    return 0;
}
