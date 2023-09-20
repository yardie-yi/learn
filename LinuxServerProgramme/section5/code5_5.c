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
        printf("param error\n");
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    int socFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socFd < 0)
    {
        perror("socket fail");
    }


    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &sockaddr.sin_addr);

    if(bind(socFd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        perror("bind fail");
    }

    if(listen(socFd, 5) < 0)
    {
        perror("listen fail");
    }

    sleep(20);

    struct sockaddr_in clientaddr;
    socklen_t clientLen = sizeof(clientaddr);
    int connFd = accept(socFd, (struct sockaddr *)&clientaddr, &clientLen);
    if (connFd < 0)
    {
        perror("accept fail");
    } else
    {
        char seeip[100];
        inet_ntop(AF_INET, &clientaddr.sin_addr, seeip, 100);
        printf("accept infomation : client ip is %s;port is %d\n", seeip, ntohs(clientaddr.sin_port));
        close(connFd);
    }
    close(socFd);
    return 0;
}

