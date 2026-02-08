#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t elem_size;
    void (*print)(const void* elem); // Вызывает функцию для вывода элементов массива
    int (*compare)(const void* a, const void* b);
} FieldInfo;

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

typedef struct {
    FieldInfo* fieldinfo;
    void* data;
    size_t size;
    size_t capacity;
    size_t elem_size;
} DynamicArray;

DynamicArray* da_create(FieldInfo* fieldinfo) { 
    DynamicArray* da = (DynamicArray*)malloc(sizeof(DynamicArray)); // Выделяем память под структуру

    if (da == NULL) {
        return NULL;
    }

    da->capacity = 4;
    da->elem_size = fieldinfo->elem_size;
    da->size = 0;
    da->fieldinfo = fieldinfo;

    da->data = malloc(da->capacity * da->elem_size); // Выделяем память под массив

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
    return (char*)da->data + index * da->elem_size; // Адрес элемента
}

void da_append(DynamicArray* da, void* elem) {
    if (da->size >= da->capacity) {
        da->capacity *= 2;
        void* tmp_data = realloc(da->data, da->capacity * da->elem_size);

        if (tmp_data == NULL) {
            printf("Memory error");
            return;
        }

        da->data = tmp_data;
    }
    
    memcpy(da_get(da, da->size), elem, da->elem_size); // куда, откуда, размер. Куда определяем через приведение к байтам
                                                                            // (0 элемент + сдвиг по байтам, который текущий размер + размер эл. в байтах)
    da->size++;
}

void da_print(DynamicArray* da) {
    
    for (int i = 0; i < da->size; i++) {
        da->fieldinfo->print(da_get(da, i)); // Вызываем функцию по указателю
    }
}

DynamicArray* da_concat(DynamicArray* a, DynamicArray* b) {
    if (a->fieldinfo == b->fieldinfo) {
        DynamicArray* ab = (DynamicArray*)malloc(sizeof(DynamicArray));

        if (ab == NULL) {
            return NULL;
        }

        ab->capacity = a->size + b->size;
        ab->elem_size = a->elem_size;
        ab->size = 0;
        ab->fieldinfo = a->fieldinfo;

        ab->data = malloc(ab->capacity * ab->elem_size);

        if (ab->data == NULL) {
            free(ab);
            return NULL;
        }

        for (int j = 0; j < a->size; j++) {
            memcpy(da_get(ab, ab->size), da_get(a, j), ab->elem_size);
            ab->size++;
        }
        
        for (int k = 0; k < b->size; k++) {
            memcpy(da_get(ab, ab->size), da_get(b, k), ab->elem_size);
            ab->size++;
        }

        // Дописать оптимизацию(как-то можно копировать целые блоки через memcpy) за два вызова

        return ab;
    }

    return NULL;
}

DynamicArray* da_map(DynamicArray* da, void (*f)(const void* src, void* dest)) { // src - указатель на исходный элемент, dest - указатель на место в новом массиве
    DynamicArray* map_da = (DynamicArray*)malloc(sizeof(DynamicArray));

    if (map_da == NULL) {
        return NULL;
    }

    map_da->capacity = da->size;
    map_da->elem_size = da->elem_size;
    map_da->fieldinfo = da->fieldinfo;
    map_da->size = 0;

    map_da->data = malloc(map_da->capacity * map_da->elem_size);

    if (map_da->data == NULL) {
        free(map_da);
        return NULL;
    }

    for (int i = 0; i < map_da->capacity; i++) {
        f(da_get(da, i), da_get(map_da, i)); // В новый массив записывается результат применения функции к элементу в исходном массиве
        map_da->size++;
    }

    return map_da;
}

DynamicArray* da_where(DynamicArray* da, int (*predicate)(const void* elem)) {
    DynamicArray* where_da = da_create(da->fieldinfo);

    for (int i = 0; i < da->size; i++) {
        if (predicate(da_get(da, i)) == 1) {
            da_append(where_da, da_get(da, i));
        }
    }

    return where_da;
}

void da_sort(DynamicArray* da) {
    qsort(da->data, da->size, da->elem_size, da->fieldinfo->compare);
}



int is_positive(const void* elem) {
    return *(double*)elem > 0;
}

void square_double(const void* src, void* dest) {
    *(double*)dest = *(double*)src * *(double*)src;
}

int main() {

    FieldInfo int_info = {
        .elem_size = sizeof(int),
        .compare = compare_int,
        .print = print_int
    };

    FieldInfo double_info = {
        .elem_size = sizeof(double),
        .compare = compare_double,
        .print = print_double
    };

    DynamicArray* a = da_create(&double_info);
    DynamicArray* b = da_create(&double_info);

    int n = 5;
    
    double c = -5.0;
    // printf("Сколько чисел хотите ввести? ");
    // scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        double val_1 = i + n;
        double val_2 = i + 2 * n;
        // scanf("%lf", &value);
        da_append(a, &val_1);
        da_append(b, &val_2);
    }
    da_append(a, &c);

    da_print(a);
    printf("\n");
    da_print(b);
    printf("\n");

    DynamicArray* da = da_concat(a, b);
    da_print(da);
    printf("\n");

    DynamicArray* map_da = da_map(da, square_double);
    da_print(map_da);
    printf("\n");

    DynamicArray* where_da = da_where(da, is_positive);
    da_print(where_da);
    printf("\n");

    da_sort(da);
    da_print(da);
    // for (int i = 0; i < da->size; i++) {
    //     printf("%lf ", *(double*)da_get(da, i)); // Вычисляем адрес через char, потом приводим к нужному типу указателя и разыменовываем
    // }
    da_free(a);
    da_free(b);
    da_free(da);
    da_free(map_da);
    da_free(where_da);

    return 0;
}

    // int i_arr[4] = {1, 2, 3, 4};
    // double d_arr[5] = {1.2, 2.0, 3.0, 4.0, 5.0};
    
    // // int (*f)(int) = square;

    // // printf("%d\n", (*f)(5));

    // apply(d_arr, 5, sizeof(double), square_double); // можно &square_int
    // for (int i = 0; i < 5; i++) {
    //     printf("%f ", *(d_arr + i));
    // }

    // apply(i_arr, 4, sizeof(int), square_int);
    // for (int i = 0; i < 4; i++) {
    //     printf("%d ", *(i_arr + i));
    // }

// void print_value(void* data, int type_flag) { // 0 - int; 1 - double    
//     if (type_flag == 0) {
//         printf("%d\n", *(int*)data);
//     } else {
//         printf("%f\n", *(double*)data);
//     }
// }

    // int (*f)(int n) = &square;

    // printf("%d", f(5));

    // int x = 5;
    // double y = 10.05;

    // print_value(&x, 0);
    // print_value(&y, 1); работа с указателем void*