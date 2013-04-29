/*Darius Lapunas IF-1/8 darlap1*/
/*darlap1_resolv02.c*/

#include <aio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void dl_resolve(char *name){
	struct addrinfo hints, *result, *rp;
	char *buf;
	int res, length;
	length = sysconf(_SC_HOST_NAME_MAX);
	buf = malloc(length);
	memset((void*)&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_ALL|AI_V4MAPPED;
	if(getaddrinfo(name, NULL, &hints, &result) != 0){
		printf("getaddrinfo sprogo\n");
		exit(1);
	}
	for(rp = result, rp != NULL; rp = rp->ai_next){
		res = getnameinfo(rp->ai_addr, rp->ai_addrlen, buf, length, NULL, NULL, NI_NUMERICHOST);
		if(res == 0)
			printf("IP: %s\n", buf);
	}
	freeaddrinfo(result);
}

int main(){
	printf( "(C) 2013 Lapunas Darius, %s\n", __FILE__ );
	dl_resolve("www.google.com");
	return 0;
}
