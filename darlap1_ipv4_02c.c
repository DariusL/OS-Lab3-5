/* Kestutis Paulikas KTK kespaul */
/* Failas: darlap1_ipv4_02c.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#define BUFLEN 32768
int dl_ipv4_client( const char *host, int port, const char *msg ){
	int s, rv, n;
	struct sockaddr_in sa;
	char buf[BUFLEN];
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if( s == -1 ){
		perror( "socket failed" );
		exit( 1 );
	}
	memset( &sa, 0, sizeof( sa ) );
	sa.sin_family = AF_INET;
	sa.sin_port = htons( port );
	rv = inet_pton( AF_INET, host, &sa.sin_addr );
	if( rv != 1 ){
		if( rv == -1 ){
			perror( "inet_pton error" );
			exit( 1 );
		}
		puts( "inet_pton wrong address" );
		exit( 1 );
	}

	rv = connect( s, (struct sockaddr *)&sa, sizeof( sa ) );
	if( rv != 0 ){
		perror( "connect failed" );
		exit( 1 );
	}
	n = strlen( msg );
	/* rv = write( s, msg, n );*/
	rv = send( s, msg, n, 0 );
	if( rv != n ){
		if( rv == -1 ){
			perror( "send failed" );
			exit( 1 );
		}
		puts( "send error" );
		exit( 1 );
	}
	/* rv = read ( s, buf, sizeof(buf) - 1 );*/
	rv = recv ( s, buf, sizeof(buf) - 1, 0 );
	if( rv == -1 ){
		perror( "recv error" );
		exit( 1 );
	}
	buf[rv] = '\0';
	buf[BUFLEN - 1] = '\0';
	puts( buf );
	shutdown( s, SHUT_RDWR );
	close( s );
	return 0;
}
int main( int argc, char * argv[] ){
	int p;
	const char *host = "127.0.0.1";
	printf( "(C) 2013 Darius Lapunas, %s\n", __FILE__ );
	if( argc > 1 ) host = argv[1];
	p = 15679;
	dl_ipv4_client( host, p, "GET / HTTP/1.0\r\n\r\n" );
	return 0;
}
