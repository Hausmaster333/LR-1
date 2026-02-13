#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

typedef enum {
    INPUT_INT,
    INPUT_DOUBLE
} InputType;

int read_one(InputType type, void* elem) { // Проверка на единственность и корректность ввода числа | Если перед был scanf, то писать while (getchar() != '\n');
    char buf[64];

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return 0;
    }

    char* endptr;
    char* start = buf;

    while (isspace(*start)) start++;

    if (*start == '\0') return 0;

    if (type == INPUT_INT) {
        long val = strtol(start, &endptr, 10);
        while (isspace(*endptr)) endptr++;
        
        if (*endptr != '\0') return 0;

        *(int*)elem = val;
        return 1;
    }

    return 0;
}

typedef struct {
    size_t elem_size;
    void (*print)(const void* elem); // Вызывает функцию для вывода элементов массива
    int (*compare)(const void* a, const void* b);
    void (*square)(const void* dest, void* src);
    int (*is_positive)(const void* elem);
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

void square_int(const void* src, void* dest) {
    *(int*)dest = *(int*)src * *(int*)src;
}

int is_positive_int(const void* elem) {
    return *(int*)elem > 0;
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

void square_double(const void* src, void* dest) {
    *(double*)dest = *(double*)src * *(double*)src;
}

int is_positive_double(const void* elem) {
    return *(double*)elem > 0;
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

    if (index >= da->size) { // Проверка на выход за пределы массива
        return NULL;
    }
    
    return (char*)da->data + index * da->elem_size; // Адрес элемента
}

static void* da_get_ptr(DynamicArray* da, size_t index) { // Возвращает указатель на элемент по индексу 
    return (char*)da->data + index * da->elem_size;
}

void da_append(DynamicArray* da, void* elem) {

    if (da == NULL) {
        return;
    }

    if (da->size >= da->capacity) {
        da->capacity *= 2;
        void* tmp_data = realloc(da->data, da->capacity * da->elem_size);

        if (tmp_data == NULL) {
            printf("Memory error");
            return;
        }

        da->data = tmp_data;
    }
    
    memcpy(da_get_ptr(da, da->size), elem, da->elem_size); // куда, откуда, размер. Куда определяем через приведение к байтам
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

// void da_square(DynamicArray* da, const void* dest, void* src) {
//     da->fieldinfo->square(dest, src);
// }

DynamicArray* da_concat(DynamicArray* a, DynamicArray* b) {

    if (a != NULL && b != NULL) {
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
                memcpy(da_get_ptr(ab, ab->size), da_get_ptr(a, j), ab->elem_size);
                ab->size++;
            }
            
            for (int k = 0; k < b->size; k++) {
                memcpy(da_get_ptr(ab, ab->size), da_get_ptr(b, k), ab->elem_size);
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
    map_da->elem_size = da->elem_size;
    map_da->fieldinfo = da->fieldinfo;
    map_da->size = 0;

    map_da->data = malloc(map_da->capacity * map_da->elem_size);

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

void da_sort(DynamicArray* da) {
    qsort(da->data, da->size, da->elem_size, da->fieldinfo->compare);
}

// int da_is_positive(DynamicArray* da, const void* elem) {
//     da->fieldinfo->is_positive(elem);
// }

void test_append() {

    printf("START test_append\n");

    FieldInfo double_info = {
        .compare = compare_double,
        .elem_size = sizeof(double),
        .print = print_double
    };

    DynamicArray* da = da_create(&double_info);

    int capacity = 6;

    for (int i = 0; i < capacity; i++) {
        double value = i + capacity;
        da_append(da, &value);

        assert(*(double*)da_get(da, i) == value);
    }

    assert(da->size == 6);

    da_free(da);

    printf("test_append PASSED\n");
}

void test_concat() {

    printf("START test_concat\n");

    FieldInfo double_info = {
        .elem_size = sizeof(double),
        .compare = compare_double,
        .print = print_double,
    };

    DynamicArray* a = da_create(&double_info);
    DynamicArray* b = da_create(&double_info);

    int capacity = 6;


    for (int i = 0; i < capacity; i++) {
        double val_1 = i + capacity;
        double val_2 = i + 2 * capacity;
        da_append(a, &val_1);
        da_append(b, &val_2);
    }

    DynamicArray* concat_da = da_concat(a, b);

    assert(concat_da->size == a->size + b->size);


    for (int i = 0; i < a->size; i++) {
        assert(*(double*)da_get(concat_da, i) == *(double*)da_get(a, i));
    }

    int g = 0;

    for (int i = a->size; i < concat_da->size; i++) {
        assert(*(double*)da_get(concat_da, i) == *(double*)da_get(b, g));
        g++;
    }

    da_free(a);
    da_free(b);
    da_free(concat_da);

    printf("test_concat PASSED\n");

}

void test_where() {

    printf("START test_where\n");

    FieldInfo double_info = {
      .elem_size = sizeof(double),
      .compare = compare_double,
      .print = print_double,
      .is_positive = is_positive_double,
      .square = square_double
    };

    DynamicArray* da = da_create(&double_info);

    int capacity = 6;
    double val1 = 5.0;

    for (int i = 0; i < capacity; i++) {
        double value = -100 + i;
        da_append(da, &value);
    }
    da_append(da, &val1);

    DynamicArray* where_da = da_where(da, da->fieldinfo->is_positive);

    for (int i = 0; i < where_da->size; i++) {
        assert(*(double*)da_get(where_da, i) > 0);
    }
    
    assert(where_da->size == 1);

    da_free(da);
    da_free(where_da);

    printf("test_where PASSED\n");
}

int is_size_zero(DynamicArray* da) { // Вовзращает 1, если size равен 0
    return da->size == 0;
}



void menu() {
    DynamicArray* da = NULL;
    int main_choice;

    DynamicArray* arrays[10];
    int array_cnt = 0;

    FieldInfo int_info = {
                    .elem_size = sizeof(int),
                    .compare = compare_int,
                    .print = print_int,
                    .square = square_int,
                    .is_positive = is_positive_int
                };

    FieldInfo double_info = {
                    .elem_size = sizeof(double),
                    .compare = compare_double,
                    .print = print_double,
                    .square = square_double,
                    .is_positive = is_positive_double
                };

    while (1) {
        printf("\n=== Dynamic array ===\n");
        printf("1. Create array\n");
        printf("2. Add element\n");
        printf("3. Print array\n");
        printf("4. Sort array\n");
        printf("5. 'Map' array\n");
        printf("6. 'Where' array\n");
        printf("7. Concatenation of two arrays\n");
        printf("8. Run tests\n");
        printf("0. Exit\n");
        printf("Choice: ");

        if (!read_one(INPUT_INT, &main_choice)) {
            printf("Invalid input, try again\n");
            continue;
        }

        // scanf("%d", &main_choice);

        switch (main_choice)
        {
        case 1: // Create array
            int create_choice;
            printf("\nWhat types of numbers?\n");
            printf("1. Integers\n");
            printf("2. Doubles\n");
            printf("Choice: ");

            if (!read_one(INPUT_INT, &create_choice)) {
                printf("Invalid input, try again\n");
                continue;
            }

            // scanf("%d", &create_choice);
            
            switch (create_choice)
            {
            case 1: // Int array
                arrays[array_cnt++] = da_create(&int_info);
                printf("Succesfull create array for integers!\n");
                break;

            case 2: // Double array
                arrays[array_cnt++] = da_create(&double_info);
                printf("Succesfull create array for doubles!\n");
                break;

            default:
                printf("Error: Incorrect option, try again\n");
                break;
            }

            break;

        case 2: // Append element
            
            if (array_cnt == 0) {
                printf("You have 0 arrays");
                printf("\n");
                break;
            }

            int arr_app_choice;
            printf("\nChoice the array, avaivable arrays: \n");

            for (int i = 0; i < array_cnt; i++) {
                printf("Array %d: ", i + 1);
                da_print(arrays[i]);
                printf("\n");
            }

            printf("Choice: ");

            if (!read_one(INPUT_INT, &arr_app_choice)) {
                printf("Error: Invalid input, try again\n");
                continue;
            }

            if (arr_app_choice < 1 || arr_app_choice > array_cnt) {
                printf("Error: Invalid array number\n");
                break;
            }

            printf("Enter a number: ");

            if (arrays[arr_app_choice - 1]->fieldinfo == &int_info) {
                int i_num;
                
                if (!read_one(INPUT_INT, &i_num)) {
                    printf("Error: Invalid input, try again\n");
                    continue;
                }

                // РЕАЛИЗОВАТЬ ПРОВЕРКУ НА НЕКОРРЕКТНЫЙ ВВОД

                da_append(arrays[arr_app_choice - 1], &i_num);
                printf("Number %d completely added\n", i_num);
            } else if (arrays[arr_app_choice - 1]->fieldinfo == &double_info) {
                double d_num;
                
                if (!read_one(INPUT_DOUBLE, &d_num)) {
                    printf("Error: Invalid input, try again\n");
                    continue;
                }

                // РЕАЛИЗОВАТЬ ПРОВЕРКУ НА НЕКОРРЕКТНЫЙ ВВОД

                da_append(arrays[arr_app_choice - 1], &d_num);
                printf("Number %lf completely added\n", d_num);
            }

            break;
        
        case 3: // Print array
            // if (da == NULL) {
            //     printf("Array is empty\n");
            //     break;
            // }
            if (array_cnt == 0) {
                printf("You have 0 arrays");
                printf("\n");
                break;
            }

            printf("\n==== Avaivable arrays ====\n");
            
            for (int i = 0; i < array_cnt; i++) {
                printf("Array %d: ", i + 1);
                da_print(arrays[i]);
                printf("\n");
            }

            break;
         
        case 4: // Sort array
            // if (da == NULL) {
            //     printf("Array is empty\n");
            //     break;
            // }

            if (array_cnt == 0) {
                printf("You have 0 arrays");
                printf("\n");
                break;
            }

            int arr_sort_choice;
            printf("\nChoice the array, avaivable arrays: \n");

            for (int i = 0; i < array_cnt; i++) {
                printf("Array %d: ", i + 1);
                da_print(arrays[i]);
                printf("\n");
            }

            printf("Choice: ");

            if (!read_one(INPUT_INT, &arr_sort_choice)) {
                printf("Error: Invalid input, try again\n");
                continue;
            }

            if (arr_sort_choice < 1 || arr_sort_choice > array_cnt) {
                printf("Error: Invalid array number\n");
                break;
            }

            if (is_size_zero(arrays[arr_sort_choice - 1])) {
                printf("Error: Array is empty, add some elements\n");
                break;
            }

            printf("Source array: ");
            da_print(arrays[arr_sort_choice - 1]);
            printf("\n");

            da_sort(arrays[arr_sort_choice - 1]);
            printf("Sorted array: ");
            da_print(arrays[arr_sort_choice - 1]);
            printf("\n");

            break;
        
        case 5: // Apply 'map' to the array
            // if (da == NULL) {
            //     printf("Array is empty");
            //     break;
            // }
            if (array_cnt == 0) {
                printf("You have 0 arrays");
                printf("\n");
                break;
            }

            int arr_map_choice;
            printf("\nChoice the array, avaivable arrays: \n");

            for (int i = 0; i < array_cnt; i++) {
                printf("Array %d: ", i + 1);
                da_print(arrays[i]);
                printf("\n");
            }

            printf("Choice: ");

            if (!read_one(INPUT_INT, &arr_map_choice)) {
                printf("Error: Invalid input, try again\n");
                continue;
            }

            if (arr_map_choice < 1 || arr_map_choice > array_cnt) {
                printf("Error: Invalid array number\n");
                break;
            }

            if (is_size_zero(arrays[arr_map_choice - 1])) {
                printf("Error: Array is empty, add some elements\n");
                break;
            }

            printf("Avaivable functions for 'map':\n");
            printf("1. Square\n");
            printf("Choice: ");

            int map_choice;

            if (!read_one(INPUT_INT, &map_choice)) {
                printf("Error: Invalid input, try again\n");
                continue;
            }

            if (map_choice != 1) {
                printf("Error: Invalid option number\n");
                break;
            } 

            switch (map_choice)
            {
            case 1: // Apply square
                printf("Source array: ");
                da_print(arrays[arr_map_choice - 1]);
                printf("\n");

                DynamicArray* map_da = da_map(arrays[arr_map_choice - 1], arrays[arr_map_choice - 1]->fieldinfo->square);
                printf("Mapped array: ");
                da_print(map_da);
                printf("\n");

                da_free(map_da);
                
                break;
            
            default:
                break;
            }
        
            break;
        
        case 6: // where
            // if (da == NULL) {
            //     printf("Array is empty");
            //     break;
            // }
            
            if (array_cnt == 0) {
                printf("You have 0 arrays");
                printf("\n");
                break;
            }

            int arr_where_choice;
            printf("\nChoice the array, avaivable arrays: \n");

            for (int i = 0; i < array_cnt; i++) {
                printf("Array %d: ", i + 1);
                da_print(arrays[i]);
                printf("\n");
            }

            printf("Choice: ");

            if (!read_one(INPUT_INT, &arr_where_choice)) {
                printf("Error: Invalid input, try again\n");
                continue;
            }

            if (arr_where_choice < 1 || arr_where_choice > array_cnt) {
                printf("Error: Invalid array number\n");
                break;
            }

            if (is_size_zero(arrays[arr_where_choice - 1])) {
                printf("Error: Array is empty, add some elements\n");
                break;
            }

            printf("Avaivable predicates:\n");
            printf("1. 'Is positive'\n");
            printf("Choice: ");

            int where_choice;

            if (!read_one(INPUT_INT, &where_choice)) {
                printf("Error: Invalid input, try again\n");
                continue;
            }

            if (where_choice != 1) {
                printf("Error: Invalid option number\n");
                break;
            }    

            switch (where_choice)
            {
            case 1: // Use predicate 'Is positive'
                printf("Source array: ");
                da_print(arrays[arr_where_choice - 1]);
                printf("\n");

                DynamicArray* where_da = da_where(arrays[arr_where_choice - 1], arrays[arr_where_choice - 1]->fieldinfo->is_positive);
                printf("Array after using predicate 'Is positive': ");
                da_print(where_da);
                printf("\n");

                da_free(where_da);

                break;
            
            default:
                break;
            }

            break;
        
        case 7: // Concatenation

            if (array_cnt == 0) {
                printf("You have 0 arrays");
                printf("\n");
                break;
            }

            int arr_concat_choice_1;
            int arr_concat_choice_2;
            printf("\nChoice 2 arrays for concatenation, avaivable arrays: \n");

            for (int i = 0; i < array_cnt; i++) {
                printf("Array %d: ", i + 1);
                da_print(arrays[i]);
                printf("\n");
            }

            printf("Number of first array: ");
            if (!read_one(INPUT_INT, &arr_concat_choice_1)) {
                printf("Error: Invalid input, try again\n");
                continue;
            }
            printf("\n");

            if (arr_concat_choice_1 < 1 || arr_concat_choice_1 > array_cnt) {
                printf("Error: Invalid array number\n");
                break;
            }

            if (is_size_zero(arrays[arr_concat_choice_1 - 1])) {
                printf("Error: Array is empty, add some elements\n");
                break;
            }
            
            printf("Number of second array: ");
            if (!read_one(INPUT_INT, &arr_concat_choice_2)) {
                printf("Error: Invalid input, try again\n");
                continue;
            }
            printf("\n");

            if (arr_concat_choice_2 < 1 || arr_concat_choice_2 > array_cnt) {
                printf("Error: Invalid array number\n");
                break;
            }

            if (is_size_zero(arrays[arr_concat_choice_2 - 1])) {
                printf("Error: Array is empty, add some elements\n");
                break;
            }

            printf("Source arrays: ");
            da_print(arrays[arr_concat_choice_1 - 1]);
            printf("| ");
            da_print(arrays[arr_concat_choice_2 - 1]);
            printf("\n");

            DynamicArray* concat_da = da_concat(arrays[arr_concat_choice_1 - 1], arrays[arr_concat_choice_2 - 1]);
            if (concat_da != NULL) {
                printf("Concatenated array: ");
                da_print(concat_da);
                printf("\n");

                da_free(concat_da);
            }

            break;

        case 8:
            
            test_where();

            break;
        case 0: // Exit programm 
            for (int i = 0; i < array_cnt; i++) {
                if (arrays[i] != NULL) {
                    da_free(arrays[i]);
                }
            }

            return;

        default:
            break;
        }
    }
}


int main() {

    menu();
  
    return 0;
}

   // FieldInfo int_info = {
    //     .elem_size = sizeof(int),
    //     .compare = compare_int,
    //     .print = print_int
    // };

    // FieldInfo double_info = {
    //     .elem_size = sizeof(double),
    //     .compare = compare_double,
    //     .print = print_double
    // };

    // DynamicArray* a = da_create(&double_info);
    // DynamicArray* b = da_create(&double_info);

    // int n = 5;
    
    // double c = -5.0;
    // // printf("Сколько чисел хотите ввести? ");
    // // scanf("%d", &n);

    // for (int i = 0; i < n; i++) {
    //     double val_1 = i + n;
    //     double val_2 = i + 2 * n;
    //     // scanf("%lf", &value);
    //     da_append(a, &val_1);
    //     da_append(b, &val_2);
    // }
    // da_append(a, &c);

    // da_print(a);
    // printf("\n");
    // da_print(b);
    // printf("\n");

    // DynamicArray* da = da_concat(a, b);
    // da_print(da);
    // printf("\n");

    // DynamicArray* map_da = da_map(da, square_double);
    // da_print(map_da);
    // printf("\n");

    // DynamicArray* where_da = da_where(da, is_positive);
    // da_print(where_da);
    // printf("\n");

    // da_sort(da);
    // da_print(da);
    // printf("\n");
    // // for (int i = 0; i < da->size; i++) {
    // //     printf("%lf ", *(double*)da_get(da, i)); // Вычисляем адрес через char, потом приводим к нужному типу указателя и разыменовываем
    // // }
    // da_free(a);
    // da_free(b);
    // da_free(da);
    // da_free(map_da);
    // da_free(where_da);

    // test_append();
    // test_concat();
    // test_where();
    // =========



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