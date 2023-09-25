#include<sys/socket.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#define BUF_SIZE 512
int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("param error!\n");
        return -1;
    }

    char *ip = argv[1];
    const int post = atoi(argv[2]);

    int soc_fd;
    soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_fd < 0)
    {
        perror("socket fail!");
        return -1;
    }

    int buf = atoi(argv[3]);
    if(setsockopt(soc_fd, SOL_SOCKET, SO_SNDBUF, &buf, sizeof(buf)) < 0)
    {
        perror("setsockopt error!");
        close(soc_fd);
        return -1;
    }

    int get_buf = 0;
    socklen_t get_buf_len = sizeof(get_buf);
    if(getsockopt(soc_fd, SOL_SOCKET, SO_SNDBUF, &get_buf, &get_buf_len) < 0)
    {
        perror("getsockopt error!");
        close(soc_fd);
        return -1;
    }
    printf("getsockopt buf = %d\n", get_buf);

    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(post);
    inet_pton(AF_INET, ip, &client_addr.sin_addr);
    if(connect(soc_fd, (struct sockaddr *)&client_addr, sizeof(client_addr))< 0)
    {
        perror("connect error");
        close(soc_fd);
        return -1;
    }

    char send_buf[BUF_SIZE];
    memset(send_buf, 'a', BUF_SIZE);
    send(soc_fd, send_buf, BUF_SIZE, 0);
    close(soc_fd);
    return 0;
}
