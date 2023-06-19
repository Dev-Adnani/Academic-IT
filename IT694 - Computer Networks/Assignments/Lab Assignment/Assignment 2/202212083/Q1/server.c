#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 5100

struct control_pkt{
	int sr_no;
	int no_of_pkt;
	int start_sr_no;
	int end_sr_no;
	int checksum;
};

struct data_pkt{
	int sr_no;
	char msg[100];
	long checkSum;
};


struct ack_pkt{
	int ack;
	int sr_no;
	long checkSum;
};

void setZero(struct control_pkt * pkt){
	pkt->sr_no=0;
	pkt->no_of_pkt=0;
	pkt->checksum=0;
	pkt->end_sr_no=0;
	pkt->start_sr_no=0;
}

int noOfPck(char * msg){
	int len = strlen(msg);
	int req = len/100;
	double reqD = len/(double)100;
	if(reqD > req) req++;
	return req;
}

long createSumControl(struct control_pkt * c_pkt){
	long sum=0;
	sum+=c_pkt->end_sr_no;
	sum+=c_pkt->no_of_pkt;
	sum+=c_pkt->sr_no;
	sum+=c_pkt->start_sr_no;
	return sum;
}

int checkSumControl(struct control_pkt * c_pkt){
	long check = createSumControl(c_pkt);
	if(c_pkt->checksum == check) return 1;
	return 0;
}

long createSumACK(struct ack_pkt* ack){
	return ack->ack+ack->sr_no;
}

int checkSumACK(struct ack_pkt * ack){
	long ackSum = createSumACK(ack);
	if(ackSum == ack->checkSum) return 1;
	return 0;
}

long createSum(struct data_pkt* data){
	long msgSum=0;

	for(int i=0;i<100;i+=2){
		msgSum += ((data->msg[i]<<8) + data->msg[i+1]);
	}

	msgSum += data->sr_no;

	return msgSum;
}

int checkSum(struct data_pkt* data){
	long check = createSum(data);

	if(data->checkSum == check)return 1;
	return 0;
}	

int shakeHand(int socket_desc,struct sockaddr_in client_addr,int client_addr_size,
	struct control_pkt * c_pkt){
	
	setZero(c_pkt);

	struct ack_pkt ackPkt;
	struct ack_pkt * ack = &ackPkt;

	ack->ack=0;
	ack->sr_no=0;

	int valid = 0; 

	while(valid == 0){
		if(recvfrom(socket_desc,c_pkt,sizeof(*c_pkt),0,
			(struct sockaddr *)&client_addr,&client_addr_size)<0){
				printf("\nRecv error.\n");
				return -1;
		}
		printf("\nRecved control pkt\n");
		valid = checkSumControl(c_pkt);
		ack->ack=1;
		ack->checkSum = createSumACK(ack);
		if(sendto(socket_desc,ack,sizeof(*ack),0,
			(struct sockaddr *)&client_addr,client_addr_size)<0){
				printf("\nError while sending akc to client.");
				return -1;
		}
	}

	printf("\nSent control pkt ack.\n");
	return 1;

}

int comunicate(int socket_desc,struct sockaddr_in client_addr,int client_addr_size){
	struct data_pkt data;
	struct data_pkt * data1 = &data;
	memset(data1->msg,0,100);

	
	struct ack_pkt ackPKT;
	struct ack_pkt * ack = &ackPKT;

	
	if(recvfrom(socket_desc,data1,sizeof(*data1),0,
		(struct sockaddr *)&client_addr,&client_addr_size)<0){
			printf("\nRecv error.\n");
			return -1;
	}
	int valid = checkSum(data1);
	printf("\nMsg from client: %s",data1->msg);

	if(valid == 1){
		ack->ack=1;
		printf("\nData pkt is valid\n");
	}
	else{
		ack->ack=0;
		printf("\nData pkt is not valid\n");
	}
	ack->checkSum = createSumACK(ack);
	char c;
	scanf("%c",&c);

	if(sendto(socket_desc,ack,sizeof(*ack),0,
		(struct sockaddr *)&client_addr,client_addr_size)<0){
			printf("\nError while sending ack to client.");
			return -1;
	}


	if(sendto(socket_desc,ack,sizeof(*ack),0,
		(struct sockaddr *)&client_addr,client_addr_size)<0){
			printf("\nError while sending ack to client.");
			return -1;
	}

}

int comunicatePkt(int socket_desc,struct sockaddr_in client_addr,int client_addr_size,int sr_no){
	struct data_pkt data;
	struct data_pkt * data1 = &data;
	memset(data1->msg,0,100);

	struct ack_pkt ackPKT;
	struct ack_pkt * ack = &ackPKT;

	if(recvfrom(socket_desc,data1,sizeof(*data1),0,
		(struct sockaddr *)&client_addr,&client_addr_size)<0){
			printf("\nRecv error.\n");
			return -1;
	}
	int valid = checkSum(data1);
	printf("\nchecksum: %ld",createSum(data1));
	printf("\nMsg from client: %s",data1->msg);

	if(valid == 1){
		if(data1->sr_no != sr_no) return -1;
		ack->ack=1;
		printf("\nData pkt is valid\n");
	}
	else{
		ack->ack=-1;
		printf("\nData pkt is not valid\n");
	}
	ack->checkSum = createSumACK(ack);
	char c;
	scanf("%c",&c);


	if(sendto(socket_desc,ack,sizeof(*ack),0,
		(struct sockaddr *)&client_addr,client_addr_size)<0){
			printf("\nError while sending ack to client.");
			return -1;
	}
	return ack->ack;
}

int main(){
	int socket_desc;
	struct sockaddr_in server_addr,client_addr;
	int client_addr_size = sizeof(client_addr);

	socket_desc = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(socket_desc < 0){
		printf("\nError while creating socket.");
		return -1;
	}
	printf("\nCreated socket.");


	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//binding to port and ip
	if(bind(socket_desc,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
		printf("\nBinding error");
		return -1;
	}
	printf("\nBinding of socket.");

	printf("\nNow listening...");

	struct control_pkt pkt;


	int handshakeResult = shakeHand(socket_desc,client_addr,client_addr_size,&pkt);
	while(handshakeResult==-1){
		return -1;
	}

	printf("\nPackets : %d\n",pkt.no_of_pkt);

	for(int i=0;i<pkt.no_of_pkt;i++){
		int result = comunicatePkt(socket_desc,client_addr,client_addr_size,i);
		while(result == -1){
			result = comunicatePkt(socket_desc,client_addr,client_addr_size,i);
		}
	}
	return 1;
}