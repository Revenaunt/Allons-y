#include <stdio.h>
#include "variable.h"

int main(){/*
	const unsigned char mask[] = {1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7};
	int i;
	
	for (i=0; i<8; i++)
		printf("%d\n", mask[i]);
	
	printf ("%d, %d, %d, %d\n", sizeof(char), sizeof(char*), sizeof(int), sizeof(int*));
	printf ("%d, %d, %d, %d\n", sizeof(long), sizeof(long*), sizeof(float), sizeof(float*));*/
	
	variable_t *var = create_variable(INTEGER_ARRAY);
	print_variable(var);
	add_item_i(var, 1);
	add_item_i(var, 5);
	add_item_i(var, 6);
	print_variable(var);
	add_item_i(var, 3);
	add_item_i(var, 5);
	add_item_i(var, 0);
	print_variable(var);
	deallocate_variable(var);
	
	return 0;
}
