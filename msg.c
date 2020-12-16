#include "msg.h"
#include <stdio.h>
#include <unistd.h>

int read_msg(int sock, char *data, int len)
{
	ssize_t _s, want, rlen;
	char  *ptr;
	struct message *msg;

	ptr = data;
	want = sizeof(struct message);
	rlen = 0;
	
	while(want > 0) {
		_s = read(sock, ptr, want);
		if (_s > 0) {
			rlen += _s;
			ptr += _s;
			want -= _s;
		}
	}
	msg = (struct message*)data;
	printf("Got msg %d with len %d\n", msg->type, msg->length);

	want = msg->length - rlen;
	while(want > 0) {
		_s = read(sock, ptr, want);
		if (_s > 0) {
			rlen += _s;
			ptr += _s;
			want -= _s;
		}
	}

	printf("Got all data of msg %d with len %d\n", msg->type, msg->length);

	return msg->length;
}

int send_msg(int sock, struct message *msg)
{
	ssize_t _s;
	ssize_t want = msg->length;
	char *ptr = (char *)msg;

	printf("Send msg type %d with len %d\n", msg->type, msg->length);

	while(want > 0) {
		_s = write(sock, ptr, want);
		if (_s > 0) {
			want -= _s;
			ptr += _s;
		}
	}

	return 0;
}

