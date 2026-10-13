#include <stdbool.h>
#include <stddef.h>


bool floor_validity_check(const char *flr) {
    // check for null
    if(flr == NULL) {
        return false;
    }

    // check for first char == B
    bool isB = (flr[0] == 'B');

    if(isB) {
        // check for second char 
        if(flr[1] < '1' || flr[1] > '9') {
            return false;
        }
        if(flr[2] == '\0') {
            return true;
        }
        if (flr[2] <'0' || flr[2] > '9') {
            return false;
        }
        return (flr[3] == '\0');
    }

    // logic check for non basement floors
    if (flr[0] < '1' || flr[0] > '9') {
        return false;
    }
    if (flr[1] == '\0') {
        return true;
    }

    if(flr[1] < '0' || flr[1] > '9') {
        return false;
    }
    if (flr[2] == '\0') {
        return true;
    }
    if (flr[2] < '0' || flr[2] > '9') {
        return false;
    }

    return flr[3] == '\0';
}