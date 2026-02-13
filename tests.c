#include <stdio.h>
#include <assert.h>
#include "dynamic_array.h"
#include "int_field_info.h"
#include "double_field_info.h"
#include "tests.h"

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

static void test_concat() { // ДОДЕЛАТЬ ДЛЯ ИНТОВ

    printf("START test_concat\n");

    FieldInfo* double_info = get_double_field_info();

    DynamicArray* a = da_create(double_info);
    DynamicArray* b = da_create(double_info);

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

static void test_where() { // ДОДЕЛАТЬ ДЛЯ ИНТОВ 

    printf("START test_where\n");

    FieldInfo* double_info = get_double_field_info(); 

    DynamicArray* da = da_create(double_info);

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

void run_tests() { /* НАПИСАТЬ ТЕСТ ДЛЯ СОРТИРОВКИ НАПИСАТЬ ТЕСТЫ С РУЧНЫМ ВВОДОМ ДАННЫХ, СДЕЛАТЬ РАЗДЕЛЕНИЕ МЕЖДУ АВТОМАТИЧЕСКИМИ ТЕСТАМИ И РУЧНЫМИ*/

    test_append();
    test_concat();
    test_where();
    printf("ALL TESTS PASSED\n");
}