#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};
void pv(int sem_id, int op)
{
    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_op = op;
    sop.sem_flg = SEM_UNDO;
    semop(sem_id, &sop, 1);
}


int main(int argc, char *argv)
{
    int sem_id = semget(IPC_PRIVATE, 1, 0666);
    if(sem_id  == -1)
    {
        perror("semget fail");
        return -1;
    }

    union semun su;
    su.val = 1;

    int ret = semctl(sem_id, 0, SETVAL, su);
    if (ret == -1)
    {
        perror("semctl fail");
        return -1;
    }
    
    pid_t pid = fork();
    if(pid < 0)
    {
        printf("fork fail\n");
        semctl(sem_id, 0, IPC_RMID, su);
        return -1;
    }
    if(pid == 0)
    {
        printf("child try to get binary sem\n");
        pv(sem_id, -1);
        printf("child get the sem and would release it after 5 seconds\n");
        sleep(5);
        pv(sem_id, 1);
        exit(0);
    }
    else
    {
        printf("parant try to get binary sem\n");
        pv(sem_id, -1);
        printf("parant get the sem and sould release it after 5 seconds\n");
        sleep(5);
        pv(sem_id, 1);
    }
    waitpid(pid, NULL, 0);
    semctl(sem_id, 0, IPC_RMID, su);
    return 0;
}
