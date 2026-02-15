#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stddef.h>

typedef struct FieldInfo {
    size_t elem_size;
    void (*print)(const void* elem);
    int (*compare)(const void* a, const void* b);
    void (*square)(const void* src, void* dest);
    void (*reverse)(const void* src, void* dest);
    int (*is_positive)(const void* elem);
    int (*is_negative)(const void* elem);
} FieldInfo;

#endif