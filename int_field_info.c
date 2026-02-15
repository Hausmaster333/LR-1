#include <stdio.h>
#include <stdlib.h>
#include "int_field_info.h"

void print_int(const void* elem) {
    printf("%d ", *(int*)(elem));
}

int compare_int(const void* val_a, const void* val_b) {
    int a = *(int*)(val_a);
    int b = *(int*)(val_b);

    if (a == b) {
        return 0;
    } else if ( a > b)
    {
        return 1;
    } else return -1;
}

void square_int(const void* src, void* dest) {
    *(int*)dest = *(int*)src * *(int*)src;
}

void reverse_int(const void* src, void* dest) {
    *(int*)dest = -(*(int*)src);
}

int is_positive_int(const void* elem) {
    return *(int*)elem > 0;
}

int is_negative_int(const void* elem) {
    return *(int*)elem < 0;
}

static FieldInfo* INT_FIELD_INFO = NULL;

FieldInfo* get_int_field_info() {

    if (INT_FIELD_INFO == NULL) {

        INT_FIELD_INFO = (FieldInfo*)malloc(sizeof(FieldInfo));

        INT_FIELD_INFO->elem_size = sizeof(int);
        INT_FIELD_INFO->compare = compare_int;
        INT_FIELD_INFO->is_positive = is_positive_int;
        INT_FIELD_INFO->is_negative = is_negative_int;
        INT_FIELD_INFO->print = print_int;
        INT_FIELD_INFO->square = square_int;
        INT_FIELD_INFO->reverse = reverse_int;
    }

    return INT_FIELD_INFO;    
}