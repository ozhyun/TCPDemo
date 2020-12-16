#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include <arpa/inet.h>
#include<pthread.h>
#include <stdlib.h>
#include "msg.h"

static void usage(const char *proc)
{
    printf("Please use :%s [port]\n",proc);
}

void *thread_run(void *arg)
{
    printf("creat a new thread\n");
    int fd = (int)arg;
    char buf[1024];

    while(1){
        memset(buf,'\0',sizeof(buf));
        ssize_t _s = read(fd,buf,sizeof(buf) - 1);
        if(_s > 0){
            buf[_s] = '\0';
            printf("client say : %s\n",buf);
        }    
        memset(buf,'\0',sizeof(buf));
        printf("please Enter: ");
        fflush(stdout);
        ssize_t _s2 = read(0,buf,sizeof(buf) - 1);
        if(_s2 > 0){
            write(fd,buf,strlen(buf));
        }
    }
}


void * thread_msg(void *arg)
{
    printf("creat a new thread\n");
    int fd = (int)arg;
	struct message *msg;
	char *data;

	data = malloc(BUF_LEN);
	if (data == NULL) {
		printf("malloc failed\n");
		return;
	}

    while(1){
		read_msg(fd, data, BUF_LEN);
		msg = (struct message*)data;

		printf("Get msg type %d len %d\n", msg->type, msg->length);


		if (msg->type == MSG_REQ) {
			msg->type = MSG_DATA;
			msg->length = BUF_LEN;
			send_msg(fd, msg);
			printf("Send msg type %d len %d\n", msg->type, msg->length);
		}
		else if(msg->type == MSG_PING || msg->type == MSG_DATA) {
				msg->type += 1;
				msg->length = sizeof(struct message);
				send_msg(fd, msg);
		}

    }

	free(data);
}

int main(int argc,char *argv[])
{
    if(argc != 2){
        usage(argv[0]);
        exit(1);
    }


    int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock < 0){
        perror("creat socket error\n");
        return 1;
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[1]));
    local.sin_addr.s_addr = htonl(INADDR_ANY);


    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
        perror("bind error\n");
        close(sock);
        return 2;
    }
   

    if(listen(sock,10) < 0){
        perror("listen error\n");
        close(sock);
        return 3;
    }

    printf("bind and listen success!wait accept...\n");


    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    while(1){

        int fd = accept(sock,(struct sockaddr*)&peer ,&len);

        if(fd < 0){
            perror("accept error\n");
            close(sock);
            return 4;
        }
        
        printf("get connect,ip is : %s port is : %d\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));

        pthread_t id;
        pthread_create(&id,NULL,thread_msg,(void*)fd);

        pthread_detach(id);

    }
    close(sock);
    return 0;
}
