#include <stdio.h>
#include <stdlib.h>
#include "double_field_info.h"

void print_double(const void* elem) {
    printf("%lf ", *(double*)(elem));
}

int compare_double(const void* val_a, const void* val_b) {
    double a = *(double*)(val_a);
    double b = *(double*)(val_b);

    if (a == b) {
        return 0;
    } else if ( a > b)
    {
        return 1;
    } else return -1;
}

void square_double(const void* src, void* dest) {
    *(double*)dest = *(double*)src * *(double*)src;
}

void reverse_double(const void* src, void* dest) {
    *(double*)dest = -(*(double*)src); 
}

int is_positive_double(const void* elem) {
    return *(double*)elem > 0;
}

int is_negative_double(const void* elem) {
    return *(double*)elem < 0;
}

static FieldInfo* DOUBLE_FIELD_INFO = NULL;

FieldInfo* get_double_field_info() {

    if (DOUBLE_FIELD_INFO == NULL) {
        
        DOUBLE_FIELD_INFO = (FieldInfo*)malloc(sizeof(FieldInfo));

        DOUBLE_FIELD_INFO->elem_size = sizeof(double);
        DOUBLE_FIELD_INFO->compare = compare_double;
        DOUBLE_FIELD_INFO->is_positive = is_positive_double;
        DOUBLE_FIELD_INFO->is_negative = is_negative_double;
        DOUBLE_FIELD_INFO->print = print_double;
        DOUBLE_FIELD_INFO->square = square_double;
        DOUBLE_FIELD_INFO->reverse = reverse_double;
    }

    return DOUBLE_FIELD_INFO;
}