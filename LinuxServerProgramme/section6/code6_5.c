#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("param error\n");
        return -1;
    }
    int fd = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0666);
    assert(fd > 0);
    int pipefd_stdout[2];
    int ret = pipe(pipefd_stdout);
    assert(ret != -1);
    int pipefd_stdin[2];
    ret = pipe(pipefd_stdin);
    assert(ret != -1);

    ret = splice(STDIN_FILENO, NULL, pipefd_stdout[1], NULL, 32768, SPLICE_F_NONBLOCK);
    assert(ret != -1);
    ret = tee(pipefd_stdout[0], pipefd_stdin[1], 32768, SPLICE_F_NONBLOCK);
    assert(ret != -1);
    ret = splice(pipefd_stdin[0], NULL, fd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_NONBLOCK);
    assert(ret != -1);
    ret = splice(pipefd_stdout[0], NULL, STDOUT_FILENO, NULL , 32768, SPLICE_F_MORE | SPLICE_F_NONBLOCK);
    assert(ret != -1);
    close(fd);
    close(pipefd_stdout[0]);
    close(pipefd_stdout[1]);
    close(pipefd_stdin[0]);
    close(pipefd_stdin[1]);
    return 0;

}
