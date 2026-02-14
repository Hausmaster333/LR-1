#include <stdio.h>
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
                arrays[array_cnt++] = da_create(int_info);
                printf("Succesfull create array for integers!\n");
                break;

            case 2: // Double array
                arrays[array_cnt++] = da_create(double_info);
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

            if (arrays[arr_app_choice - 1]->fieldinfo == int_info) {
                int i_num;
                
                if (!read_one(INPUT_INT, &i_num)) {
                    printf("Error: Invalid input, try again\n");
                    continue;
                }

                da_append(arrays[arr_app_choice - 1], &i_num);
                printf("Number %d completely added\n", i_num);
            } else if (arrays[arr_app_choice - 1]->fieldinfo == double_info) {
                double d_num;
                
                if (!read_one(INPUT_DOUBLE, &d_num)) {
                    printf("Error: Invalid input, try again\n");
                    continue;
                }

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

            if (arrays[arr_sort_choice - 1]->size == 0) {
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

            if (arrays[arr_map_choice - 1]->size == 0) {
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

            if (arrays[arr_where_choice - 1]->size == 0) {
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

            if (arrays[arr_concat_choice_1 - 1]->size == 0) {
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

            if (arrays[arr_concat_choice_2 - 1]->size == 0) {
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

            run_tests();

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