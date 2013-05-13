/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_un02s.c*/

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

static const int bufLength = 1024;

int s;

void dl_exit(int code){
	close(s);
	exit(code);
}

void dl_start_server(char *name){
	struct sockaddr sa;
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if(s == -1){
		printf("Serveris sprogo\n");
		dl_exit(1);
	}

	sa.sa_family = AF_UNIX;
	strncpy(sa.sa_data, name, sizeof(sa.sa_data) - 1);
	if(bind(s, &sa, sizeof(struct sockaddr)) != 0){
		printf("Bindinant sprogo\n");
		dl_exit(1);
	}
	if(listen(s, 20) == -1){
		printf("listen sprogo\n");
		dl_exit(1);
	}
	printf("socket: %d, kelias: %s\n", s, sa.sa_data);
}

int dl_receive(){
	char *buf = malloc(bufLength);
	int bytes = 0;
	int packetSize = 1;
	int desc;
	struct sockaddr clientAddress;
	socklen_t length = sizeof(clientAddress);
	desc = accept(s, &clientAddress, &length);

	if(desc != -1){
		while(packetSize){
			packetSize = recv(desc, buf, bufLength, MSG_WAITALL);
			if(packetSize == -1){
				printf("Klaida gaunant paketa\n");
				return 0;
			}
			printf("Gautas %d baitu paketas\n", packetSize);
			bytes += packetSize;
			if(packetSize > 0)
				if(buf[packetSize-1] == 13){
					printf("Gautas pabaigos signalas\n");
					shutdown(desc, SHUT_RDWR);
					close(desc);
					return 0;
				}
		}
	}else if(errno != EWOULDBLOCK){
		perror("Klaida priimant klienta\n");
		printf("ERRNO: %d\n", errno);
		return 0;
	}
	free(buf);
	printf("Gauta %d baitu\n", bytes);
	shutdown(desc, SHUT_RDWR);
	close(desc);
	usleep(10);
	return 1;
}	

int main(){
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );
	dl_start_server("server");
	while(dl_receive());
	unlink("server");
	dl_exit(0);
	return 0;
}
