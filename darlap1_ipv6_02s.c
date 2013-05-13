/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_ipv6_02s.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

static const int bufLength = 1024;
static const int port = 15680;
const char address[] = "::1";

int s;

void dl_exit(int code){
	shutdown(s, SHUT_RDWR);
	close(s);
	exit(code);
}

void dl_start_server(){
	struct sockaddr_in6 sa;
	s = socket(AF_INET6, SOCK_STREAM, 0);
	if(s == -1){
		printf("Serveris sprogo\n");
		dl_exit(1);
	}

	sa.sin6_family = AF_INET6;
	sa.sin6_port = htons(port);
	if(inet_pton(AF_INET6, address, &sa.sin6_addr) == -1){
		perror("IP pervedimas i skaiciu sprogo\n");
		dl_exit(1);
	}
	if(bind(s, (const struct sockaddr*)&sa, sizeof(sa)) != 0){
		perror("Bindinant sprogo\n");
		dl_exit(1);
	}
	if(listen(s, 20) == -1){
		printf("listen sprogo\n");
		dl_exit(1);
	}
}

void dl_receive(){
	char *buf = malloc(bufLength);
	int packetSize;
	int desc;
	struct sockaddr clientAddress;
	socklen_t length = sizeof(clientAddress);
	desc = accept(s, &clientAddress, &length);
	if(desc == -1){
		perror("Nepavyko priimti kliento\n");
		dl_exit(1);
	}
	packetSize = recv(desc, buf, bufLength, 0);
	if(packetSize == -1){
		perror("Nepavyko priimti duomenu\n");
		dl_exit(1);
	}
	printf("Gauti %d baitu: %s\n", packetSize, buf);
	if(send(s, "gavau", strlen("gavau"), 0) == -1){
		perror("Nepavyko atsakyti\n");
		dl_exit(1);
	}
}

int main(){
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );
	dl_start_server();
	dl_receive();	
	dl_exit(0);
	return 0;
}
