#include <stdbool.h>
#include <stddef.h>
#define CAR_MAX_LEN 31
#define CAR_MIN_LEN 1

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

bool floor_to_int(const char *flr, int *signed_floor) {
    if(!floor_validity_check(flr) || signed_floor == NULL) {
        return false;
    }
    if (flr[0] == 'B') {
        *signed_floor = -atoi(flr + 1);
    }
    else{
        *signed_floor = atoi(flr);
    }
    return true;
}

bool int_to_floor(int signed_floor, char *flr) {
    
    bool validity_check = false;

    if(flr == NULL) {
        return false;
    }
    if (signed_floor == 0) {
        validity_check = false;
    }
    else if( -99 <= signed_floor && signed_floor <= -1) {
        snprintf(flr, 4, "B%d", -signed_floor);
        validity_check = true;
    }
    else if( 1 <= signed_floor && signed_floor <= 999) {
        snprintf(flr, 4, "%d", signed_floor);
        validity_check = true;
    }
    // catches all other cases when signed_floor is not in the range of -99 to 999 and is not 0 
    else{
        validity_check = false;
    }
    return validity_check;
}

bool car_name_validity_check(const char *car_name) {

    if(car_name == NULL) {
        return false;
    }

    size_t length = strlen(car_name);

    // initial check for length of car_name. must be between 1 and 31 characters
    if (length < CAR_MIN_LEN || length > CAR_MAX_LEN) {
        return false;
    }
    
    if (!((car_name[0] >= 'A' && car_name[0] <= 'Z')
        || (car_name[0] >= 'a' && car_name[0] <= 'z')
        || (car_name[0] >= '0' && car_name[0] <= '9'))) {
        return false;
    }
  
    // check if all char are not capitalised or lower case letters 
    for (size_t i = 0; i < length; i++) {
        if(!((car_name[i] >= 'A' && car_name[i] <= 'Z') 
            || (car_name[i] >= 'a' && car_name[i] <= 'z') 
            || (car_name[i] >= '0' && car_name[i] <= '9')
            || (car_name[i] == '_') || (car_name[i] == '-')
        )) {
            return false;
        }
    }
    return true;
}