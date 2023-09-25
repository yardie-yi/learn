#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("param error!\n");
        return -1;
    }

    char *ip = argv[1];
    int post = atoi(argv[2]);

    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_fd < 0)
    {
        perror("socket fail!");
        return -1;
    }

    int rec_buf = atoi(argv[3]);
    socklen_t rec_buf_len = sizeof(rec_buf);
    if(setsockopt(soc_fd, SOL_SOCKET, SO_RCVBUF, (void *)&rec_buf, &rec_buf_len) < 0)
    {
        perror("setsockopt fail");
        close(soc_fd);
        return -1;
    }

    int get_buf = 0;
    socklen_t get_buf_len = sizeof(get_buf);
    if(getsockopt(soc_fd, SOL_SOCKET, SO_RCVBUF, (const void *)&get_buf, get_buf_len) < 0)
    {
        perror("getsockopt fail!");
        close(soc_fd);
        return -1;
    }

    printf("getsockopt buf is %d\n", get_buf);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
    if(bind(soc_fd, (struct sockaddr *), sizeof()) < 0)
    {
        perror("bind error");
        close(soc_fd);
        return -1;
    }

    

    return 0;
}
