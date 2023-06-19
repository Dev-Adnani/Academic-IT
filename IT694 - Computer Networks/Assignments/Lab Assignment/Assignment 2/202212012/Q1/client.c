#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

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


int noOfPck(int size){
	int req = size/100;
	double reqD = size/(double)100;
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


int shakeHand(int sock_fd,struct sockaddr_in server_addr,int sock_length,int size,
	struct control_pkt* c_pkt){

	int pkts = noOfPck(size);

	printf("\nPackets in handshake: %d\n",pkts);

	c_pkt->end_sr_no = pkts;
	c_pkt->sr_no = 0;
	c_pkt->no_of_pkt = pkts;
	c_pkt->start_sr_no=1;
	c_pkt->checksum = createSumControl(c_pkt);

	struct ack_pkt ackPkt;
	struct ack_pkt * ack = &ackPkt;

	ack->ack=-1;

	
	while(ack->ack != 1){
		if(sendto(sock_fd,c_pkt,sizeof(*c_pkt),0,
			(struct sockaddr *)&server_addr,sock_length)<0){
				printf("\nError while sending message to server.\n");
				return -1;
		}
		printf("\nSent handshake.");

		struct timeval t;
		t.tv_sec=5;
		fd_set socks;
		FD_ZERO(&socks);
		FD_SET(sock_fd,&socks);

		if(select(sock_fd+1,&socks,NULL,NULL,&t) && 
			recvfrom(sock_fd,ack,sizeof(*ack),0,
			(struct sockaddr *)&server_addr,&sock_length)<0){
				printf("\nError in delay recv function.");
		}

	}
	return 1;

}


int comunicate(int sock_fd,struct sockaddr_in server_addr,int sock_length){
	struct data_pkt dataSend;
	struct data_pkt * data1 = &dataSend;
	memset(data1->msg,0,100);

	printf("\nEnter msg: ");

	fgets(data1->msg,100,stdin);
	data1->sr_no=1;

	data1->checkSum = createSum(data1);

	
	if(sendto(sock_fd,data1,sizeof(*data1),0,
		(struct sockaddr *)&server_addr,sock_length)<0){
			printf("\nError while sending message to server.");
			return -1;
	}
	printf("\nSent: %s",data1->msg);

	
	struct ack_pkt dataRecv;
	struct ack_pkt * ack = &dataRecv;
	ack->ack=-1;

	struct timeval t;
	t.tv_sec=5;
	fd_set socks;
	FD_ZERO(&socks);
	FD_SET(sock_fd,&socks);

	if(select(sock_fd+1,&socks,NULL,NULL,&t) && 
		recvfrom(sock_fd,ack,sizeof(*ack),0,
		(struct sockaddr *)&server_addr,&sock_length)<0){
			printf("\nError in delay recv function.");
	
	}

	if(ack->ack==-1){
		printf("\nNo ack recved");
	}
	else{
		printf("\nAck recved");
	}


	return 1;
}

int comunicatePkt(int sock_fd,struct sockaddr_in server_addr,int sock_length,char * msg,int sr_no){
	struct data_pkt dataSend;
	struct data_pkt * data = &dataSend;
	memset(data->msg,0,100);

	struct ack_pkt ackPkt;
	struct ack_pkt * ack = &ackPkt;

	ack->ack=-1;

	int i=0;
	while(*msg != '\0'){
		data->msg[i]=*msg;
		msg++;i++;
	}

	data->sr_no = sr_no;
	data->checkSum = createSum(data);

	if(sendto(sock_fd,data,sizeof(*data),0,
		(struct sockaddr *)&server_addr,sock_length)<0){
			printf("\nError while sending message to server");
			return -1;
	}

	printf("\nSent packet %d : %s ",sr_no,data->msg);
	printf("\nchecksum: %ld",data->checkSum);

	struct timeval t;
	t.tv_sec=5;
	fd_set socks;
	FD_ZERO(&socks);
	FD_SET(sock_fd,&socks);
	if(select(sock_fd+1,&socks,NULL,NULL,&t) && 
		recvfrom(sock_fd,ack,sizeof(*ack),0,
		(struct sockaddr *)&server_addr,&sock_length)<0){
			printf("\nError in delay recv function.");
	}
	printf("\nack: %d",ack->ack);
	return ack->ack;

}

int main(){
	int sock_fd;
	struct sockaddr_in server_addr;
	int sock_length = sizeof(server_addr);

	sock_fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	if(sock_fd < 0){
		printf("\nError while creating socket.");
		return -1;
	}
	printf("\nSocket created.");

	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(PORT);
	server_addr.sin_family = AF_INET;

	char msg[200];
	int size = sizeof(msg)/sizeof(msg[0]);

	struct control_pkt pkt;

	int handshakeResult = shakeHand(sock_fd,server_addr,sock_length,size,&pkt);
	while(handshakeResult==-1){
		handshakeResult=shakeHand(sock_fd,server_addr,sock_length,size,&pkt);
	}

	printf("\nSuccefull handshake\n");
	printf("\nNo of packets: %d.",pkt.no_of_pkt);

	char *pkt_datas[2];
	char * f = "hello";
	char * s = "world";
	printf("here");
	pkt_datas[0]=f;
	pkt_datas[1]=s;

	
	for(int i=0;i<pkt.no_of_pkt;i++){
		int result = comunicatePkt(sock_fd,server_addr,sock_length,pkt_datas[i],i);
		while(result == -1){
			result = comunicatePkt(sock_fd,server_addr,sock_length,pkt_datas[i],i);
		}
	}

	return 0;
}