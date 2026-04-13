#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "dynamic_array.h"
#include "int_field_info.h"
#include "double_field_info.h"
#include "dynamic_array_field_info.h"
#include "tests.h"
#include "utils.h"

static void test_append() {

    printf("\n--- START Append test ---\n");

    FieldInfo* double_info = get_double_field_info();

    DynamicArray* da = da_create(double_info);

    int capacity = 6;

    for (int i = 0; i < capacity; i++) {
        double value = i + capacity;
        da_append(da, &value);

        assert(*(double*)da_get(da, i) == value);
    }

    assert(da->size == 6);

    printf("Doubles array after appending: ");
    da_print(da);
    printf("\n");

    da_free(da);

    FieldInfo* int_info = get_int_field_info();

    DynamicArray* int_da = da_create(int_info);

    for (int i = 0; i < capacity; i++) {
        int value = i + capacity;
        da_append(int_da, &value);

        assert(*(int*)da_get(int_da, i) == value);
    }

    assert(int_da->size == 6);

    printf("Integers array after appending: ");
    da_print(int_da);
    printf("\n");

    da_free(int_da);

    printf("--- Append test PASSED ---\n");
}

static void test_create() {

    printf("\n--- START Create test ---\n");

    FieldInfo* int_info = get_int_field_info();
    FieldInfo* double_info = get_double_field_info();
    FieldInfo* da_info = get_dynamic_array_field_info();

    DynamicArray* int_da = da_create(int_info);
    assert(int_da != NULL);
    assert(int_da->size == 0);
    assert(int_da->capacity > 0);
    assert(int_da->fieldinfo == int_info);
    assert(int_da->data != NULL);

    DynamicArray* double_da = da_create(double_info);
    assert(double_da != NULL);
    assert(double_da->size == 0);
    assert(double_da->capacity > 0);
    assert(double_da->fieldinfo == double_info);
    assert(double_da->data != NULL);

    DynamicArray* nested_da = da_create(da_info);
    assert(nested_da != NULL);
    assert(nested_da->size == 0);
    assert(nested_da->capacity > 0);
    assert(nested_da->fieldinfo == da_info);
    assert(nested_da->data != NULL);

    printf("All three types of arrays successfully created!\n");

    da_free(int_da);
    da_free(double_da);
    da_free(nested_da);

    printf("--- Create test PASSED ---\n");
}

static void test_append_grow() {

    printf("\n--- START Append grow test ---\n");

    FieldInfo* int_info = get_int_field_info();

    DynamicArray* da = da_create(int_info);

    size_t start_capacity = da->capacity;

    int count = (int)start_capacity * 4 + 3;

    for (int i = 0; i < count; i++) {
        int value = i;
        da_append(da, &value);
    }

    assert(da->size == (size_t)count);
    assert(da->capacity >= da->size);

    for (int i = 0; i < count; i++) {
        int elem = *(int*)da_get(da, i);
        assert(elem == i);
    }

    printf("Start capacity - %zu\nSize after growth - %zu\nCapacity after growth - %zu\n", start_capacity, da->size, da->capacity);

    da_free(da);

    printf("--- Append grow test PASSED ---\n");
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

static void test_nested_append() {

    printf("\n--- START Nested append test ---\n");

    FieldInfo* da_info = get_dynamic_array_field_info();
    FieldInfo* int_info = get_int_field_info();

    DynamicArray* outer = da_create(da_info);

    int count = 5;

    for (int i = 0; i < count; i++) {
        DynamicArray* inner = da_create(int_info);

        for (int j = 0; j <= i; j++) {
            int value = j + 1;
            da_append(inner, &value);
        }

        da_append(outer, &inner);

        DynamicArray* got = *(DynamicArray**)da_get(outer, i);
        assert(got == inner);
        assert(got->size == (size_t)(i + 1));
    }

    assert(outer->size == (size_t)count);

    printf("Nested array: ");
    da_print(outer);
    printf("\n");

    da_free(outer); // da_free каскадно очистит внутренние массивы через destroy_dynamic_array

    printf("--- Nested append test PASSED ---\n");
}

static void test_nested_concat(DynamicArray* src_da) {

    printf("\n--- START Nested concatenation test ---\n");

    FieldInfo* int_info = get_int_field_info();

    DynamicArray* first_da = da_create(src_da->fieldinfo);
    DynamicArray* second_da = da_create(src_da->fieldinfo);

    // Создаем НОВЫЕ внутренние массивы, чтобы не ломать владение указателями в src_da
    for (int i = 0; i < src_da->size; i++) {
        DynamicArray* src_inner = *(DynamicArray**)da_get(src_da, i);

        DynamicArray* copy_first = da_create(int_info);
        DynamicArray* copy_second = da_create(int_info);

        for (int j = 0; j < src_inner->size; j++) {
            da_append(copy_first, da_get(src_inner, j));
            da_append(copy_second, da_get(src_inner, j));
        }

        da_append(first_da, &copy_first);
        da_append(second_da, &copy_second);
    }

    DynamicArray* concat_da = da_concat(first_da, second_da);

    assert(concat_da->size == first_da->size + second_da->size);

    for (int i = 0; i < first_da->size; i++) {
        DynamicArray* elem_in_concat_da = *(DynamicArray**)da_get(concat_da, i);
        DynamicArray* elem_in_first_da = *(DynamicArray**)da_get(first_da, i);

        assert(elem_in_concat_da == elem_in_first_da);
    }

    int g = 0;

    for (int i = first_da->size; i < concat_da->size; i++) {
        DynamicArray* elem_in_concat_da = *(DynamicArray**)da_get(concat_da, i);
        DynamicArray* elem_in_second_da = *(DynamicArray**)da_get(second_da, g);

        assert(elem_in_concat_da == elem_in_second_da);
        g++;
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

    // concat_da хранит те же указатели на внутренние массивы, что и first_da/second_da.
    // Чтобы не получить double-free, чистим первые два через сброс size — данные разрушит concat_da.
    first_da->size = 0;
    second_da->size = 0;

    da_free(first_da);
    da_free(second_da);
    da_free(concat_da);

    printf("--- Nested concatenation test PASSED ---\n");
}

static void test_nested_where(DynamicArray* src_da) {

    printf("\n--- START Nested where test ---\n");

    printf("-- Using predicate 'Is_positive' --\n");

    DynamicArray* where_pos_da = da_where(src_da, src_da->fieldinfo->is_positive);

    size_t positive_cnt = 0;

    for (int i = 0; i < src_da->size; i++) {
        if (src_da->fieldinfo->is_positive(da_get(src_da, i))) {
            positive_cnt++;
        }
    }

    for (int i = 0; i < where_pos_da->size; i++) {
        DynamicArray* elem_in_where_pos_da = *(DynamicArray**)da_get(where_pos_da, i);

        assert(elem_in_where_pos_da->size > 0);
    }

    printf("Source array: ");
    da_print(src_da);
    printf("\n");

    printf("Array with used predicate 'Is positive': ");
    da_print(where_pos_da);
    printf("\n");

    printf("Count of accepted elements - %zu\nSize of resulting array - %zu\n", positive_cnt, where_pos_da->size);
    assert(where_pos_da->size == positive_cnt);

    where_pos_da->size = 0; // Разделяемые указатели принадлежат src_da
    da_free(where_pos_da);

    printf("\n-- Using predicate 'Is negative' --\n");

    DynamicArray* where_neg_da = da_where(src_da, src_da->fieldinfo->is_negative);

    size_t negative_cnt = 0;

    for (int i = 0; i < src_da->size; i++) {
        if (src_da->fieldinfo->is_negative(da_get(src_da, i))) {
            negative_cnt++;
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
    assert(where_neg_da->size == 0);

    da_free(where_neg_da);

    printf("--- Nested where test PASSED ---\n");
}

static void test_nested_map(DynamicArray* src_da) {

    printf("\n--- START Nested map test ---\n");

    printf("-- Using function 'square' --\n");

    DynamicArray* map_sqrt_da = da_map(src_da, src_da->fieldinfo->square);

    assert(map_sqrt_da->size == src_da->size);

    for (int i = 0; i < src_da->size; i++) {
        DynamicArray* elem_in_map_sqrt_da = *(DynamicArray**)da_get(map_sqrt_da, i);
        DynamicArray* elem_in_src_da = *(DynamicArray**)da_get(src_da, i);

        assert(elem_in_map_sqrt_da->size == elem_in_src_da->size);

        for (int j = 0; j < elem_in_src_da->size; j++) {
            int src_val = *(int*)da_get(elem_in_src_da, j);
            int mapped_val = *(int*)da_get(elem_in_map_sqrt_da, j);

            assert(mapped_val == src_val * src_val);
        }
    }

    printf("Source array: ");
    da_print(src_da);
    printf("\n");

    printf("Mapped array: ");
    da_print(map_sqrt_da);
    printf("\n");

    da_free(map_sqrt_da); // square_dynamic_array создает новые внутренние массивы — владение полностью у map_sqrt_da

    printf("-- Using function 'reverse' --\n");

    DynamicArray* map_rev_da = da_map(src_da, src_da->fieldinfo->reverse);

    assert(map_rev_da->size == src_da->size);

    for (int i = 0; i < src_da->size; i++) {
        DynamicArray* elem_in_map_rev_da = *(DynamicArray**)da_get(map_rev_da, i);
        DynamicArray* elem_in_src_da = *(DynamicArray**)da_get(src_da, i);

        assert(elem_in_map_rev_da->size == elem_in_src_da->size);

        for (int j = 0; j < elem_in_src_da->size; j++) {
            int src_val = *(int*)da_get(elem_in_src_da, j);
            int mapped_val = *(int*)da_get(elem_in_map_rev_da, j);

            assert(mapped_val == -(src_val));
        }
    }

    printf("Source array: ");
    da_print(src_da);
    printf("\n");

    printf("Mapped array: ");
    da_print(map_rev_da);
    printf("\n");

    da_free(map_rev_da);

    printf("--- Nested map test PASSED ---\n");
}

static void test_nested_sort(DynamicArray* src_da) {

    printf("\n--- START Nested sort test ---\n");

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

    for (int i = 1; i < sort_da->size; i++) {
        DynamicArray* current_elem = *(DynamicArray**)da_get(sort_da, i);
        DynamicArray* previous_elem = *(DynamicArray**)da_get(sort_da, i - 1);

        assert(current_elem->size >= previous_elem->size);
    }

    sort_da->size = 0; // Разделяемые указатели принадлежат src_da
    da_free(sort_da);

    printf("--- Nested sort test PASSED ---\n");
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

static void fill_nested_nonempty(DynamicArray* da, int count) {

    FieldInfo* int_info = get_int_field_info();

    for (int i = 1; i <= count; i++) {
        DynamicArray* inner = da_create(int_info);

        for (int j = 1; j <= i; j++) {
            int elem = j;
            da_append(inner, &elem);
        }

        da_append(da, &inner);
    }

    printf("Array is completely filled with non-empty nested arrays!\n");
}

static void fill_nested_empty(DynamicArray* da, int count) {

    FieldInfo* int_info = get_int_field_info();

    for (int i = 1; i <= count; i++) {
        DynamicArray* inner = da_create(int_info);

        da_append(da, &inner);
    }

    printf("Array is completely filled with empty nested arrays!\n");
}

static void fill_nested_mixed(DynamicArray* da, int count) {

    FieldInfo* int_info = get_int_field_info();

    for (int i = 1; i <= count; i++) {
        DynamicArray* inner = da_create(int_info);

        if (i % 2 == 0) {
            for (int j = 1; j <= i; j++) {
                int elem = j;
                da_append(inner, &elem);
            }
        }

        da_append(da, &inner);
    }

    printf("Array is completely filled with empty and non-empty nested arrays!\n");
}

static void fill_nested_decreasing(DynamicArray* da, int count) {

    FieldInfo* int_info = get_int_field_info();

    for (int i = 1; i <= count; i++) {
        DynamicArray* inner = da_create(int_info);

        int inner_size = count - i + 1;

        for (int j = 1; j <= inner_size; j++) {
            int elem = j;
            da_append(inner, &elem);
        }

        da_append(da, &inner);
    }

    printf("Array is completely filled with decreasing-size nested arrays!\n");
}

static void run_auto_tests_nested() {

    FieldInfo* da_info = get_dynamic_array_field_info();

    int arr_dataset_choice;

    printf("Choose the dataset: \n");
    printf("1. Non-empty nested arrays\n");
    printf("2. Empty nested arrays\n");
    printf("3. Mixed empty and non-empty nested arrays\n");
    printf("4. Decreasing-size nested arrays\n");
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

    DynamicArray* da = da_create(da_info);

    switch (arr_dataset_choice)
    {
    case 1:
        fill_nested_nonempty(da, count_elements);
        break;

    case 2:
        fill_nested_empty(da, count_elements);
        break;

    case 3:
        fill_nested_mixed(da, count_elements);
        break;

    case 4:
        fill_nested_decreasing(da, count_elements);
        break;

    default:
        printf("ERROR: Invalid option number\n");
        da_free(da);
        return;
    }

    printf("Generated array: ");
    da_print(da);
    printf("\n\n=== STARTS TESTS ===\n");

    clock_t start = clock();

    test_create();
    test_nested_append();
    test_nested_concat(da);
    test_nested_where(da);
    test_nested_map(da);
    test_nested_sort(da);

    clock_t end = clock();
    printf("|| All tests took: %f sec ||\n", (double)(end - start) / CLOCKS_PER_SEC);

    printf("\n=== ALL TESTS PASSED ===\n");

    da_free(da);
}

void run_auto_tests() {

    FieldInfo* int_info = get_int_field_info();
    FieldInfo* double_info = get_double_field_info();

    int arr_type_choice;
    printf("Choose type of array: \n");
    printf("1. Integers\n");
    printf("2. Doubles\n");
    printf("3. Dynamic arrays (nested)\n");
    printf("Choice: ");

    if (!read_one(INPUT_INT, &arr_type_choice)) {
        printf("ERROR: Invalid choice, try again\n");
        return;
    }

    if (arr_type_choice == 3) {
        run_auto_tests_nested();
        return;
    }

    if (arr_type_choice != 1 && arr_type_choice != 2) {
        printf("ERROR: Invalid option number\n");
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

    test_create();
    test_append();
    test_append_grow();
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

    int is_nested = (arrays[arr_test_index]->fieldinfo == get_dynamic_array_field_info());

    switch (test_choice)
    {
    case 1: // Run all tests
        printf("\n\n=== STARTS TESTS ===\n");

        clock_t start_all = clock();

        if (is_nested) {
            test_nested_concat(arrays[arr_test_index]);
            test_nested_map(arrays[arr_test_index]);
            test_nested_sort(arrays[arr_test_index]);
            test_nested_where(arrays[arr_test_index]);
        } else {
            test_concat(arrays[arr_test_index]);
            test_map(arrays[arr_test_index]);
            test_sort(arrays[arr_test_index]);
            test_where(arrays[arr_test_index]);
        }

        clock_t end_all = clock();
        printf("\n|| All tests took: %f sec ||\n", (double)(end_all - start_all) / CLOCKS_PER_SEC);

        printf("\n=== ALL TESTS PASSED ===\n");
        break;

    case 2: // Concatenation test
        clock_t start_concat = clock();

        if (is_nested) {
            test_nested_concat(arrays[arr_test_index]);
        } else {
            test_concat(arrays[arr_test_index]);
        }

        clock_t end_concat = clock();
        printf("|| Concatenation test took: %f sec ||\n", (double)(end_concat - start_concat) / CLOCKS_PER_SEC);

        break;

    case 3: // Where test
        clock_t start_where = clock();

        if (is_nested) {
            test_nested_where(arrays[arr_test_index]);
        } else {
            test_where(arrays[arr_test_index]);
        }

        clock_t end_where = clock();
        printf("|| Where test took: %f sec ||\n", (double)(end_where - start_where) / CLOCKS_PER_SEC);

        break;

    case 4: // Map test
        clock_t start_map = clock();

        if (is_nested) {
            test_nested_map(arrays[arr_test_index]);
        } else {
            test_map(arrays[arr_test_index]);
        }

        clock_t end_map = clock();
        printf("|| Map test took: %f sec ||\n", (double)(end_map - start_map) / CLOCKS_PER_SEC);

        break;

    case 5: // Sort test
        clock_t start_sort = clock();

        if (is_nested) {
            test_nested_sort(arrays[arr_test_index]);
        } else {
            test_sort(arrays[arr_test_index]);
        }

        clock_t end_sort = clock();
        printf("|| Sort test took: %f sec ||\n", (double)(end_sort - start_sort) / CLOCKS_PER_SEC);

        break;
    
    default:
        printf("ERROR: Invalid option number\n");
        break;
    }
}

