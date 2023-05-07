#ifndef SOCKET_LIB_H
#define SOCKET_LIB_H

#include <sys/socket.h>

char *receive_udp_packet(int sockfd, int buflen);
int get_socket(char *port);

#endif
