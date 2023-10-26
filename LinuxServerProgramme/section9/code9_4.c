#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <stdbool.h>
#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024

struct fds
{
    int epollfd;
    int sockfd;
};

void reset_oneshot(int epollfd, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return new_option;
}

void addfd(int epollfd, int fd, bool oneshot)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(oneshot)
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

void *worker(void *arg)
{
    struct fds *all_fd = (struct fds *)arg;
    printf("start new thread ro receive data on fd:%d\n", all_fd->sockfd);
    char buf[BUFFER_SIZE];
    memset(buf, '\0', BUFFER_SIZE);
    while(1)
    {
        int ret = recv(all_fd->sockfd, buf, BUFFER_SIZE - 1, 0);
        if(ret == 0)
        {
            close(all_fd->sockfd);
            printf("foreiner closed the connection\n");
            break;
        }
        else if (ret < 0)
        {
            if (errno == EAGAIN)
            {
                reset_oneshot(all_fd->epollfd, all_fd->sockfd);
                printf("read data latter");
                break;
            }
        }
        else
        {
            printf("get content : %s\n", buf);
            sleep(5);
        }
    }
    printf("end thread");
}

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("param error!\n");
        exit(0);
    }
    int port = atoi(argv[2]);
    char *ip = argv[1];

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    struct sockaddr_in listen_addr;
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, (void *)&listen_addr.sin_addr);

    if(bind(listenfd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
    {
        perror("bind fail");
        close(listenfd);
        exit(-1);
    }

    if(listen(listenfd, 5) < 0)
    {
        perror("listen fail");
        close(listenfd);
        exit(-1);
    }

    int epollfd = epoll_create(5);
    addfd(epollfd, listenfd, false);

    struct epoll_event event[MAX_EVENT_NUMBER];


    while(1)
    {
        int ret = epoll_wait(epollfd, event, MAX_EVENT_NUMBER, -1);
        if(ret < 0)
        {
            printf("epoll fail!\n");
            break;
        }
        for (int i = 0; i < ret; i++)
        {
            int sockfd = event[i].data.fd;
            if(sockfd == listenfd)
            {
                struct sockaddr_in acc_addr;
                socklen_t acc_addr_len = sizeof(acc_addr);
                int acc_fd = accept(listenfd, (struct sockaddr *)&acc_addr, &acc_addr_len);
                addfd(epollfd, acc_fd, true);
            }
            else if (event[i].events&EPOLLIN)
            {
                struct fds thwork;
                thwork.epollfd = epollfd;
                thwork.sockfd = sockfd;
                pthread_t thread;
                pthread_create(&thread, NULL, worker, (void *)&thwork);
            }
            else
            {
                printf("someting else happened\n");
            }
        }
    }
    close(listenfd);
    exit(0);
}
