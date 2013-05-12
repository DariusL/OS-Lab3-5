/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_un02c.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

int s;

void dl_exit(int code){
	shutdown(s, SHUT_RDWR);
	close(s);
	exit(code);
}

void dl_start_client(char *name){
	struct sockaddr addr;
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if(s == -1){
		printf("Kliento paleidimas sprogo\n");
		dl_exit(1);
	}
	addr.sa_family = AF_UNIX;
	strncpy(addr.sa_data, "server", sizeof(addr.sa_data) - 1);
	if(bind(s, &addr, sizeof(addr)) != 0){
		printf("Bind sprogo\n");
		dl_exit(1);
	}
	if(connect(s, &addr, sizeof(addr)) != 0){
		printf("Connect sprogo\n");
		dl_exit(1);
	}
}

void dl_send_data(long bytes){
	void *data = malloc(bytes);
	if(send(s, data, bytes, 0) == -1){
		printf("Klaida siunciant duomenis\n");
		dl_exit(1);
	}
	free(data);
}

void dl_close_server(){
	char data = 10;
	if(send(s, &data, 1, 0) == -1){
		printf("Klaida uzdarant serveri\n");
		dl_exit(1);
	}
}

int main(){
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );
	dl_start_client("server");
	dl_send_data(50);
	dl_send_data(1024*1024);
	dl_close_server();
	dl_exit(0);
	return 0;
}
