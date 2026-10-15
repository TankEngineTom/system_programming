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
#define SEND_ERR_SOCKET -1
#define SEND_ERR_OK 0
#define SEND_ERR_LARGE -2

int send_packet(int socket, const void *buf, size_t len, int flags) {
    size_t bytes_sent_total = 0;
    size_t bytes_sent_now = 0;

    const char *buf_ptr = buf;

    // active until all bytes are sent
    while (bytes_sent_total < len) {
        // send returns the number of bytes sent, or -1 on error
        bytes_sent_now = send(socket, buf_ptr + bytes_sent_total, len - bytes_sent_total, flags);
        // interrupted by signal so we can try again, otherwise return error
        if (bytes_sent_now == -1) {
            if (errno == EINTR) {
                continue; 
            }
            return RECV_ERR_SOCKET;
        }
        bytes_sent_total += (size_t)bytes_sent_now;
    }
    return RECV_ERR_OK;
}


// 0 on success, -1 on failure, 1 if the connection is closed by the peer

int receive_packet(int socket, void *buf, size_t len, int flags) {
    size_t bytes_received_total = 0;
    ssize_t bytes_received_now = 0;
    char *buf_ptr = buf;
    // loop to account for less than requested bytes received
    while (bytes_received_total < len) {
        bytes_received_now = recv(socket, buf_ptr + bytes_received_total, len - bytes_received_total, flags);

        // check for peer closing before or mid way through the receive
        if (bytes_received_now == 0) {
            if (bytes_received_total == 0) {
                return RECV_ERR_CLOSED;
            }
            return RECV_ERR_SOCKET;
        }

        
        if (bytes_received_now == -1) {
            if (errno == EINTR) {
                continue;
            }
            return RECV_ERR_SOCKET;
        }

        bytes_received_total += (size_t)bytes_received_now;
    }
    return RECV_ERR_OK;
}


// output: 0 on success, -1 on failure
int send_package(int socket, const char *load) {
    size_t len = strlen(load);

    // check if the message length exceeds the maximum allowed size
    if (len > MAX_MESSAGE) {
        return SEND_ERR_LARGE;
    }
    // explicit conversion to network byte order
    uint32_t len_net = htonl((uint32_t)len);
    // send header then body, return error if either fails
    if (send_packet(socket, &len_net, sizeof(len_net), 0) != SEND_ERR_OK) {
        return SEND_ERR_SOCKET;
    }
    return send_packet(socket, load, len, 0);
}

// output: message length on success, -1 on failure
int receive_package(int socket, char *buf, size_t buf_len) {
    uint32_t len_net;
    // reads header
    if (receive_packet(socket, &len_net, sizeof(len_net), 0) != RECV_ERR_OK) {
        return RECV_ERR_SOCKET;
    }
    // converts back to host byte order
    uint32_t len = ntohl(len_net);

    // buffrer length check
    if (len >= buf_len) {
        return RECV_ERR_SOCKET;
    }
    if (receive_packet(socket, buf, len, MSG_NOSIGNAL) != RECV_ERR_OK) {
        return RECV_ERR_SOCKET;
    }
    buf[len] = '\0';
    return RECV_ERR_OK;
}