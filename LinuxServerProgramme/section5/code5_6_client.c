#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("param error\n");
        return -1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("socket fail");
        return 1;
    }

    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &clientAddr.sin_addr);

    if(connect(sock_fd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0)
    {
        perror("connect fail");
        close(sock_fd);
        return -1;
    }

    const char *oob_data = "abc";
    const char *normal_data = "123";

    send(sock_fd, normal_data, strlen(normal_data), 0);
    send(sock_fd, oob_data, strlen(oob_data), MSG_OOB);
    send(sock_fd, normal_data, strlen(normal_data), 0);

    close(sock_fd);
    return 0;
}
