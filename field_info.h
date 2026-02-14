#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stdio.h>

typedef struct FieldInfo {
    size_t elem_size;
    void (*print)(const void* elem);
    int (*compare)(const void* a, const void* b);
    void (*square)(const void* dest, void* src);
    int (*is_positive)(const void* elem);
} FieldInfo;

#endif