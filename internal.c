#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // O_CREAT, O_RDWR 
#include <sys/wait.h>  // wait()
#include <unistd.h>  // ftruncate, close()

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
    const char *arg_type = (argc == 3) ? "car" : "floor";


    /*
    {operation}: open, close, stop, service_on, service_off, up, down
    {}
    */
    if (strcmp(arg_type, "car") == 0) {
        
    }
    else if (strcmp(arg_type, "floor") == 0) {
        
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
    int shm_fd = shm_open();
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    int ftruncate_result = ftruncate(shm_fd, sizeof(int));



}


