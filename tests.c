#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "dynamic_array.h"
#include "int_field_info.h"
#include "double_field_info.h"
#include "tests.h"
#include "utils.h"

static void test_append() { // ДОДЕЛАТЬ ДЛЯ ИНТОВ

    printf("START test_append\n");

    FieldInfo* double_info = get_double_field_info();

    DynamicArray* da = da_create(double_info);

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

static void test_concat(DynamicArray* src_da) {

    printf("\n--- START Concatenation test ---\n");

    DynamicArray* first_da = da_create(src_da->fieldinfo);
    DynamicArray* second_da = da_create(src_da->fieldinfo);

    for (int i = 0; i < src_da->size; i++) {
        da_append(first_da, da_get(src_da, i));
        da_append(second_da, da_get(src_da, i));
    }

    DynamicArray* concat_da = da_concat(first_da, second_da);

    assert(concat_da->size == first_da->size + second_da->size);

    if (concat_da->fieldinfo == get_int_field_info()) {
        for (int i = 0; i < first_da->size; i++) {
            int elem_in_concat_da = *(int*)da_get(concat_da, i);
            int elem_in_first_da = *(int*)da_get(first_da, i);

            assert(elem_in_concat_da == elem_in_first_da);
        }

        int g = 0;

        for (int i = first_da->size; i < concat_da->size; i++) {
            int elem_in_concat_da = *(int*)da_get(concat_da, i);
            int elem_in_second_da = *(int*)da_get(second_da, g);

            assert(elem_in_concat_da == elem_in_second_da);
            g++;
        }
    } else if (concat_da->fieldinfo == get_double_field_info()) {
        for (int i = 0; i < first_da->size; i++) {
            double elem_in_concat_da = *(double*)da_get(concat_da, i);
            double elem_in_first_da = *(double*)da_get(first_da, i);

            assert(elem_in_concat_da == elem_in_first_da);
        }

        int g = 0;

        for (int i = first_da->size; i < concat_da->size; i++) {
            double elem_in_concat_da = *(double*)da_get(concat_da, i);
            double elem_in_second_da = *(double*)da_get(second_da, g);

            assert(elem_in_concat_da == elem_in_second_da);
            g++;
        }
    }

    printf("First array: ");
    da_print(first_da);
    printf("\n");

    printf("Second array: ");
    da_print(second_da);
    printf("\n");

    printf("Concatenationaly array: ");
    da_print(concat_da);
    printf("\n");

    printf("Size of first array - %zu\nSize of second array - %zu\nSize of concatenation array - %zu\n", first_da->size, second_da->size, concat_da->size);

    da_free(first_da);
    da_free(second_da);
    da_free(concat_da);

    printf("--- Concatenation test PASSED ---\n");
}

static void test_where(DynamicArray* src_da) { 

    printf("\n--- START Where test ---\n");

    printf("-- Using predicate 'Is_positive' --\n");

    DynamicArray* where_pos_da = da_where(src_da, src_da->fieldinfo->is_positive);

    size_t positive_cnt = 0;

    for (int i = 0; i < src_da->size; i++) {
        if (src_da->fieldinfo->is_positive(da_get(src_da, i))) {
            positive_cnt++;
        }
    }

    if (where_pos_da->fieldinfo == get_double_field_info()) {  
        for (int i = 0; i < where_pos_da->size; i++) {
            double elem_in_where_pos_da = *(double*)da_get(where_pos_da, i);

            assert(elem_in_where_pos_da > 0);
        }
    } else if (where_pos_da->fieldinfo == get_int_field_info()) {
        for (int i = 0; i < where_pos_da->size; i++) {
            int elem_in_where_pos_da = *(int*)da_get(where_pos_da, i);

            assert(elem_in_where_pos_da > 0);
        } 
    }

    printf("Source array: ");
    da_print(src_da);
    printf("\n");

    printf("Array with used predicate 'Is positive': ");
    da_print(where_pos_da);
    printf("\n");

    printf("Count of accepted elements - %zu\nSize of resulting array - %zu\n", positive_cnt, where_pos_da->size);
    assert(where_pos_da->size == positive_cnt);

    da_free(where_pos_da);

    printf("\n-- Using predicate 'Is negative' --\n");

    DynamicArray* where_neg_da = da_where(src_da, src_da->fieldinfo->is_negative);

    size_t negative_cnt = 0;

    for (int i = 0; i < src_da->size; i++) {
        if (src_da->fieldinfo->is_negative(da_get(src_da, i))) {
            negative_cnt++;
        }
    }

    if (where_neg_da->fieldinfo == get_double_field_info()) {  
        for (int i = 0; i < where_neg_da->size; i++) {
            double elem_in_where_neg_da = *(double*)da_get(where_neg_da, i);

            assert(elem_in_where_neg_da < 0);
        }
    } else if (where_neg_da->fieldinfo == get_int_field_info()) {
        for (int i = 0; i < where_neg_da->size; i++) {
            int elem_in_where_neg_da = *(int*)da_get(where_neg_da, i);

            assert(elem_in_where_neg_da < 0);
        } 
    }

    printf("Source array: ");
    da_print(src_da);
    printf("\n");

    printf("Array with used predicate 'Is negative': ");
    da_print(where_neg_da);
    printf("\n");

    printf("Count of accepted elements - %zu\nSize of resulting array - %zu\n", negative_cnt, where_neg_da->size);
    assert(where_neg_da->size == negative_cnt);

    da_free(where_neg_da);

    printf("--- Where test PASSED ---\n");
}

static void test_map(DynamicArray* src_da) {

    printf("\n--- START Map test ---\n");

    printf("-- Using function 'square' --\n");

    DynamicArray* map_sqrt_da = da_map(src_da, src_da->fieldinfo->square);

    assert(map_sqrt_da->size == src_da->size);

    if (map_sqrt_da->fieldinfo == get_double_field_info()) {
        for (int i = 0; i < src_da->size; i++) {
            double elem_in_map_sqrt_da = *(double*)da_get(map_sqrt_da, i);
            double elem_in_src_da = *(double*)da_get(src_da, i);

            assert(elem_in_map_sqrt_da == elem_in_src_da * elem_in_src_da);
        }
    } else if (map_sqrt_da->fieldinfo == get_int_field_info()) {
        for (int i = 0; i < src_da->size; i++) {
            int elem_in_map_sqrt_da = *(int*)da_get(map_sqrt_da, i);
            int elem_in_src_da = *(int*)da_get(src_da, i);

            assert(elem_in_map_sqrt_da == elem_in_src_da * elem_in_src_da);
        }
    }

    printf("Source array: ");
    da_print(src_da);
    printf("\n");

    printf("Mapped array: ");
    da_print(map_sqrt_da);
    printf("\n");

    da_free(map_sqrt_da);

    printf("-- Using function 'reverse' --\n");

    DynamicArray* map_rev_da = da_map(src_da, src_da->fieldinfo->reverse);

    assert(map_rev_da->size == src_da->size);

    if (map_rev_da->fieldinfo == get_double_field_info()) {
        for (int i = 0; i < src_da->size; i++) {
            double elem_in_map_rev_da = *(double*)da_get(map_rev_da, i);
            double elem_in_src_da = *(double*)da_get(src_da, i);

            assert(elem_in_map_rev_da == -(elem_in_src_da));
        }
    } else if (map_rev_da->fieldinfo == get_int_field_info()) {
        for (int i = 0; i < src_da->size; i++) {
            int elem_in_map_rev_da = *(int*)da_get(map_rev_da, i);
            int elem_in_src_da = *(int*)da_get(src_da, i);

            assert(elem_in_map_rev_da == -(elem_in_src_da));
        }
    }

    printf("Source array: ");
    da_print(src_da);
    printf("\n");

    printf("Mapped array: ");
    da_print(map_rev_da);
    printf("\n");

    da_free(map_rev_da);

    printf("--- Map test PASSED ---\n");
}

static void test_sort(DynamicArray* src_da) {
    
    printf("\n--- START Sort test ---\n");
    
    DynamicArray* sort_da = da_create(src_da->fieldinfo);

    for (int i = 0; i < src_da->size; i++) {
        da_append(sort_da, da_get(src_da, i));
    }

    da_sort(sort_da);

    printf("Source array: ");
    da_print(src_da);
    printf("\n");

    printf("Sorted array: ");
    da_print(sort_da);
    printf("\n");

    if (sort_da->fieldinfo == get_double_field_info()) {
        for (int i = 1; i < sort_da->size; i++) {
            double current_elem = *(double*)da_get(sort_da, i);
            double previous_elem = *(double*)da_get(sort_da, i - 1);

            assert(current_elem >= previous_elem);
        }
    } else if (sort_da->fieldinfo == get_int_field_info()) {
        for (int i = 1; i < sort_da->size; i++) {
            int current_elem = *(int*)da_get(sort_da, i);
            int previous_elem = *(int*)da_get(sort_da, i - 1);

            assert(current_elem >= previous_elem);
        }
    }

    da_free(sort_da);

    printf("--- Sort test PASSED ---\n");
}

static void fill_positive(DynamicArray* da, int count) {

    if (da->fieldinfo == get_int_field_info()) {
        for (int i = 1; i <= count; i++) {
            int elem = i;
            da_append(da, &elem);
        }

    } else if (da->fieldinfo == get_double_field_info()) {
        for (int i = 1; i <= count; i++) {
            double elem = (double)i / (double)(i + 1);
            da_append(da, &elem);
        }
    }

    printf("Array is completely filled with positive numbers!\n");
}

static void fill_negative(DynamicArray* da, int count) {

    if (da->fieldinfo == get_int_field_info()) {
        for (int i = 1; i <= count; i++) {
            int elem = i * -1;
            da_append(da, &elem);
        }

    } else if (da->fieldinfo == get_double_field_info()) {
        for (int i = 1; i <= count; i++) {
            double elem = (double)(i * -1);
            da_append(da, &elem);
        }
    }

    printf("Array is completely filled with positive numbers!\n");
}

static void fill_even_and_odd(DynamicArray* da, int count) {

    if (da->fieldinfo == get_int_field_info()) {
        for (int i = 1; i <= count; i++) {
            int elem;

            if (i % 2 == 0) {
                elem = i;
            } else {
                elem = i * -1;
            }

            da_append(da, &elem);
        }

    } else if (da->fieldinfo == get_double_field_info()) {
        for (int i = 1; i < count; i++) {
            double elem;

            if (i % 2 == 0) {
                elem = (double)i;
            } else {
                elem = (double)(i * -1);
            }

            da_append(da, &elem);
        }
    }

    printf("Array is completely filled with even and odd positive and negative numbers!\n");
}

static void fill_mixed(DynamicArray* da, int count) {

    if (da->fieldinfo == get_int_field_info()) {
        for (int i = 1; i <= count; i++) {
            int elem;

            if (i <= count / 2) {
                elem = -(count / 2 - i + 1);
            } else {
                elem = i - count / 2;
            }

            da_append(da, &elem);
        }

    } else if (da->fieldinfo == get_double_field_info()) {
        for (int i = 1; i < count; i++) {
            double elem;

            if (i % 2 == 0) {
                elem = (double)i;
            } else {
                elem = (double)(i * -1);
            }

            da_append(da, &elem);
        }
    }
}

void run_auto_tests() { 

    FieldInfo* int_info = get_int_field_info();
    FieldInfo* double_info = get_double_field_info();
    
    int arr_type_choice;
    printf("Choose type of array: \n");
    printf("1. Integers\n");
    printf("2. Doubles\n");
    printf("Choice: ");

    if (!read_one(INPUT_INT, &arr_type_choice)) {
        printf("ERROR: Invalid choice, try again\n");
        return;
    }
    
    FieldInfo* info = (arr_type_choice == 1) ? int_info : double_info;

    int arr_dataset_choice;

    printf("Choose the dataset: \n");
    printf("1. Positive numbers\n");
    printf("2. Negative numbers\n");
    printf("3. Even and odd numbers (positive and negative)\n");
    printf("4. Mixed\n");
    printf("Number of dataset: ");

    if (!read_one(INPUT_INT, &arr_dataset_choice)) {
        printf("ERROR: Invalid choice, try again\n");
        return;
    }

    int count_elements;
    printf("Enter the count of elements: ");

    if (!read_one(INPUT_INT, &count_elements)) {
        printf("ERROR: Invalid input, enter the 1 integer number\n");
        return;
    }
    
    DynamicArray* da = da_create(info);

    switch (arr_dataset_choice)
    {
    case 1:
        fill_positive(da, count_elements);
        break;
    
    case 2:
        fill_negative(da, count_elements);
        break;
    
    case 3:
        fill_even_and_odd(da, count_elements);
        break;
    
    case 4:
        fill_mixed(da, count_elements);
        break;

    default:
        printf("ERROR: Invalid option number\n");
        break;
    }

    printf("Generated array: ");
    da_print(da);
    printf("\n\n=== STARTS TESTS ===\n");

    clock_t start = clock();

    test_concat(da);
    test_where(da);
    test_map(da);
    test_sort(da);

    clock_t end = clock();
    printf("|| All tests took: %f sec ||\n", (double)(end - start) / CLOCKS_PER_SEC);

    printf("\n=== ALL TESTS PASSED ===\n");

    da_free(da);
    return;
} 

void run_manual_tests(DynamicArray** arrays, int array_cnt) {

    if (array_cnt == 0) {
        printf("ERROR: You have 0 arrays\n");
        return;
    }

    int arr_test_choice;
    printf("\nChoice the array, avaivable arrays: \n");

    for (int i = 0; i < array_cnt; i++) {
        printf("Array %d: ", i + 1);
        da_print(arrays[i]);
        printf("\n");
    }

    printf("Choice: ");

    if (!read_one(INPUT_INT, &arr_test_choice)) {
        printf("ERROR: Invalid input, try again\n");
        return;
    }

    if (arr_test_choice < 1 || arr_test_choice > array_cnt) {
        printf("ERROR: Invalid array number\n");
        return;
    }

    int arr_test_index = arr_test_choice - 1;

    if (arrays[arr_test_index]->size == 0) {
        printf("ERROR: Array is empty, add some elements\n");
        return;
    }

    int test_choice;

    printf("Avaivable tests: \n");
    printf("1. Run all tests\n2. Concatenation test\n3. 'Where' test\n4. 'Map' test\n5. Sort test\nChoice: ");

    if (!read_one(INPUT_INT, &test_choice)) {
        printf("ERROR: Invalid choice, try again\n");
        return;
    }

    switch (test_choice)
    {
    case 1: // Run all tests
        printf("\n\n=== STARTS TESTS ===\n");

        clock_t start_all = clock();

        test_concat(arrays[arr_test_index]);
        test_map(arrays[arr_test_index]);
        test_sort(arrays[arr_test_index]);
        test_where(arrays[arr_test_index]);

        clock_t end_all = clock();
        printf("\n|| All tests took: %f sec ||\n", (double)(end_all - start_all) / CLOCKS_PER_SEC);

        printf("\n=== ALL TESTS PASSED ===\n");
        break;
    
    case 2: // Concatenation test
        clock_t start_concat = clock();

        test_concat(arrays[arr_test_index]);

        clock_t end_concat = clock();
        printf("|| Concatenation test took: %f sec ||\n", (double)(end_concat - start_concat) / CLOCKS_PER_SEC);

        break;

    case 3: // Where test
        clock_t start_where = clock();

        test_map(arrays[arr_test_index]);

        clock_t end_where = clock();
        printf("|| Where test took: %f sec ||\n", (double)(end_where - start_where) / CLOCKS_PER_SEC);

        break;
    
    case 4: // Map test
        clock_t start_map = clock();

        test_where(arrays[arr_test_index]);

        clock_t end_map = clock();
        printf("|| Where test took: %f sec ||\n", (double)(end_map - start_map) / CLOCKS_PER_SEC);

        break;
    
    case 5: // Sort test
        clock_t start_sort = clock();

        test_sort(arrays[arr_test_index]);
        clock_t end_sort = clock();
        printf("|| Where test took: %f sec ||\n", (double)(end_sort - start_sort) / CLOCKS_PER_SEC);

        break;
    
    default:
        printf("ERROR: Invalid option number\n");
        break;
    }
}

