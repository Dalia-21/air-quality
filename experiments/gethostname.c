/*
 * This program is for the purpose of practicing the
 * gethostname system call.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
	// Get max hostname length
	int hostnamelength = sysconf(_SC_HOST_NAME_MAX);
	char *hostname;

	int error_status = gethostname(hostname, hostnamelength);

	if (error_status) {
		perror("Error retrieving hostname.");
		exit(EXIT_FAILURE);
	}

	printf("Hostname: %s\n", hostname);

	return 0;
}
