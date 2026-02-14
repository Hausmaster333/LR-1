#ifndef UTILS_H
#define UTILS_H

typedef enum InputType {
    INPUT_INT,
    INPUT_DOUBLE
} InputType;

int read_one(InputType type, void* elem);

#endif