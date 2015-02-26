#include "variable.h"

#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#define LEFT -1
#define RIGHT 1

struct node{
	variable_t *payload;
	int height;
	struct node *kid[2];
};

extern void init_tree();
extern int insert(variable_t*);
extern variable_t* query(char*);
extern variable_t* delete(char*);
extern void print_tree();
extern void deallocate_tree();

#endif
