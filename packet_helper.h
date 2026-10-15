#ifndef PACKET_HELPER_H
#define PACKET_HELPER_H

#include <stddef.h>
#define MAX_MESSAGE 4096


int send_packet(int socket, const void *buf, size_t len, int flags);

int receive_packet(int socket, void *buf, size_t len, int flags);

int send_package(int socket, const char *load);

int receive_package(int socket, char *buf, size_t buf_len);


#endif