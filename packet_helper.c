#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include "packet_helper.h"
#include <arpa/inet.h>



#define MAX_MESSAGE 4096


int send_packet(int socket, const void *buf, size_t len, int flags) {
    size_t bytes_sent_total = 0;
    size_t bytes_sent_now = 0;

    const char *buf_ptr = buf;

    while (bytes_sent_total < len) {
        bytes_sent_now = send(socket, buf_ptr + bytes_sent_total, len - bytes_sent_total, flags);
        if (bytes_sent_now == -1) {
            if (errno == EINTR) {
                continue; 
            }
            return -1;
        }
        bytes_sent_total += (size_t)bytes_sent_now;
    }
    return 0;
}


// 0 on success, -1 on failure, 1 if the connection is closed by the peer
int receive_packet(int socket, void *buf, size_t len, int flags) {
    size_t bytes_received_total = 0;
    size_t bytes_received_now = 0;
    char *buf_ptr = buf;
    while (bytes_received_total < len) {
        bytes_received_now = recv(socket, buf_ptr + bytes_received_total, len - bytes_received_total, flags);
        if (bytes_received_now == 0) {
            return 1;
        }
        if (bytes_received_now == -1) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        bytes_received_total += (size_t)bytes_received_now;
    }
    return 0;
}

int send_package(int socket, const char *load) {
    size_t len = strlen(load);
    if (len > MAX_MESSAGE) {
        return -1;
    }
    uint32_t len_net = htonl((uint32_t)len);
    if (send_packet(socket, &len_net, ))
}

int recieve_package() {

}