#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"
#include "double_field_info.h"
#include "int_field_info.h"
#include "menu.h"
#include "utils.h"
#include "tests.h"

void menu() {
    int main_choice;

    DynamicArray* arrays[10];
    int array_cnt = 0;

    FieldInfo* int_info = get_int_field_info();

    FieldInfo* double_info = get_double_field_info();

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
            printf("ERROR: Invalid input, try again\n");
            continue;
        }

        switch (main_choice)
        {
        case 1: // Create array
            int create_choice;
            printf("\nWhat types of numbers?\n");
            printf("1. Integers\n2. Doubles\nChoice: ");

            if (!read_one(INPUT_INT, &create_choice)) {
                printf("ERROR: Invalid input, try again\n");
                continue;
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

            break;

        case 2: // Append element
            
            if (array_cnt == 0) {
                printf("ERROR: You have 0 arrays\n");
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
                printf("ERROR: Invalid input, try again\n");
                continue;
            }

            if (arr_app_choice < 1 || arr_app_choice > array_cnt) {
                printf("ERROR: Invalid array number\n");
                break;
            }

            printf("Enter a number: ");

            if (arrays[arr_app_choice - 1]->fieldinfo == int_info) {
                int i_num;
                
                if (!read_one(INPUT_INT, &i_num)) {
                    printf("ERROR: Invalid input, try again\n");
                    continue;
                }

                da_append(arrays[arr_app_choice - 1], &i_num);
                printf("Number %d completely added\n", i_num);
            } else if (arrays[arr_app_choice - 1]->fieldinfo == double_info) {
                double d_num;
                
                if (!read_one(INPUT_DOUBLE, &d_num)) {
                    printf("ERROR: Invalid input, try again\n");
                    continue;
                }

                da_append(arrays[arr_app_choice - 1], &d_num);
                printf("Number %lf completely added\n", d_num);
            }

            break;
        
        case 3: // Print array

            if (array_cnt == 0) {
                printf("ERROR: You have 0 arrays\n");
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

            if (array_cnt == 0) {
                printf("ERROR: You have 0 arrays\n");
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
                printf("ERROR: Invalid input, try again\n");
                continue;
            }

            if (arr_sort_choice < 1 || arr_sort_choice > array_cnt) {
                printf("ERROR: Invalid array number\n");
                break;
            }

            if (arrays[arr_sort_choice - 1]->size == 0) {
                printf("ERROR: Array is empty, add some elements\n");
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

            if (array_cnt == 0) {
                printf("You have 0 arrays\n");
                break;
            }

            int arr_map_choice;
            printf("\nChoice the array, avaivable arrays: \n");

            for (int arr_num = 0; arr_num < array_cnt; arr_num++) {
                printf("Array %d: ", arr_num + 1);
                da_print(arrays[arr_num]);
                printf("\n");
            }

            printf("Choice: ");

            if (!read_one(INPUT_INT, &arr_map_choice)) {
                printf("ERROR: Invalid input, try again\n");
                continue;
            }

            if (arr_map_choice < 1 || arr_map_choice > array_cnt) {
                printf("ERROR: Invalid array number\n");
                break;
            }

            if (arrays[arr_map_choice - 1]->size == 0) {
                printf("ERROR: Array is empty, add some elements\n");
                break;
            }

            printf("Avaivable functions for 'map':\n");
            printf("1. Square\n2. Reverse\nChoice: ");

            int map_choice;

            if (!read_one(INPUT_INT, &map_choice)) {
                printf("ERROR: Invalid input, try again\n");
                continue;
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
        
            break;
        
        case 6: // where
            
            if (array_cnt == 0) {
                printf("You have 0 arrays\n");
                break;
            }

            int arr_where_choice;
            printf("\nChoice the array, avaivable arrays: \n");

            for (int arr_num = 0; arr_num < array_cnt; arr_num++) {
                printf("Array %d: ", arr_num + 1);
                da_print(arrays[arr_num]);
                printf("\n");
            }

            printf("Choice: ");

            if (!read_one(INPUT_INT, &arr_where_choice)) {
                printf("ERROR: Invalid input, try again\n");
                continue;
            }

            if (arr_where_choice < 1 || arr_where_choice > array_cnt) {
                printf("ERROR: Invalid array number\n");
                break;
            }

            if (arrays[arr_where_choice - 1]->size == 0) {
                printf("ERROR: Array is empty, add some elements\n");
                break;
            }

            printf("Avaivable predicates:\n");
            printf("1. 'Is positive'\n2. 'Is negative'\nChoice: ");

            int where_choice;

            if (!read_one(INPUT_INT, &where_choice)) {
                printf("ERROR: Invalid input, try again\n");
                continue;
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

            break;
        
        case 7: // Concatenation

            if (array_cnt == 0) {
                printf("ERROR: You have 0 arrays\n");
                break;
            }

            int arr_concat_choice_1;
            int arr_concat_choice_2;
            printf("\nChoice 2 arrays for concatenation, avaivable arrays: \n");

            for (int arr_num = 0; arr_num < array_cnt; arr_num++) {
                printf("Array %d: ", arr_num + 1);
                da_print(arrays[arr_num]);
                printf("\n");
            }

            printf("Number of first array: ");
            if (!read_one(INPUT_INT, &arr_concat_choice_1)) {
                printf("ERROR: Invalid input, try again\n");
                continue;
            }
            printf("\n");

            if (arr_concat_choice_1 < 1 || arr_concat_choice_1 > array_cnt) {
                printf("ERROR: Invalid array number\n");
                break;
            }

            int arr_concat_index_1 = arr_concat_choice_1 - 1;

            if (arrays[arr_concat_index_1]->size == 0) {
                printf("ERROR: Array is empty, add some elements\n");
                break;
            }
            
            printf("Number of second array: ");
            if (!read_one(INPUT_INT, &arr_concat_choice_2)) {
                printf("ERROR: Invalid input, try again\n");
                continue;
            }
            printf("\n");

            if (arr_concat_choice_2 < 1 || arr_concat_choice_2 > array_cnt) {
                printf("ERROR: Invalid array number\n");
                break;
            }

            int arr_concat_index_2 = arr_concat_choice_2 - 1;

            if (arrays[arr_concat_index_2]->size == 0) {
                printf("ERROR: Array is empty, add some elements\n");
                break;
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

            break;

        case 8: // Run tests
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

            break;

        case 0: // Exit programm 
            for (int arr_num = 0; arr_num < array_cnt; arr_num++) {
                if (arrays[arr_num] != NULL) {
                    da_free(arrays[arr_num]);
                }
            }

            free(int_info);
            free(double_info);

            return;

        default:
            printf("ERROR: Invalid option number\n");
        } // switch 
    } // while
} // menu