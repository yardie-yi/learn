#define _GNU_SOURCE 1
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
    struct sockaddr_in address;
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
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
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
    struct client_data *users =(struct client_data *)malloc(FD_LIMIT*sizeof(struct client_data));
    struct pollfd fds[USER_LIMIT + 1];
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
                fds[user_counter].events = POLLIN|POLLRDHUP|POLLERR;
                fds[user_counter].revents = 0;
                printf("comes a new user, now have %d users\n", user_counter);
            }
            else if (fds[i].revents & POLLERR)
            {
                printf("get an error from %d\n", fds[i].fd);
                char errors[100];
                memset(errors, '\0', 100);
                socklen_t length = sizeof(errors);
                if (getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, &errors, &length) < 0)
                {
                    printf("get socket option failed\n");
                }
                continue;
            }
            else if (fds[i].revents & POLLRDHUP)
            {
                users[fds[i].fd] = users[fds[user_counter].fd];
                close(fds[i].fd);
                fds[i] = fds[user_counter];
                i--;
                user_counter--;
                printf("a client left\n");
            }
            else if(fds[i].revents & POLLIN)
            {
                int connfd = fds[i].fd;
                memset(users[connfd].buf, '\0', BUFFER_SIZE);
                ret = recv(connfd, users[connfd].buf, BUFFER_SIZE - 1, 0);
                if (ret < 0)
                {
                    if (errno != EAGAIN)
                    {
                        close(connfd);
                        users[fds[i].fd] = users[fds[user_counter].fd];
                        fds[i] = fds[user_counter];
                        i--;
                        user_counter--;
                    }
                }
                else if (ret == 0)
                {

                }
                else
                {
                    for (int j = 1; j <= user_counter; j++)
                    {
                        if (fds[j].fd == connfd)
                        {
                            continue;
                        }
                        fds[j].events |= ~POLLIN;
                        fds[j].events |= POLLOUT;
                        users[fds[j].fd].write_buf = users[connfd].buf;
                    }
                }
            }
            else if (fds[i].revents & POLLOUT)
            {
                int connfd = fds[i].fd;
                if (!users[connfd].write_buf)
                {
                    continue;
                }
                ret = send(connfd, users[connfd].write_buf, strlen(users[connfd].write_buf),0);
                users[connfd].write_buf = NULL;
                fds[i].events |= ~POLLOUT;
                fds[i].events |= POLLIN;
            }
        }
    }
    free(users);
    close(soc_fd);
    return 0;
}
