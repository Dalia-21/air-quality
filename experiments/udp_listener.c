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
#include <unistd.h>

#define PORT_NO "2345"
#define MAXBUFLEN 100

struct addrinfo get_udp_hints_struct();
struct addrinfo *get_local_address_struct(struct addrinfo hints, char *port);
void check_for_error(int status, char *msg);
uint16_t get_port_number(struct sockaddr *s);
int get_available_socket(struct addrinfo *a);
char *receive_udp_packet(int sockfd);
int get_socket(char *port);

int main(int argc, char *argv[]) {
	char *buf;
	int sockfd = get_socket(PORT_NO);

	buf = receive_udp_packet(sockfd);

	printf("Received message: %s\n", buf);

	close(sockfd);
	free(buf);

	return 0;
}

int get_socket(char *port) {
	struct addrinfo hints = get_udp_hints_struct();
	struct addrinfo *server_info = get_local_address_struct(hints, port);
	int sockfd = get_available_socket(server_info);

	int yes = 1;
	int return_status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	check_for_error(return_status, "setsockopt failed");

	freeaddrinfo(server_info); // Info not required once socket is bound
	
	return sockfd;
}

struct addrinfo get_udp_hints_struct() {
	struct addrinfo address_hints;
	memset(&address_hints, 0, sizeof address_hints);
	address_hints.ai_family = AF_INET; // IPv4
	address_hints.ai_socktype = SOCK_DGRAM; // UDP
	address_hints.ai_flags = AI_PASSIVE; // Accept the host's IP address
	return address_hints;
}

struct addrinfo *get_local_address_struct(struct addrinfo hints, char *port) {
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

int get_available_socket(struct addrinfo *a) {
	// Iterate through the addrinfo linked list until a valid socket is bound
	int sockfd;
	for (; a != NULL; a = a->ai_next) {
		if ((sockfd = socket(a->ai_family, a->ai_socktype, a->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, a->ai_addr, a->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break; // If code reaches this point, binding was successful
	}

	if (a == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		exit(EXIT_FAILURE);
	}

	return sockfd;
}

char *receive_udp_packet(int sockfd) {
	int numbytes;
	struct sockaddr_storage sender_addr;
	char *buf = malloc(MAXBUFLEN);
	socklen_t addr_len;

	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
					(struct sockaddr *)&sender_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(EXIT_FAILURE);
	}

	buf[numbytes] = '\0';

	return buf;
}

void check_for_error(int status, char *msg) {
	if (status) {
		perror(msg);
		exit(EXIT_FAILURE);
	}
}
