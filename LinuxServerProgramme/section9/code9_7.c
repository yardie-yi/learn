#define_GNU_SOURCE 1
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
#include<poll.h>
#define USER_LIMIT 5/*最大用户数量*/
#define BUFFER_SIZE 64/*读缓冲区的大小*/
#define FD_LIMIT 65535/*文件描述符数量限制*/

struct client_data
{
    sockaddr_in address;
    char *write_buf;
    char buf[BUFFER_SIZE];
};

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

int main(int argc, char **argv)
{
    int ret;
    if (argc < 3)
    {
        printf("param error!\n");
        return -1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[0]);
    int soc_fd = socket(AF_TNET, SOCK_STREAM, 0);
    struct sockaddr_in soc_address;
    soc_address.sin_family = AF_INET;
    soc_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &soc_address.sin_addr);
    if(bind(soc_fd, (struct sockaddr *)&soc_address, sizeof(soc_address)) < 0)
    {
        close(soc_fd);
        perror("bind fail");
        return -1;
    }
    listen(soc_fd, 5);
    client_data *users = new client_data[FD_LIMIT];
    pollfd fds[USER_LIMIT + 1];
    int user_counter = 0;
    for(int i = 1; i <= USER_LIMIT; ++i)
    {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[0].fd = soc_fd;
    fds[0].events = POLLIN|POLLERR;
    fds[0].revents = 0;
    while(1)
    {
        ret = poll(fds, user_counter + 1, -1);
        if(ret < 0)
        {
            printf("poll fail\n");
            break;
        }
        for(int i = 0; i < user_counter + 1; i++)
        {
            if ((fds[i].fd == soc_fd) && (fds[i].revents & POLLIN))
            {
                struct sockaddr_in accept_address;
                socklen_t accept_address_len = sizeof(accept_address);
                int connfd = accept(soc_fd, (struct sockaddr *)&accept_address, &accept_address_len);
                if (connfd < 0)
                {
                    perror("accept fail");
                    continue;
                }
                if(user_counter > USER_LIMIT)
                {
                    const char *info = "too many users\n";
                    printf("%s\n", info);
                    send(connfd, info, strlen(info), 0);
                    close(connfd);
                    continue;
                }
                user_counter++;
                users[connfd].address = accept_address;
                setnonblocking(connfd);
                fds[user_counter].fd = connfd;
            }
        }
    }

}
