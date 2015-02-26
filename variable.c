#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "variable.h"

variable_t* create_variable(char *name){
	variable_t *var;
	if ((var = malloc(sizeof(variable_t))) == NULL){
		yyerror("Memory allocation error.");
		exit(0);
	}
	var->name = name;
	var->capacity = 5;
	var->size = 0;
	if ((var->list = malloc(var->capacity*sizeof(value_type))) == NULL){
		yyerror("Memory allocation error.");
		exit(0);
	}
	return var;
}

value_type get_item(variable_t *var, long index){
	if (index >= var->size){
		yyerror("Index out of bounds.");
		exit(0);
	}
	return var->list[index];
}

//Returns a pointer to the variable var
value_type* get_item_p(variable_t *var, long index){
	if (index >= var->size){
		yyerror("Index out of bounds.");
		exit(0);
	}
	return &var->list[index];
}

value_type* add_item(variable_t *var, long index){
	if (index > var->size){
		yyerror("Index out of bounds.");
		exit(0);
	}
	if (var->size == var->capacity){
		var->capacity *= 2;
		if ((var->list = realloc(var->list, var->capacity*sizeof(value_type))) == NULL){
			yyerror("Memory allocation error.");
			exit(0);
		}
	}
	int i;
	for (i=var->size; i!=index; i--){
		var->list[i] = var->list[i-1];
	}
	var->size++;
	return &var->list[i];
}

value_type del_item(variable_t *var, long index){
	if (index >= var->size){
		yyerror("Index out of bounds.");
		exit(0);
	}
	value_type ret = var->list[index];
	while (index<var->size){
		var->list[index] = var->list[++index];
	}
	var->size--;
	return ret;
}

void print_variable(variable_t *var){
	int i;
	printf ("[");
	for (i=0; i<var->size; i++){
		if (i>0){
			printf(", ");
		}
		if (var->list[i].type == long_type){
			printf("%ld", var->list[i].l);
		}
		else if (var->list[i].type == double_type){
			printf("%f", var->list[i].d);
		}
		else if (var->list[i].type == string_type){
			printf("%s", var->list[i].s);
		}
	}
	printf("]");
}

void deallocate_variable(variable_t *var){
	int i;
	for (i=0; i<var->size; i++){
		if (var->list[i].type==string_type){
			free(var->list[i].s);
		}
	}
	free(var->list);
	free(var);
}
