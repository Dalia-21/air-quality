/*
 * This program gets the host IP, usually 0.0.0.0, from the host
 * and prints it to stdout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

struct addrinfo *get_address_struct(struct addrinfo hints, char *port);

int main(int argc, char *argv[]) {
	int return_status;
	struct addrinfo address_hints;

	memset(&address_hints, 0, sizeof address_hints);
	address_hints.ai_family = AF_INET; // IPv4
	address_hints.ai_socktype = SOCK_DGRAM; // UDP
	address_hints.ai_flags = AI_PASSIVE; // Accept the host's IP address
																			 
	struct addrinfo *server_info = get_address_struct(address_hints, "2345");

	char ipstr[INET_ADDRSTRLEN];
	struct sockaddr_in *sa = (struct sockaddr_in *)server_info->ai_addr;
	inet_ntop(AF_INET, &(sa->sin_addr), ipstr, INET_ADDRSTRLEN);

	printf("IP address: %s\n", ipstr);


	return 0;
}

struct addrinfo *get_address_struct(struct addrinfo hints, char *port) {
	struct addrinfo *server_info;
	int return_status = getaddrinfo(NULL, port, &hints, &server_info);
	if (return_status) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(return_status));
		exit(EXIT_FAILURE);
	}
	return server_info;
}
