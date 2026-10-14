#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include "packet_helper.h"
#include <arpa/inet.h>

#define MAX_MESSAGE 4096
#define RECV_ERR_SOCKET -1
#define RECV_ERR_CLOSED 1
#define RECV_ERR_OK 0


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
            return -RECV_ERR_SOCKET;
        }
        bytes_sent_total += (size_t)bytes_sent_now;
    }
    return RECV_ERR_OK;
}


// 0 on success, -1 on failure, 1 if the connection is closed by the peer

int receive_packet(int socket, void *buf, size_t len, int flags) {
    size_t bytes_received_total = 0;
    size_t bytes_received_now = 0;
    char *buf_ptr = buf;
    // loop to account for less than requested bytes received
    while (bytes_received_total < len) {
        bytes_received_now = recv(socket, buf_ptr + bytes_received_total, len - bytes_received_total, flags);
        if (bytes_received_now == 0) {
            return RECV_ERR_CLOSED;
        }
        if (bytes_received_now == -1) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        bytes_received_total += (size_t)bytes_received_now;
    }
    return RECV_ERR_OK;
}


// output: 0 on success, -1 on failure
int send_package(int socket, const char *load) {
    size_t len = strlen(load);
    if (len > MAX_MESSAGE) {
        return -1;
    }
    uint32_t len_net = htonl((uint32_t)len);
    if (send_packet(socket, &len_net, sizeof(len_net), MSG_NOSIGNAL) == -1) {
        return -1;
    }
    return send_packet(socket, load, len, MSG_NOSIGNAL);
}

// output: message length on success, -1 on failure
// 
int receive_package(int socket, char *buf, size_t buf_len) {
    uint32_t len_net;
    if (receive_packet(socket, &len_net, sizeof(len_net), 0) != RECV_ERR_OK) {
        return -1;
    }

    uint32_t len = ntohl(len_net);
    if (len >= buf_len) {
        return -1;
    }
    if (receive_packet(socket, buf, len, MSG_NOSIGNAL) != RECV_ERR_OK) {
        return -1;
    }
    buf[len] = '\0';
    return 0;
}