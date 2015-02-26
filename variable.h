#include "nodes.h"

#ifndef __VARIABLE_H__
#define __VARIABLE_H__

typedef struct variable_s{
	char *name;
	int capacity, size;
	value_type *list;
}variable_t;

variable_t* create_variable(char*);
value_type get_item(variable_t*, long);
value_type* get_item_p(variable_t*, long);
value_type* add_item(variable_t*, long);
value_type del_item(variable_t*, long);
void print_variable(variable_t*);
void deallocate_variable(variable_t*);

#endif
