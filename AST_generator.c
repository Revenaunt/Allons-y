#include "nodes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//Long values.
AST_node_t* value_l(long value){
	AST_node_t *node;
	
	if ((node = malloc(sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	node->type = value_type_node;
	node->value.type = long_type;
	node->value.l = value;
	
	return node;
}

//Double values.
AST_node_t* value_d(double value){
	AST_node_t *node;
	
	if ((node = malloc(sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	node->type = value_type_node;
	node->value.type = double_type;
	node->value.d = value;
	
	return node;
}

//String values.
AST_node_t* value_s(char *value){
	AST_node_t *node;
	
	if ((node = malloc(sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	node->type = value_type_node;
	node->value.type = string_type;
	node->value.s = value;
	
	return node;
}


//Variables
AST_node_t* variable(char *name){
	AST_node_t *node;
	
	if ((node = malloc(sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	node->type = var_type_node;
	node->variable.name = name;
	
	return node;
}


//Operators
AST_node_t* operator(int operator, int num_operators, ...){
	va_list args;
	AST_node_t *node;
	int i;
	
	if ((node = malloc(sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	if ((node->operator.operators = malloc(num_operators*sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	node->type = operator_type_node;
	node->operator.num_operators = num_operators;
	node->operator.operator = operator;
	
	va_start(args, num_operators);
	for (i=0; i<num_operators; i++){
		node->operator.operators[i] = va_arg(args, AST_node_t*);
	}
	va_end(args);
	
	return node;	
}


//Block
AST_node_t* block(AST_node_t *line){
	AST_node_t *node;
	
	if ((node = malloc(sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	if ((node->block.lines = malloc(sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	node->type = block_type_node;
	node->block.num_lines = 1;
	node->block.lines[0] = line;
	
	return node;
}

//Block grouping
AST_node_t* add_line(AST_node_t *block, AST_node_t *line){
	block->block.num_lines++;
	if ((block->block.lines = realloc(block->block.lines, block->block.num_lines*sizeof(AST_node_t))) == NULL){
		yyerror("Memory allocation error.\n");
	}
	block->block.lines[block->block.num_lines-1] = line;
	
	return block;
}


//Deallocation.
void deallocate_node(AST_node_t *node){
	int i;
	
	if (node != NULL){
		if (node->type == operator_type_node){
			for(i=0; i<node->operator.num_operators; i++){
				deallocate_node(node->operator.operators[i]);
			}
			free(node->operator.operators);
		}
		else if (node->type == block_type_node){
			for(i=0; i<node->block.num_lines; i++){
				deallocate_node(node->block.lines[i]);
			}
			free(node->block.lines);
		}
	}
	
	free(node);
}
