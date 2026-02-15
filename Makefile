# mingw32-make для сборки на Windows
# make для сборки на Linux
program: main.o menu.o double_field_info.o int_field_info.o dynamic_array.o utils.o tests.o
	gcc main.o menu.o double_field_info.o int_field_info.o dynamic_array.o utils.o tests.o -o program

main.o: main.c menu.h
	gcc -c main.c

menu.o: menu.c menu.h double_field_info.h int_field_info.h dynamic_array.h utils.h tests.h
	gcc -c menu.c

double_field_info.o: double_field_info.c double_field_info.h
	gcc -c double_field_info.c

int_field_info.o: int_field_info.c int_field_info.h
	gcc -c int_field_info.c

dynamic_array.o: dynamic_array.c dynamic_array.h
	gcc -c dynamic_array.c

utils.o: utils.c utils.h
	gcc -c utils.c

tests.o: tests.c tests.h dynamic_array.h int_field_info.h double_field_info.h utils.h
	gcc -c tests.c

clean:
	rm *.o program