#ifndef _MSG_H_
#define _MSG_H_

enum message_type {
	MSG_PING	= 0,
	MSG_PONG	= 1,
	MSG_DATA	= 2,
	MSG_DATA_ACK	= 3,
	MSG_REQ			= 4,
	MSG_REQ_ACK		= 5,
};

struct message
{
	int type;
	int length;
	char value[0];
};

#define BUF_LEN (40 << 20)
//#define BUF_LEN (128)

int read_msg(int sock, char *data, int len);
int send_msg(int sock, struct message *msg);


#endif

