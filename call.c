#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "packet_helper.h"
#include "shared.h"


#define CONTROLLER_PORT 3001


int main(int argc, char *argv[]) {
        if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments\n");
        return EXIT_FAILURE;
    }

        if (argv[0] == NULL || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return EXIT_FAILURE;
    }

    if (!car_name_validity_check(argv[1])) {
        fprintf(stderr, "Invalid car name\n");
        return EXIT_FAILURE;
    }
    if(!floor_validity_check(argv[2])) {
        fprintf(stderr, "Invalid floor\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[3], "up") != 0 && strcmp(argv[3], "down") != 0) {
        fprintf(stderr, "Invalid direction\n");
        return EXIT_FAILURE;
    }



}