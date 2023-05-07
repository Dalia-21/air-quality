/*
 * This program gets a socket from the host
 * and prints the info to stdout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

struct addrinfo get_udp_hints_struct();
struct addrinfo *get_address_struct(struct addrinfo hints, char *port);
void check_for_error(int status, char *msg);
uint16_t get_port_number(struct sockaddr *s);

int main(int argc, char *argv[]) {
	int return_status;
	struct addrinfo address_hints = get_udp_hints_struct();
	int socket_fd;
	char *port = "2345";
	
	struct addrinfo *server_info = get_address_struct(address_hints, port);

	socket_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if (socket_fd == -1) {
		perror("failed to get socket");
		exit(EXIT_FAILURE);
	}

	return_status = bind(socket_fd, server_info->ai_addr, server_info->ai_addrlen);
	check_for_error(return_status, "binding to port failed");

	int yes = 1;
	return_status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	check_for_error(return_status, "setsockopt failed");

	uint16_t port_number = get_port_number(server_info->ai_addr);
	printf("Port no: %d\n", port_number);

	freeaddrinfo(server_info);

	return 0;
}

struct addrinfo get_udp_hints_struct() {
	struct addrinfo address_hints;
	memset(&address_hints, 0, sizeof address_hints);
	address_hints.ai_family = AF_INET; // IPv4
	address_hints.ai_socktype = SOCK_DGRAM; // UDP
	address_hints.ai_flags = AI_PASSIVE; // Accept the host's IP address
	return address_hints;
}

struct addrinfo *get_address_struct(struct addrinfo hints, char *port) {
	struct addrinfo *server_info;
	int return_status = getaddrinfo(NULL, port, &hints, &server_info);
	if (return_status) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(return_status));
		exit(EXIT_FAILURE);
		freeaddrinfo(server_info);
	}
	return server_info;
}

uint16_t get_port_number(struct sockaddr *s) {
	uint16_t port_number;
	struct sockaddr_in *sin = (struct sockaddr_in *)s;
	char ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(sin->sin_addr), ip, sizeof ip);
	port_number = htons(sin->sin_port);

	return port_number;
}

void check_for_error(int status, char *msg) {
	if (status) {
		perror(msg);
		exit(EXIT_FAILURE);
	}
}
