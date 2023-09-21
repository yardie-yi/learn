#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#define BUF_SIZE 1024
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("param error\n");
        return -1;
    }
    const char *ip = argv[1];
    const int port = atoi(argv[2]);

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("socket fail");
        return -1;
    }
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    //serverAddr.sin_port = htons(port);
    serverAddr.sin_port = ntohs(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    if(bind(sock_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) <0 )
    {
        perror("bind fail");
        close(sock_fd);
        return -1;
    }
    
    if(listen(sock_fd, 5) < 0)
    {
        perror("listen fail");
        close(sock_fd);
        return -1;
    }

    struct sockaddr_in acc_addr;
    socklen_t acc_len = sizeof(acc_addr);
    int acc_fd = accept(sock_fd, (struct sockaddr *)&acc_addr, &acc_len);
    if (acc_fd < 0)
    {
        perror("accept fail");
        close(sock_fd);
        return -1;
    }

    char buf[BUF_SIZE];
    memset(buf, '\0', BUF_SIZE);
    int count = recv(acc_fd, buf, BUF_SIZE - 1, 0);
    printf("got%d bytes of normal data'%s'\n", count, buf);

    memset(buf, '\0', BUF_SIZE);
    count = recv(acc_fd, buf, BUF_SIZE - 1, MSG_OOB);
    printf("got%d bytes of oob data'%s'\n", count, buf);

    memset(buf, '\0', BUF_SIZE);
    count = recv(acc_fd, buf, BUF_SIZE - 1, 0);
    printf("got%d bytes of normal data'%s'\n", count, buf);

    close(acc_fd);
    close(sock_fd);
    return 0;
}
