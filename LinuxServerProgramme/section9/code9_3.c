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
#include<sys/epoll.h>
#include<pthread.h>

#include <stdbool.h>
#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

int setnonblocking(const int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(const int epollfd, const int fd, bool enable_et)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    if(enable_et)
    {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

void lt(struct epoll_event *events, int number, int epollfd, int listenfd)
{
    char buf[BUFFER_SIZE];
    for(int i = 0; i < number; i++)
    {
        int socfd = events[i].data.fd;
        if(socfd == listenfd)
        {
            struct sockaddr_in acc_addr;
            socklen_t acc_addr_len = sizeof(acc_addr);
            int acc_fd = accept(socfd, (struct sockaddr *)&acc_addr, &acc_addr_len);
            addfd(epollfd, acc_fd, false);
        }
        else if (events[i].events & EPOLLIN)
        {
            printf("event trigger once\n");
            memset(buf, '\0', BUFFER_SIZE);
            int ret = recv(socfd, buf, BUFFER_SIZE - 1, 0);
            if(ret < 0)
            {
                close(socfd);
                continue;
            }
            printf("get %d bytes of content : %s \n", ret, buf);
        }
        else
        {
            printf("someting else happened\n");
        }
    }
}

void et(struct epoll_event *events, int number, int epollfd, int listenfd)
{
    char buf[BUFFER_SIZE];
    for(int i = 0; i < number; i++)
    {
        int sockfd = events[i].data.fd;
        if (sockfd == listenfd)
        {
            struct sockaddr_in acc_addr;
            socklen_t acc_addr_len = sizeof(acc_addr);
            int acc_fd = accept(sockfd, (struct sockaddr *)&acc_addr, &acc_addr_len);
            addfd(epollfd, acc_fd, true);
        }
        else if(events[i].events & EPOLLIN)
        {
            printf("event triger once\n");
            while(1)
            {
                memset(buf, '\0', BUFFER_SIZE);
                int ret = recv(sockfd, buf, BUFFER_SIZE -1, 0);
                if (ret < 0)
                {
                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                    {
                        printf("read latter\n");
                        break;
                    }
                    close(sockfd);
                }
                else if (ret == 0)
                {
                    close(sockfd);
                }
                else
                {
                    printf("get%d bytes of content:%s\n",ret,buf);
                }
            }
        }
        else
        {
            printf("sometine else happened\n");
        }
    }
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("param error\n");
        return -1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_fd < 0)
    {
        perror("socket fail");
        return -1;
    }

    struct sockaddr_in soc_addr;
    soc_addr.sin_family = AF_INET;
    soc_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &soc_addr.sin_addr);

    if (bind(soc_fd, (struct sockaddr *)&soc_addr, sizeof(soc_addr)) < 0)
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

    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, soc_fd, true);
    struct epoll_event events[MAX_EVENT_NUMBER];
    while(1)
    {
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if(ret < 0)
        {
            perror("epoll_wait fail");
            break;
        }
        lt(events, ret, epollfd, soc_fd);
        //et(events, ret, epollfd, soc_fd);
    }

    close(soc_fd);
    return 0;
}
