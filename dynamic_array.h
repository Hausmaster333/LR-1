#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include "field_info.h"

typedef struct {
    FieldInfo* fieldinfo;
    void* data;
    size_t size;
    size_t capacity;
} DynamicArray;

DynamicArray* da_create(FieldInfo* fieldinfo);

void da_free(DynamicArray* da);

void* da_get(DynamicArray* da, size_t index);

void da_append(DynamicArray* da, void* elem);

void da_print(DynamicArray* da);

DynamicArray* da_concat(DynamicArray* a, DynamicArray* b);

DynamicArray* da_map(DynamicArray* da, void (*f)(const void* src, void* dest));

DynamicArray* da_where(DynamicArray* da, int (*predicate)(const void* elem));

void da_sort(DynamicArray* da);


#endif