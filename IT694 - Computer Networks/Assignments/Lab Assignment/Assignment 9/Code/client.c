#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <wait.h>
#include <signal.h>
#define PORT 8080

char server_ip[20] = "10.0.2.14";
char ip_address[20];
int readPipe;
int writePipe;

struct req{
    char type[1];
    char data[1024];
    char filename[20];
    int save;
};

struct nodeList{
    char nodes[5][15];
};

struct interProcComm{
    char type[1];
    char data[1024];
    struct nodeList nl;
    int childPID;
};

struct manifest{
    char chunk_IP[5][2][15];
};


void getIP(){
    system("ifconfig | grep 'inet ' | sed -n '1 p' | awk '{print $2}' > ip.txt");
    FILE * f = fopen("ip.txt","r");
    fgets(ip_address,15,f);
}

void comm(int sockfd,int pipeSend,int pipeRecv){
  
    sendIP(sockfd);
    printf("Sent my IP\n");
    int i;
    scanf("%d",&i);
    
    struct manifest man;
    memset(&man,0,sizeof(struct manifest));
    getManifestData(sockfd,&man);

    for(int i=0;i<5;i++){
        printf("filename :%s IP:%s\n",man.chunk_IP[i][0],
        man.chunk_IP[i][1]);
    }

    scanf("%d",&i);
    getChunkFiles(sockfd,&man);
   

}

void sendIP(int sockfd){
    struct req req;
  
    memset(&req,0,sizeof(struct req));
  
    req.type[0]='1';
  
    strcpy(req.data,ip_address);

    send(sockfd,&req,sizeof(struct req),0);
}


void gtValidNodes(int sockfd,int pipeSend,int pipeRecv){
    struct req req;
    struct nodeList nl;

    memset(&nl,0,sizeof(nl));
    memset(&req,0,sizeof(struct req));
    
    req.type[0]='2';
    
    send(sockfd,&req,sizeof(struct req),0);
    recv(sockfd,&nl,sizeof(struct nodeList),0);

    for(int i=0;i<5;i++){
        printf("IP [%d] : %s\n",i,nl.nodes[i]);
    }
    sendNodeListToParent(&nl,pipeSend,pipeRecv);
}

void getManifestData(int sockfd,struct manifest * man){
    struct req req;
    memset(&req,0,sizeof(struct req));

    req.type[0]='3';

    send(sockfd,&req,sizeof(struct req),0);
    recv(sockfd,man,sizeof(struct manifest),0);
}

void getChunkFiles(int sockfd,struct manifest * man){
    int count=0;
    int status;
    for(int i=0;i<5;i++){
        if(strlen(man->chunk_IP[i][1])==0)break; 
        count++;
        if((fork())==0){
            printf("trying to get file:%s\n",man->chunk_IP[i][0]);
            clientSegmentForChunk(man->chunk_IP[i][1],man->chunk_IP[i][0]);
        }
        wait(&status);
    }
}

void sendNodeListToParent(struct nodeList *nl,int pipeSend,int pipeRecv){
    kill(getppid(),SIGUSR1);
    struct interProcComm interProcComm;
    interProcComm.type[0]='1';

    for(int i=0;i<5;i++){
        strcpy(interProcComm.nl.nodes[i],nl->nodes[i]);
    }

    write(pipeSend,&interProcComm,sizeof(struct interProcComm));
    exit(0);
}

void clientSegmentForChunk(char * server_ip,char * filename){
    int sockfd;
    struct sockaddr_in address;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        printf("Error while creating socket\n");
        exit(0);
    }
    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr(server_ip);

    if((connect(sockfd,(struct sockaddr*)&address,sizeof(address)))!=0){
        printf("Connection with server failed.\n");
        exit(0);
    }
    printf("Connection with server established\n");
    

    struct req req,req1;
    memset(&req,0,sizeof(struct req));
    req.type[0]='4';

    strcpy(req.filename,filename);
    
    send(sockfd,&req,sizeof(struct req),0);

    memset(&req1,0,sizeof(struct req));
    recv(sockfd,&req1,sizeof(struct req),0);

    FILE * f = fopen("dev_saif.txt","a");
    printf("-----------\ndata from %s : %s\n---------\n",filename,req1.data);
   
    fprintf(f,"%s",req1.data);
    fclose(f);


    if(req1.save==1){
        char * temp = "test.p2p";
        FILE * chunk = fopen(temp,"w");
        fprintf(chunk,"%s",req1.data);
        fclose(chunk);
        
      
        struct req manReq;
        memset(&manReq,0,sizeof(struct req));

        manReq.type[0]='5';
        strcpy(manReq.filename,filename);
        strcpy(manReq.data,ip_address);
        printf("sent req to update manifest data\n");
        send(sockfd,&manReq,sizeof(struct req),0);
       
    }
    exit(0);
}

void clientSegment(char * server_ip,int pipeSend,int pipeRecv){
    int sockfd;
    struct sockaddr_in address;
    
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        printf("Error while creating socket\n");
        exit(0);
    }
    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr(server_ip);

    if((connect(sockfd,(struct sockaddr*)&address,sizeof(address)))!=0){
        printf("Connection with server failed.\n");
        exit(0);
    }
    printf("Connection with server established\n");
    comm(sockfd,pipeSend,pipeRecv);
}

void signalHandler(int sig){
    struct interProcComm interProcComm;
    memset(&interProcComm,0,sizeof(struct interProcComm));
    read(readPipe,&interProcComm,sizeof(struct interProcComm));

    switch(interProcComm.type[0]){
        case '1':startNewConnection(&interProcComm);break;
    }
}

void startNewConnection(struct interProcComm *interProcComm){
    strcpy(server_ip,interProcComm->nl.nodes[0]);
}

int main(){
    int pipe1[2];
    int pipe2[2];

    pipe(pipe1);
    pipe(pipe2);
    readPipe = pipe1[0];
    writePipe = pipe2[0];
    int status;
    getIP();
    printf("My IP: %s",ip_address);

    signal(SIGUSR1,signalHandler);
    while(1){
        if(fork()==0){
            printf("Connecting with server of IP: %s\n",server_ip);
            clientSegment(server_ip,pipe1[1],pipe2[0]);
        }
        wait(&status);
    }
}
