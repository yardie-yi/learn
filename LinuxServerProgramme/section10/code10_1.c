#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<pthread.h>
#include <stdbool.h>
#define MAX_EVENT_NUMBER 1024
static int pipefd[2];

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int socfd)
{
    struct epoll_event event;
    event.data.fd = socfd;
    event.events = EPOLLIN | EPOLLET;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, socfd, &event);
    setnonblocking(socfd);
}

void sig_handler(int sig)
{
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

void addsig(int sig)
{
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    sigfillset(&act.sa_mask);
    act.sa_handler = sig_handler;
    act.sa_flags |= SA_RESTART;
    int ret = sigaction(sig, &act, NULL);
    assert(ret != -1);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("param error");
        return -1;
    }

    int port = atoi(argv[2]);
    char *ip = argv[1];

    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (soc_fd < 0)
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
    listen(soc_fd, 5);
    int ret;
    struct epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);

    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
    assert(ret !=- 1);
    setnonblocking(pipefd[1]);
    addsig(SIGHUP);
    addsig(SIGCHLD);
    addsig(SIGTERM);
    addsig(SIGINT);

    bool stop_server = false;
    while(stop_server)
    {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR))
        {
            printf("epoll wait fail\n");
            break;
        }
        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if (sockfd == soc_fd)
            {
                struct sockaddr_in acc_addr;
                socklen_t acc_addr_len = sizeof(acc_addr);
                int acc_fd = accept(soc_fd, (struct sockaddr *)&acc_addr, &acc_addr_len);
                addfd(epollfd, acc_fd);
            }
            else if (sockfd == pipefd[0] && events[i].events & EPOLLIN)
            {
                int sig;
                char signals[1024];
                ret = recv(pipefd[0], signals, sizeof(signals), 0);
                if (ret == -1)
                {
                    continue;
                }
                else if (ret == 0)
                {
                    continue;
                }
                else
                {
                    for(int i = 0; i < ret; i++)
                    {
                        switch(signals[i])
                        {
                            case SIGCHLD:
                            case SIGHUP:
                            {
                                continue;
                            }
                            case SIGINT:
                            {
                                stop_server = true;
                            }
                        }
                    }
                }
            }
            else
            {
            }
        }
    }
    printf("close fds\n");
    close(soc_fd);
    close(pipefd[1]);
    close(pipefd[0]);
    return 0;

}
