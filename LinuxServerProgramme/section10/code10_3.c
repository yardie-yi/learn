#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<fcntl.h>
#define BUF_SIZE 1024
static int connfd;

void sig_urg(int sig)
{
    int save_errno = errno;
    char buffer[BUF_SIZE];
    memset(buffer, '\0', BUF_SIZE);
    int ret = recv(connfd, buffer, BUF_SIZE - 1, MSG_OOB);
    printf("got %d bytes of oob data '%s' \n", ret, buffer);
    errno = save_errno;
}

void sigadd(int sig, void (*sighander)(int))
{
    struct sigaction act;
    act.sa_handler = sighander;
    act.sa_flags |= SA_RESTART;
    sigfillset(&act.sa_mask);
    assert(sigaction(sig, &act, NULL) != -1);
}

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("exit\n");
        return -1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
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
    int ret = listen(soc_fd, 5);
    assert(ret != -1);

    struct sockaddr_in acc_addr;
    socklen_t acc_addr_len = sizeof(acc_addr);
    connfd = accept(soc_fd, (struct sockaddr *)&acc_addr, &acc_addr_len);
    if (connfd < 0)
    {
        perror("accept fail");
        close(soc_fd);
        return -1;
    }
    sigadd(SIGURG, sig_urg);
    fcntl(connfd, F_SETOWN, getpid());
    char buffer[BUF_SIZE];
    while(1)
    {
        memset(buffer, '\0', BUF_SIZE);
        ret =  recv(connfd, buffer, BUF_SIZE -1, 0);
        if(ret < 0)
        {
            break;
        }
        printf("got %d bytes of normal data %s \n",ret, buffer);
    }
    close(connfd);
    close(soc_fd);
    return 0;


}
