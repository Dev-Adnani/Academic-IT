#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int sp1[2];
    int sp2[2];

    int cp1[2];
    int cp2[2];

 
    pipe(sp1);
    pipe(sp2);
   
    pipe(cp1);
    pipe(cp2);

    if(fork()==0){
        char spipe1[5];
        char spipe2[5];

   
        sprintf(spipe1,"%d",sp1[1]);
        sprintf(spipe2,"%d",sp2[0]);

    
        execl("./server.out","server.out",spipe1,spipe2,NULL);
        printf("Failed to start server child process\n");
        exit(1);
    }
    int t;
    printf("Do you want to start client?");
    scanf("%d",&t);
    if(fork()==0){
        char cpipe1[5];
        char cpipe2[5];

        sprintf(cpipe1,"%d",cp1[1]);
        sprintf(cpipe2,"%d",cp2[0]);

        char *arguments[]={"./client.out",cpipe1,cpipe2,NULL};

      
        execl("./client.out","client.out",cpipe1,cpipe2,NULL);
        printf("Failed to start client child process\n");
        exit(1);
    }
    int statusServer,statusClient;
    pid_t pid1,pid2;
    pid1 = wait(&statusServer);
    pid2 = wait(&statusClient);
}