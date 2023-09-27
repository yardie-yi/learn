#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<unistd.h>
#include<assert.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("param error\n");
        return -1;
    }

    char *host = argv[1];
    struct hostent *hostinfo;
    hostinfo = gethostbyname(host);
    if(hostinfo == NULL)
    {
        printf("gethostbyname fail\n");
        return -1;
    }
    struct servent *servinfo;
    servinfo = getservbyname("daytime","tcp");
    if(servinfo == NULL)
    {
        printf("getservbyname fail\n");
        return -1;
    }
    printf("daytime port is%d\n",ntohs(servinfo->s_port));
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_fd < 0)
    {
        perror("socket fail");
        return -1;
    }

    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = servinfo->s_port;
    client_addr.sin_addr = *((struct in_addr *)*(hostinfo->h_addr_list));
    if(connect(soc_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
    {
        perror("connect fail");
        close(soc_fd);
        return -1;
    }
    char buf[128];
    int ret = read(soc_fd, buf, sizeof(buf));
    if(ret < 0)
    {
        perror("read fail");
        close(soc_fd);
        return -1;
    }
    buf[ret] = '\0';
    printf("the day tiem is:%s",buf);
    close(soc_fd);
    return 0;

}
