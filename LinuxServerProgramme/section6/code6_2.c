#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include <stdbool.h>
#include <sys/uio.h>
#define BUFFER_SIZE 1024

static const char* status_line[2]={"200 OK","500 Internal server error"};

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("param error!\n");
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(soc_fd < 0)
    {
        perror("socket fail!!");
        return -1;
    }


    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (bind(soc_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind fail!!");
        close(soc_fd);
        return -1;
    }

    if (listen(soc_fd, 5) < 0)
    {
        perror("listen fail!!!");
        close(soc_fd);
        return -1;
    }

    struct sockaddr_in acc_addr;
    socklen_t acc_addr_len = sizeof(acc_addr);
    int acc_fd = accept(soc_fd, (struct sockaddr *)&acc_addr, &acc_addr_len);
    if (acc_fd < 0)
    {
        perror("accept fail!!!");
        close(soc_fd);
        return -1;
    }

    //file imformation
    char handle_buf[BUFFER_SIZE];
    memset(handle_buf, '\0', BUFFER_SIZE);
    char *file_buf;
    struct stat file_stat;
    bool valid = true;
    int len = 0;
    const char *file_name = argv[3];
    if(stat(file_name, &file_stat) < 0)
    {
        valid = false;
    }
    else
    {
        if(S_ISDIR(file_stat.st_mode))
        {
            valid = false;
        }
        else if (file_stat.st_mode & S_IROTH)
        {
            int fd = open(file_name, O_RDONLY);
            file_buf =  malloc(file_stat.st_size + 1);
            memset(file_buf, '\0', file_stat.st_size + 1);
            if(read(fd, file_buf, file_stat.st_size) < 0)
            {
                valid = false;
            }
        }
        else
        {
            valid = false;
        }
    }

    int ret;
    if (valid)
    {
        ret = snprintf(handle_buf, BUFFER_SIZE - 1, "%s%s\r\n","HTTP/1.1",status_line[0]);
        len+=ret;
        ret = snprintf(handle_buf+len,BUFFER_SIZE-1-len,"Content␂Length:%ld\r\n",file_stat.st_size);
        len+=ret;
        ret=snprintf(handle_buf+len,BUFFER_SIZE-1-len,"%s","\r\n");

        struct iovec iv[2];
        iv[0].iov_base = handle_buf;
        iv[0].iov_len = strlen(handle_buf);
        iv[1].iov_base = file_buf;
        iv[1].iov_len = file_stat.st_size;
        ret = writev(acc_fd, iv, 2);
    }
    else
    {
        ret=snprintf(handle_buf, BUFFER_SIZE-1,"%s%s\r\n","HTTP/1.1",status_line[1]);
        len+=ret;
        ret=snprintf(handle_buf + len,BUFFER_SIZE-1-len,"%s","\r\n");
        send(acc_fd, handle_buf, strlen(handle_buf), 0);
    }

    close(acc_fd);
    free(file_buf);
    close(soc_fd);
}
