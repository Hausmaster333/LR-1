#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

int read_one(InputType type, void* elem) {
    char buf[64];

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return 0;
    }

    char* endptr;
    char* start = buf;

    while (isspace(*start)) start++;

    if (*start == '\0') return 0;

    if (type == INPUT_INT) {
        long val = strtol(start, &endptr, 10);
        while (isspace(*endptr)) endptr++;
        
        if (*endptr != '\0') return 0;

        *(int*)elem = val;
        return 1;
    } else if (type == INPUT_DOUBLE) {
        double val = strtod(start, &endptr);
        while (isspace(*endptr)) endptr++;
        
        if (*endptr != '\0') return 0;

        *(double*)elem = val;
        return 1;
    }   

    return 0;
}