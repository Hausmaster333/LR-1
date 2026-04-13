#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array_field_info.h"

void print_dynamic_array(const void* elem) {
    DynamicArray* da = *(DynamicArray**)elem; // Приводим к указателю на указатель на DynamicArray и разыменовывая получаем указатель на DynamicArray
    da_print(da);
}

int compare_dynamic_array(const void* a, const void* b) {
    DynamicArray* da_a = *(DynamicArray**)a;
    DynamicArray* da_b = *(DynamicArray**)b;

    if (da_a->size < da_b->size) {
        return -1;
    } else if (da_a->size > da_b->size) {
        return 1;
    } else return 0;
}

void square_dynamic_array(const void* src, void* dest) {
    DynamicArray* src_da = *(DynamicArray**)src;
    
    DynamicArray* new_da = da_create(src_da->fieldinfo);
    
    for (int i = 0; i < src_da->size; i++) {
        void* tmp = malloc(src_da->fieldinfo->elem_size);
        src_da->fieldinfo->square(da_get(src_da, i), tmp);

        da_append(new_da, tmp);

        free(tmp);
    }
    
    *(DynamicArray**)dest = new_da;
}

int is_positive_dynamic_array(const void* elem) {
    DynamicArray* da = *(DynamicArray**)elem;
    return da->size > 0;
}

int is_negative_dynamic_array(const void* elem) {
    return 0;
}

void reverse_dynamic_array(const void* src, void* dest) {
    DynamicArray* src_da = *(DynamicArray**)src;
    
    DynamicArray* new_da = da_create(src_da->fieldinfo);
    
    for (int i = 0; i < src_da->size; i++) {
        void* tmp = malloc(src_da->fieldinfo->elem_size);
        src_da->fieldinfo->reverse(da_get(src_da, i), tmp);

        da_append(new_da, tmp);

        free(tmp);
    }
    
    *(DynamicArray**)dest = new_da;
}

void destroy_dynamic_array(void* elem) {
    DynamicArray* da = *(DynamicArray**)elem;

    da_free(da);
}

static FieldInfo* DYNAMIC_ARRAY_FIELD_INFO = NULL;

FieldInfo* get_dynamic_array_field_info() {

    if (DYNAMIC_ARRAY_FIELD_INFO == NULL) {

        DYNAMIC_ARRAY_FIELD_INFO = (FieldInfo*)malloc(sizeof(FieldInfo));

        DYNAMIC_ARRAY_FIELD_INFO->elem_size = sizeof(DynamicArray*);
        DYNAMIC_ARRAY_FIELD_INFO->print = print_dynamic_array;
        DYNAMIC_ARRAY_FIELD_INFO->compare = compare_dynamic_array;
        DYNAMIC_ARRAY_FIELD_INFO->square = square_dynamic_array;
        DYNAMIC_ARRAY_FIELD_INFO->is_positive = is_positive_dynamic_array;
        DYNAMIC_ARRAY_FIELD_INFO->is_negative = is_negative_dynamic_array;
        DYNAMIC_ARRAY_FIELD_INFO->reverse = reverse_dynamic_array;
        DYNAMIC_ARRAY_FIELD_INFO->destroy = destroy_dynamic_array;
    }

    return DYNAMIC_ARRAY_FIELD_INFO;
}

