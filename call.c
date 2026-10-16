#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include "packet_helper.h"
#include "shared.h"


#define CONTROLLER_PORT 3001



int make_connection() {
    struct sockaddr_in server = {0};
    int server_socket = 0;

    // create TCP socket
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // define server address
    server.sin_family = AF_INET;
    server.sin_port = htons(CONTROLLER_PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // replace with actual IP address

    int connection_status = 0;
    connection_status = connect(server_socket, (struct sockaddr *)&server, sizeof server);

    if (connection_status == -1) {
        close(server_socket);
        perror("Connection failed");
        return -1;
    }
    return server_socket;
}

/*
./call {car name} {floor} {up|down}
validates three arguments and exits with EXIT_FAILURE if any are invalid.
*/

int main(int argc, char *argv[]) {

    /*
    validation of arguments. 

    */
    if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments\n");
        return EXIT_FAILURE;
    }
        if (argv[0] == NULL || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return EXIT_FAILURE;
    }

    if (!car_name_validity_check(argv[1])) {
        fprintf(stderr, "Invalid car name.\n");
        return EXIT_FAILURE;
    }
    if(!floor_validity_check(argv[2])) {
        fprintf(stderr, "Invalid floor specified.\n");
        return EXIT_FAILURE;
    }
    // only up or down are valid directions
    if (strcmp(argv[3], "up") != 0 && strcmp(argv[3], "down") != 0) {
        fprintf(stderr, "Invalid direction specified.\n");
        return EXIT_FAILURE;
    }

    int socket_fd = make_connection();
    if (socket_fd == -1) {
        fprintf(stderr, "Unable to connect to elevator system.\n");
        return EXIT_FAILURE;
    }

    // convert the direction to uppercase
    for (int i = 0; argv[3][i]; i++) {
        argv[3][i] = toupper((unsigned char)argv[3][i]);
    }
    
    char message_const[60];
    snprintf(message_const, sizeof(message_const), "CALL %s %s %s", argv[1], argv[2], argv[3]);

    if (send_package(socket_fd, message_const) < 0) {
        fprintf(stderr, "Failed to send request to server.\n");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    char response[60];
    if (receive_package(socket_fd, response, sizeof(response)) < 0) {
        fprintf(stderr, "Failed to receive response from server.\n");
        close(socket_fd);
        return EXIT_FAILURE;
    }


    if (strcmp(response, "ACCEPTED") == 0) {
        printf("Car %s is arriving.\n", argv[1]);
    }
    else if (strcmp(response, "UNAVAILABLE") == 0) {
        printf("Sorry, that car is unavailable for this request.\n");
    }

    close(socket_fd);


}
