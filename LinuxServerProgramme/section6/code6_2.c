#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#define BUFFER_SIZE 1024

int main(int argc, char **atgv)
{
    if (argc < 4)
    {
        printf("param error!\n");
    }
    char *ip = argv[1];
    int port = ntohs(argv[2]);

    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_fd < 0)
    {
        perror("socket fail!!");
        return -1;
    }


    struct sockaddr_in server_addr;
    server_addr.sin_family = IF_INET;
    server_addr.sin_port = port;
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (bind(soc_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind fail!!");
        close(soc_fd);
        return -1;
    }

    if (listen(soc_fd, 5) < 0)
    {
        perror("listen fail!!!");
        close(soc_fd);
        return -1;
    }

    struct sockaddr_in acc_addr;
    socklen_t acc_addr_len = sizeof(acc_addr);
    int acc_fd = accept(soc_fd, (struct sockaddr *)&acc_addr, &acc_addr_len);
    if (acc_fd < 0)
    {
        perror("accept fail!!!");
        close(soc_fd);
        return -1;
    }

    //file imformation
    char handle_buf[BUFFER_SIZE];
    char *file_buf;
    struct stat file_stat;
    int len = 0;
    

    close(acc_fd);
    close(soc_fd)
}
