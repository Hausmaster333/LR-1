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
    return (char*)da->data + index * da->fieldinfo->elem_size;
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
                                                                            // (0 элемент + сдвиг по байтам, который текущий размер + размер эл. в байтах)
    da->size++;
}

void da_print(DynamicArray* da) {
    
    if (da->size == 0) {
        printf("Array is empty ");
        return;
    }

    for (int i = 0; i < da->size; i++) {
        da->fieldinfo->print(da_get_ptr(da, i)); // Вызываем функцию по указателю
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

            for (int j = 0; j < a->size; j++) {
                memcpy(da_get_ptr(ab, ab->size), da_get_ptr(a, j), ab->fieldinfo->elem_size);
                ab->size++;
            }
            
            for (int k = 0; k < b->size; k++) {
                memcpy(da_get_ptr(ab, ab->size), da_get_ptr(b, k), ab->fieldinfo->elem_size);
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

    for (int i = 0; i < map_da->capacity; i++) {
        f(da_get_ptr(da, i), da_get_ptr(map_da, i)); // В новый массив записывается результат применения функции к элементу в исходном массиве
        map_da->size++;
    }

    return map_da;
}

DynamicArray* da_where(DynamicArray* da, int (*predicate)(const void* elem)) {
    DynamicArray* where_da = da_create(da->fieldinfo);

    for (int i = 0; i < da->size; i++) {
        if (predicate(da_get_ptr(da, i)) == 1) {
            da_append(where_da, da_get_ptr(da, i));
        }
    }

    return where_da;
}

void da_sort(DynamicArray* da) { // Сортировка, написать самому без встроенной функции
    qsort(da->data, da->size, da->fieldinfo->elem_size, da->fieldinfo->compare);
}

