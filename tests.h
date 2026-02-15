#ifndef TESTS_H
#define TESTS_H

#include "field_info.h"

void run_auto_tests();

void run_manual_tests(DynamicArray** arrays, int array_cnt); // Указатель на указатель, т.к. в меню создается массив указателей на структуру

#endif