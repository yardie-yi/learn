#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("param error!\n");
        return -1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in soc_addr;
    soc_addr.sin_family = AF_INET;
    soc_addr.sin_port = htons(port);
    inet_pton(AF_INET, (char *)ip, (void *)&soc_addr.sin_addr);

    if(bind(soc_fd, (struct sockaddr *)&soc_addr, sizeof(soc_addr)) < 0)
    {
        perror("bind fail");
        close(soc_fd);
        return -1;
    }

    if (listen(soc_fd, 5) < 0)
    {
        perror("liseten fail");
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

    int ret = 0;
    fd_set read_fd;
    fd_set except_fd;
    FD_ZERO(&read_fd);
    FD_ZERO(&except_fd);
    char buf[1024];
    while(1)
    {
        memset(buf, '\0', sizeof(buf));
        FD_SET(acc_fd, &read_fd);
        FD_SET(acc_fd, &except_fd);
        if(select(acc_fd + 1, &read_fd, NULL, &except_fd, NULL) < 0)
        {
            perror("select fail");
            ret = -1;
            break;
        }
        if(FD_ISSET(acc_fd, &read_fd))
        {
            int recv_ret;
            recv_ret = recv(acc_fd, buf, sizeof(buf) - 1, 0);
            if(recv_ret <= 0)
                break;
            printf("get %d bytes in normal dat %s \n", recv_ret, buf);
        }

        if(FD_ISSET(acc_fd, &except_fd))
        {
            int recv_ret;
            recv_ret = recv(acc_fd, buf, sizeof(buf) - 1, MSG_OOB);
            if(recv_ret <= 0)
                break;
            printf("get %d bytes in MSG_OOB dat %s \n", recv_ret, buf);
        }
    }


    close(acc_fd);
    close(soc_fd);
    return ret;
}
