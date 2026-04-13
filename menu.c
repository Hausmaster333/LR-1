#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"
#include "double_field_info.h"
#include "int_field_info.h"
#include "menu.h"
#include "utils.h"
#include "tests.h"

static DynamicArray* arrays[10];

static int array_cnt = 0;

static FieldInfo* int_info = NULL; // Константа, присваиваем в menu уже в runtime
static FieldInfo* double_info = NULL;

static void print_main_menu() {
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
}

static void print_arrays_list() {
    for (int i = 0; i < array_cnt; i++) {
        printf("Array %d: ", i + 1);
        da_print(arrays[i]);
        printf("\n");
    }
}

static void menu_create() {
    int create_choice;
    printf("\nWhat types of numbers?\n");
    printf("1. Integers\n2. Doubles\nChoice: ");

    if (!read_one(INPUT_INT, &create_choice)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }

    switch (create_choice)
    {
    case 1: // Int array
        arrays[array_cnt++] = da_create(int_info);
        printf("Succesfull create array for integers!\n");
        break;

    case 2: // Double array
        arrays[array_cnt++] = da_create(double_info);
        printf("Succesfull create array for doubles!\n");
        break;

    default:
        printf("ERROR: Incorrect option, try again\n");
        break;
    }
}

static void menu_append() {
    if (array_cnt == 0) {
        printf("ERROR: You have 0 arrays\n");
        return;
    }

    int arr_app_choice;
    printf("\nChoice the array, avaivable arrays: \n");

    print_arrays_list();

    printf("Choice: ");

    if (!read_one(INPUT_INT, &arr_app_choice)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }

    if (arr_app_choice < 1 || arr_app_choice > array_cnt) {
        printf("ERROR: Invalid array number\n");
        return;
    }

    printf("Enter a number: ");

    if (arrays[arr_app_choice - 1]->fieldinfo == int_info) {
        int i_num;

        if (!read_one(INPUT_INT, &i_num)) {
            printf("ERROR: Invalid input, try again\n");
            return;
        }

        da_append(arrays[arr_app_choice - 1], &i_num);
        printf("Number %d completely added\n", i_num);
    } else if (arrays[arr_app_choice - 1]->fieldinfo == double_info) {
        double d_num;

        if (!read_one(INPUT_DOUBLE, &d_num)) {
            printf("ERROR: Invalid input, try again\n");
            return;
        }

        da_append(arrays[arr_app_choice - 1], &d_num);
        printf("Number %lf completely added\n", d_num);
    }
}

static void menu_print() {
    if (array_cnt == 0) {
        printf("ERROR: You have 0 arrays\n");
        return;
    }

    printf("\n==== Avaivable arrays ====\n");

    print_arrays_list();
}

static void menu_sort() {
    if (array_cnt == 0) {
        printf("ERROR: You have 0 arrays\n");
        return;
    }

    int arr_sort_choice;
    printf("\nChoice the array, avaivable arrays: \n");

    print_arrays_list();

    printf("Choice: ");

    if (!read_one(INPUT_INT, &arr_sort_choice)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }

    if (arr_sort_choice < 1 || arr_sort_choice > array_cnt) {
        printf("ERROR: Invalid array number\n");
        return;
    }

    if (arrays[arr_sort_choice - 1]->size == 0) {
        printf("ERROR: Array is empty, add some elements\n");
        return;
    }

    printf("Source array: ");
    da_print(arrays[arr_sort_choice - 1]);
    printf("\n");

    da_sort(arrays[arr_sort_choice - 1]);
    printf("Sorted array: ");
    da_print(arrays[arr_sort_choice - 1]);
    printf("\n");
}

static void menu_map() {
    if (array_cnt == 0) {
        printf("You have 0 arrays\n");
        return;
    }

    int arr_map_choice;
    printf("\nChoice the array, avaivable arrays: \n");

    print_arrays_list();

    printf("Choice: ");

    if (!read_one(INPUT_INT, &arr_map_choice)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }

    if (arr_map_choice < 1 || arr_map_choice > array_cnt) {
        printf("ERROR: Invalid array number\n");
        return;
    }

    if (arrays[arr_map_choice - 1]->size == 0) {
        printf("ERROR: Array is empty, add some elements\n");
        return;
    }

    printf("Avaivable functions for 'map':\n");
    printf("1. Square\n2. Reverse\nChoice: ");

    int map_choice;

    if (!read_one(INPUT_INT, &map_choice)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }

    printf("Source array: ");
    da_print(arrays[arr_map_choice - 1]);
    printf("\n");

    switch (map_choice)
    {
    case 1: // Apply square

        DynamicArray* map_sqrt_da = da_map(arrays[arr_map_choice - 1], arrays[arr_map_choice - 1]->fieldinfo->square);
        printf("Mapped array: ");
        da_print(map_sqrt_da);
        printf("\n");

        da_free(map_sqrt_da);

        break;

    case 2: // Apply reverse

        DynamicArray* map_rev_da = da_map(arrays[arr_map_choice - 1], arrays[arr_map_choice - 1]->fieldinfo->reverse);
        printf("Mapped array: ");
        da_print(map_rev_da);
        printf("\n");

        da_free(map_rev_da);

        break;

    default:
        printf("ERROR: Invalid option number\n");
        break;
    }
}

static void menu_where() {
    if (array_cnt == 0) {
        printf("You have 0 arrays\n");
        return;
    }

    int arr_where_choice;
    printf("\nChoice the array, avaivable arrays: \n");

    print_arrays_list();

    printf("Choice: ");

    if (!read_one(INPUT_INT, &arr_where_choice)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }

    if (arr_where_choice < 1 || arr_where_choice > array_cnt) {
        printf("ERROR: Invalid array number\n");
        return;
    }

    if (arrays[arr_where_choice - 1]->size == 0) {
        printf("ERROR: Array is empty, add some elements\n");
        return;
    }

    printf("Avaivable predicates:\n");
    printf("1. 'Is positive'\n2. 'Is negative'\nChoice: ");

    int where_choice;

    if (!read_one(INPUT_INT, &where_choice)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }

    printf("Source array: ");
    da_print(arrays[arr_where_choice - 1]);
    printf("\n");

    switch (where_choice)
    {
    case 1: // Use predicate 'Is positive'

        DynamicArray* where_pos_da = da_where(arrays[arr_where_choice - 1], arrays[arr_where_choice - 1]->fieldinfo->is_positive);
        printf("Array after using predicate 'Is positive': ");
        da_print(where_pos_da);
        printf("\n");

        da_free(where_pos_da);

        break;

    case 2: // Use predicate 'Is negative'
        DynamicArray* where_neg_da = da_where(arrays[arr_where_choice - 1], arrays[arr_where_choice - 1]->fieldinfo->is_negative);
        printf("Array after using predicate 'Is negative': ");
        da_print(where_neg_da);
        printf("\n");

        da_free(where_neg_da);

        break;

    default:
        printf("ERROR: Invalid option number\n");
        break;
    }
}

static void menu_concat() {
    if (array_cnt == 0) {
        printf("ERROR: You have 0 arrays\n");
        return;
    }

    int arr_concat_choice_1;
    int arr_concat_choice_2;
    printf("\nChoice 2 arrays for concatenation, avaivable arrays: \n");

    print_arrays_list();

    printf("Number of first array: ");
    if (!read_one(INPUT_INT, &arr_concat_choice_1)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }
    printf("\n");

    if (arr_concat_choice_1 < 1 || arr_concat_choice_1 > array_cnt) {
        printf("ERROR: Invalid array number\n");
        return;
    }

    int arr_concat_index_1 = arr_concat_choice_1 - 1;

    if (arrays[arr_concat_index_1]->size == 0) {
        printf("ERROR: Array is empty, add some elements\n");
        return;
    }

    printf("Number of second array: ");
    if (!read_one(INPUT_INT, &arr_concat_choice_2)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }
    printf("\n");

    if (arr_concat_choice_2 < 1 || arr_concat_choice_2 > array_cnt) {
        printf("ERROR: Invalid array number\n");
        return;
    }

    int arr_concat_index_2 = arr_concat_choice_2 - 1;

    if (arrays[arr_concat_index_2]->size == 0) {
        printf("ERROR: Array is empty, add some elements\n");
        return;
    }

    printf("Source arrays: ");
    da_print(arrays[arr_concat_index_1]);
    printf("| ");
    da_print(arrays[arr_concat_index_2]);
    printf("\n");

    DynamicArray* concat_da = da_concat(arrays[arr_concat_index_1], arrays[arr_concat_index_2]);
    if (concat_da != NULL) {
        printf("Concatenated array: ");
        da_print(concat_da);
        printf("\n");

        da_free(concat_da);
    }
}

static void menu_tests() {
    int tests_choice;
    int in_tests = 1;
    while (in_tests) {

        printf("\n==== Tests menu ====\n");
        printf("1. Automatic tests\n");
        printf("2. Manual tests\n");
        printf("0. Exit\n");
        printf("Choice: ");

        if (!read_one(INPUT_INT, &tests_choice)) {
            printf("ERROR: Invalid choice, try again\n");
            continue;
        }

        switch (tests_choice)
        {
        case 1: // Run auto tests
            run_auto_tests();
            break;

        case 2: // Run manual tests
            run_manual_tests(arrays, array_cnt);
            break;

        case 0: // Exit
            in_tests = 0;
            break;

        default:
            printf("ERROR: Invalid option number\n");
            break;
        }

    }
}

static void menu_exit() {
    for (int arr_num = 0; arr_num < array_cnt; arr_num++) {
        if (arrays[arr_num] != NULL) {
            da_free(arrays[arr_num]);
        }
    }

    free(int_info);
    free(double_info);
}

void menu() {
    int main_choice;

    int_info = get_int_field_info();
    double_info = get_double_field_info();

    while (1) {
        print_main_menu();

        if (!read_one(INPUT_INT, &main_choice)) {
            printf("ERROR: Invalid input, try again\n");
            continue;
        }

        switch (main_choice)
        {
        case 1: // Create array
            menu_create();
            break;

        case 2: // Append element
            menu_append();
            break;

        case 3: // Print array
            menu_print();
            break;

        case 4: // Sort array
            menu_sort();
            break;

        case 5: // Apply 'map' to the array
            menu_map();
            break;

        case 6: // where
            menu_where();
            break;

        case 7: // Concatenation
            menu_concat();
            break;

        case 8: // Run tests
            menu_tests();
            break;

        case 0: // Exit programm
            menu_exit();
            return;

        default:
            printf("ERROR: Invalid option number\n");
        } // switch
    } // while
} // menu
