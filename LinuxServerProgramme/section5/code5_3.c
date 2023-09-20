#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include <stdbool.h>

static bool stop = false;

static void handle_term(int sig)
{
    stop = true;
}

int main(int argc, char **argv)
{
   const char *ip = argv[1];
   int port = atoi(argv[2]);
   int backlog = atoi(argv[3]);
   signal(SIGTERM,handle_term);
   if (argc < 4)
   {
        printf("param error\n");
        return -1;
   }
   int soc = socket(AF_INET, SOCK_STREAM, 0);
   if(soc < 0)
   {
        perror("socket fail");
        printf("socket fail %s\n", strerror(errno));
   }

   struct sockaddr_in socaddr;
   socaddr.sin_family = AF_INET;
   socaddr.sin_port = htons(port);
   inet_pton(AF_INET, ip, &socaddr.sin_addr);

   int ret1 = bind(soc, (struct sockaddr *)&socaddr, sizeof(socaddr));
   if (ret1 < 0)
   {
        printf("bind fail : %s\n", strerror(errno));
   }

   int ret2 = listen(soc, backlog);
   if (ret2 < 0)
   {
        printf("listen fail ; %s\n", strerror(errno));
   }

   while(!stop)
   {
        sleep(1);
   }


   close(soc);
   return 0;
}
