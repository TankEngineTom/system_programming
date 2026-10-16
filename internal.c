#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // O_CREAT, O_RDWR 
#include <unistd.h>  // ftruncate, close()
#include <sys/mman.h>
#include <sys/stat.h> 
#include "car_shared.h"
#include <string.h>>
/* 
arguments: 
./internal {operation} {car name}
./internal floor {car name} {floor}
*/

int main(int argc, char *argv[]) {
    /*validate command line arguments*/

    if (argc != 3 && argc != 4) { 
        fprintf(stderr, "Invalid number of arguments\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "floor") == 0 && argc != 4) {
        fprintf(stderr, "Invalid arguments.\n");
        return EXIT_FAILURE;

    }
    if (strcmp(argv[1], "floor") == 0 && argc != 3) {
        fprintf(stderr, "Invalid arguments.\n");
        return EXIT_FAILURE;

    }


    const char *arg_type = (argc == 3) ? "car" : "floor";


    /*
    {operation}: open, close, stop, service_on, service_off, up, down
    */
    if (strcmp(arg_type, "car") == 0) {
        if (strcmp(argv[1], "open") == 0 || 
        strcmp(argv[1],  "close") == 0 ||
        strcmp(argv[1],  "stop") == 0 ||
        strcmp(argv[1],  "service_on") == 0 ||
        strcmp(argv[1],  "service_off") == 0 ||
        strcmp(argv[1],  "up") == 0 ||
        strcmp(argv[1],  "down") == 0 ) {
            // passed, can continue
        }
        else
            return EXIT_FAILURE;

        // check car name
        if(!car_name_validity_check(argv[2])) {
            return EXIT_FAILURE;
        }
    }
    
    
    if (strcmp(arg_type, "floor") == 0 && strcmp(argv[1], "floor") == 0) {
        if (!car_name_validity_check(argv[2])) {

            return EXIT_FAILURE;
        }
        if (!floor_validity_check(argv[3])) {
            return EXIT_FAILURE;
        }
    }

    /* 
    error codes:
    • EACCES: Permission to shm_open() the name in the specified mode was denied, or O_TRUNC was specified and the caller lacks write permission, or permission to create the shared memory object was denied.
    • EEXIST: Both O_CREAT and O_EXCL flags were specified, but the shared memory object with that name already exists.
    • EINTR: The shm_open() call was interrupted by a signal.
    • EINVAL: The name argument was invalid, the name string was empty, or incompatible access modes were provided in oflag.
    • EMFILE: The per-process limit on the number of open file descriptors has been reached.
    • ENAMETOOLONG: The length of the name argument exceeds PATH_MAX (or NAME_MAX).
    • ENFILE: The system-wide limit on the total number of open files/shared memory objects has been reached.
    • ENOENT: An attempt was made to open a name that does not exist, and O_CREAT was not specified.
    • ENOSPC: There is insufficient space remaining to create the new shared memory object.
    • ENOSYS: The shm_open() function is not supported or not implemented on this system.
    • EFAULT: The name argument points outside the accessible address space of the calling process.
    */

    // create a shared memory object
    char shm_name[64];
    snprintf(shm_name, sizeof(shm_name), "/cab403_car_%s", argv[2]);


    int shm_fd = shm_open(shm_name, O_RDWR, 0);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }


    car_shared_mem *car_shm = mmap(NULL, sizeof(car_shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);
    if(shm_fd == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    pthread_mutex_lock (&car_shm->mutex);
    car_shm->open_button = 1;
    pthread_cond_broadcast(&car_shm->cond);
    pthread_mutex_unlock(&car_shm->mutex);

    munmap(car_shm, sizeof(car_shared_mem));
    return EXIT_SUCCESS;



}


