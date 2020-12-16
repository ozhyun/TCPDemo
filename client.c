#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include "msg.h"

static void usage(const char *proc)
{
    printf("please use : %s [ip] [port]\n",proc);
}




int main(int argc,char *argv[])
{
    if( argc != 3 ){
        usage(argv[0]);
        exit(1);
    }
    
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        perror("socket error");
        return 1;
    }

    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(atoi(argv[2]));
    remote.sin_addr.s_addr = inet_addr(argv[1]);


    int ret = connect(sock,(struct sockaddr*)&remote,sizeof(remote));
    if(ret < 0){
        printf("connect failed:%s\n",strerror(errno));
        return 2;
    }
    
    printf("connect success!\n");

	/**
    char buf[1024];
    
    while(1){
        memset(buf,'\0',sizeof(buf));
        printf("please enter:");
        fflush(stdout);
        ssize_t _s = read(0,buf,sizeof(buf)-1);
        if(_s > 0){
            buf[_s - 1] = '\0';
            write(sock,buf,strlen(buf));
            _s = read(sock,buf,sizeof(buf)-1);
            if(_s > 0){
                if(strncasecmp(buf,"quit",4) == 0){
                     printf("qiut\n");
                     break;
                 }
                buf[_s -1] = '\0';
                printf("%s\n",buf);
            }
        }
    }
	*/

	struct message *msg;
	struct timespec  ts, te;
	char *data;

	data = malloc(BUF_LEN); // 40M
	if (data == NULL) {
		printf("malloc failed\n");
		return -1;
	}
	
	// ping
	msg = (struct message*)data;
	msg->type = MSG_PING;
	msg->length = sizeof(struct message);
	clock_gettime(CLOCK_MONOTONIC, &ts);
	send_msg(sock, msg);
	read_msg(sock, data, BUF_LEN);
	clock_gettime(CLOCK_MONOTONIC, &te);
	printf("Latency: %d s %ld ns\n", te.tv_sec - ts.tv_sec, te.tv_nsec - ts.tv_nsec);

	// upload
	msg = (struct message*)data;
	msg->type = MSG_DATA;
	msg->length = BUF_LEN;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	send_msg(sock, msg);
	read_msg(sock, data, BUF_LEN);
	clock_gettime(CLOCK_MONOTONIC, &te);
	printf("Upload: %d s %ld ns\n", te.tv_sec - ts.tv_sec, te.tv_nsec - ts.tv_nsec);

	// download
	msg = (struct message*)data;
	msg->type = MSG_REQ;
	msg->length = sizeof(struct message);
	clock_gettime(CLOCK_MONOTONIC, &ts);
	send_msg(sock, msg);
	read_msg(sock, data, BUF_LEN);
	clock_gettime(CLOCK_MONOTONIC, &te);
	printf("Download: %d s %ld ns\n", te.tv_sec - ts.tv_sec, te.tv_nsec - ts.tv_nsec);


    close(sock);
    return 0;
}


