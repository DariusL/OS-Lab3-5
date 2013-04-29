/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_un02s.c*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

static const int bufLength = 10;

int s;

void dl_exit(int code){
	close(s);
	exit(code);
}

void dl_start_server(char *name){
	struct sockaddr_un sa;
	char *buf;
	int rv;
	buf = malloc(bufLength);
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if(s == -1){
		printf("Serveris sprogo\n");
		dl_exit(1);
	}

	sa.sun_family = AF_UNIX;
	strncpy(sa.sun_path, name, sizeof(sa.sun_path) - 1);
	if(bind(s, &sa, sizeof(sa)) != 0){
		printf("Bindinant sprogo\n");
		dl_exit(1);
	}
	printf("socket: %d, kelias: %s\n", s, sa.sun_path);
	
	rv = recvfrom(s, buf, length - 1, MSG_WAITALL, NULL, NULL);
	if(rv == -1){
		printf("recvfrom sprogo\n");
		dl_exit(1);
	}
	buf[length] = 0;
	buf[rv] = 0;
	printf("Gauta %d baitu: %s\n", rv, buf);

int main(){
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );

	return 0;
}
