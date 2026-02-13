#include "menu.h"

// void da_square(DynamicArray* da, const void* dest, void* src) {
//     da->fieldinfo->square(dest, src);
// }

// int da_is_positive(DynamicArray* da, const void* elem) {
//     da->fieldinfo->is_positive(elem);
// }

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