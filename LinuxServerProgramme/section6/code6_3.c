#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/sendfile.h>

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("param error!\n");
        return 1;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *file = argv[3];

    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (soc_fd < 0)
    {
        perror("socket fail");
        return -1;
    }

    struct sockaddr_in sock_server_addr;
    sock_server_addr.sin_family = AF_INET;
    sock_server_addr.sin_port = ntohs(port);
    inet_pton(AF_INET, ip, (void *)&sock_server_addr.sin_addr);

    if (bind(soc_fd, (struct sockaddr *)&sock_server_addr, sizeof(sock_server_addr)) < 0)
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

    struct sockaddr_in acc_sock_addr;
    socklen_t acc_sock_len = sizeof(acc_sock_addr);

    int acc_fd = accept(soc_fd, (struct sockaddr *)&acc_sock_addr, &acc_sock_len);
    if(acc_fd < 0)
    {
        perror("accept fail");
        close(soc_fd);
        return -1;
    }

    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("open file fail");
        close(soc_fd);
        close(acc_fd);
        return -1;
    }

    struct stat stat_buf;
    fstat(fd, &stat_buf);

    sendfile(acc_fd, fd, NULL, stat_buf.st_size);
    close(fd);
    close(acc_fd);
    close(soc_fd);
}
