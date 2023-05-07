/*
 * This program gets a socket from the host
 * and prints the info to stdout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "socketlib.h"

#define PORT_NO "2345"

int main(int argc, char *argv[]) {
	char *buf;
	int sockfd = get_socket(PORT_NO);
	int buflen = 100;

	buf = receive_udp_packet(sockfd, buflen);

	printf("Received message: %s\n", buf);

	close(sockfd);
	free(buf);

	return 0;
}

