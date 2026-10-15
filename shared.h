#ifndef SHARED_H
#define SHARED_H
#include <stdbool.h>
#define CAR_MAX_LEN 31
#define CAR_MIN_LEN 1
bool floor_validity_check(const char *flr);
bool floor_to_int(const char *flr, int *signed_floor);
bool car_name_validity_check(const char *car_name);
bool int_to_floor(int signed_floor, char *flr);
#endif