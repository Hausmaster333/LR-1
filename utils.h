#ifndef UTILS_H
#define UTILS_H

typedef enum {
    INPUT_INT,
    INPUT_DOUBLE
} InputType;

int read_one(InputType type, void* elem);

#endif