#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int send_packet(int sockfd, const void *buf, size_t len, int flags);

int receive_packet(int sockfd, void *buf, size_t len, int flags);


