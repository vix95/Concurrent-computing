#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define IP_ADDRESS "10.10.10.230"

int main() {
	int sd, size, bits;
	int num = 0;
   	struct sockaddr_in to;
	sd = socket(PF_INET, SOCK_DGRAM, 0);

	to.sin_family = AF_INET;
	to.sin_port = htons((ushort) 5000);
	inet_pton(PF_INET, IP_ADDRESS, &to.sin_addr);
	size = sizeof(struct sockaddr_in);

	bind(sd,(struct sockaddr*) &to, size);

	printf("Type a number:\n");
	scanf("%d", &num);

	num = htonl(num);
	sendto(sd, (char *) &num, sizeof(int), 0, (struct sockaddr *) &to, size);

	recvfrom(sd, (char *) &num, sizeof(int),0, (struct sockaddr *) &to, &size);
    num = ntohl(num);

	printf("Received: %d\n", num);
}
