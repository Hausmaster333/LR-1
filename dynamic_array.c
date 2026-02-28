#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"

DynamicArray* da_create(FieldInfo* fieldinfo) {
    DynamicArray* da = (DynamicArray*)malloc(sizeof(DynamicArray)); // Выделяем память под структуру

    if (da == NULL) {
        return NULL;
    }

    da->capacity = 4;
    da->size = 0;
    da->fieldinfo = fieldinfo;

    da->data = malloc(da->capacity * da->fieldinfo->elem_size); // Выделяем память под массив

    if (da->data == NULL) {
        free(da);
        return NULL;
    }

    return da;
}

void da_free(DynamicArray* da) {
    free(da->data);
    free(da);
}

void* da_get(DynamicArray* da, size_t index) { // Возвращает указатель на элемент по индексу

    if (index >= da->size) { // Проверка на выход за пределы массива
        return NULL;
    }
    
    return (char*)da->data + index * da->fieldinfo->elem_size; // Адрес элемента
}

static void* da_get_ptr(DynamicArray* da, size_t index) {
    return (char*)da->data + index * da->fieldinfo->elem_size; // (0 элемент + сдвиг по байтам, который текущий размер + размер эл. в байтах)
}

void da_append(DynamicArray* da, void* elem) {

    if (da == NULL) {
        return;
    }

    if (da->size >= da->capacity) {
        da->capacity *= 2;
        void* tmp_data = realloc(da->data, da->capacity * da->fieldinfo->elem_size);

        if (tmp_data == NULL) {
            printf("Memory error");
            return;
        }

        da->data = tmp_data;
    }
    
    memcpy(da_get_ptr(da, da->size), elem, da->fieldinfo->elem_size); // куда, откуда, размер. Куда определяем через приведение к байтам
                                                                            
    da->size++;
}

void da_print(DynamicArray* da) {
    
    if (da->size == 0) {
        printf("Array is empty ");
        return;
    }

    for (int elem_i = 0; elem_i < da->size; elem_i++) {
        da->fieldinfo->print(da_get_ptr(da, elem_i)); // Вызываем функцию по указателю
    }
}

DynamicArray* da_concat(DynamicArray* a, DynamicArray* b) {

    if (a != NULL && b != NULL) {
        if (a->fieldinfo == b->fieldinfo) {
            DynamicArray* ab = (DynamicArray*)malloc(sizeof(DynamicArray));

            if (ab == NULL) {
                return NULL;
            }

            ab->capacity = a->size + b->size;
            ab->size = 0;
            ab->fieldinfo = a->fieldinfo;
            
            ab->data = malloc(ab->capacity * ab->fieldinfo->elem_size);

            if (ab->data == NULL) {
                free(ab);
                return NULL;
            }

            for (int a_elem_i = 0; a_elem_i < a->size; a_elem_i++) {
                memcpy(da_get_ptr(ab, ab->size), da_get_ptr(a, a_elem_i), ab->fieldinfo->elem_size);
                ab->size++;
            }
            
            for (int b_elem_i = 0; b_elem_i < b->size; b_elem_i++) {
                memcpy(da_get_ptr(ab, ab->size), da_get_ptr(b, b_elem_i), ab->fieldinfo->elem_size);
                ab->size++;
            }

            // Дописать оптимизацию(как-то можно копировать целые блоки через memcpy) за два вызова

            return ab;
        }
        printf("Error: Arrays must be of the same types\n");
        return NULL;
    }

    return NULL;
}

DynamicArray* da_map(DynamicArray* da, void (*f)(const void* src, void* dest)) { // src - указатель на исходный элемент, dest - указатель на место в новом массиве
    DynamicArray* map_da = (DynamicArray*)malloc(sizeof(DynamicArray));

    if (map_da == NULL) {
        return NULL;
    }

    map_da->capacity = da->size;
    map_da->fieldinfo = da->fieldinfo;
    map_da->size = 0;

    map_da->data = malloc(map_da->capacity * map_da->fieldinfo->elem_size);

    if (map_da->data == NULL) {
        free(map_da);
        return NULL;
    }

    for (int elem_i = 0; elem_i < map_da->capacity; elem_i++) {
        f(da_get_ptr(da, elem_i), da_get_ptr(map_da, elem_i)); // В новый массив записывается результат применения функции к элементу в исходном массиве
        map_da->size++;
    }

    return map_da;
}

DynamicArray* da_where(DynamicArray* da, int (*predicate)(const void* elem)) {
    DynamicArray* where_da = da_create(da->fieldinfo);

    for (int elem_i = 0; elem_i < da->size; elem_i++) {
        if (predicate(da_get_ptr(da, elem_i)) == 1) {
            da_append(where_da, da_get_ptr(da, elem_i));
        }
    }

    return where_da;
}

static void da_elements_swap(DynamicArray* da, size_t first_elem, size_t second_elem) {

    void* ptr_first_elem = da_get(da, first_elem);
    void* ptr_second_elem = da_get(da, second_elem);

    size_t size = da->fieldinfo->elem_size;
    void* tmp_ptr = (size_t*)malloc(size);
    memcpy(tmp_ptr, ptr_first_elem, size);
    memcpy(ptr_first_elem, ptr_second_elem, size);
    memcpy(ptr_second_elem, tmp_ptr, size);
}

static int da_split(DynamicArray* da, int low, int high) { // Возвращает точку разделения
    void* pivot = malloc(da->fieldinfo->elem_size);
    memcpy(pivot, da_get_ptr(da, (low + high) / 2), da->fieldinfo->elem_size);

    while (low <= high) {
        while (da->fieldinfo->compare(da_get_ptr(da, low), pivot) < 0) low++;
        while (da->fieldinfo->compare(da_get_ptr(da, high), pivot) > 0) high--;

        if (low <= high) {
            da_elements_swap(da, low, high);
            low++;
            high--;
        }
        
    }
    free(pivot);
    return low;
}

static void da_quicksort(DynamicArray* da, int low, int high) {
    if (low < high) {
        int split = da_split(da, low, high);
        da_quicksort(da, low, split - 1);
        da_quicksort(da, split, high);
    }
}

void da_sort(DynamicArray* da) {
    da_quicksort(da, 0, da->size - 1);
    // qsort(da->data, da->size, da->fieldinfo->elem_size, da->fieldinfo->compare);
}

