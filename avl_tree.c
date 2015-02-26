#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl_tree.h"
#include "variable.h"

//internally, nnil is the new nul
struct node avldummy={NULL, 0, {&avldummy, &avldummy}}, *nnil=&avldummy;
struct node *tree;

struct node *new_node(variable_t *value){
	struct node *n = calloc(1, sizeof *n);
	*n = (struct node) {value, 1, {nnil, nnil}};
	return n;
}

int max(int a, int b) { return a > b ? a : b; }

inline void set_height(struct node *n){
	n->height = 1 + max(n->kid[0]->height, n->kid[1]->height);
}

inline int ballance(struct node *n){
	return n->kid[0]->height - n->kid[1]->height;
}

// rotate a subtree according to dir; if new root is nil, old root is freed
struct node * rotate(struct node **rootp, int dir){
	struct node *old_r = *rootp, *new_r = old_r->kid[dir];
 
	if (nnil == (*rootp = new_r))
		free(old_r);
	else {
		old_r->kid[dir] = new_r->kid[!dir];
		set_height(old_r);
		new_r->kid[!dir] = old_r;
	}
	return new_r;
}

void adjust_balance(struct node **rootp){
	struct node *root = *rootp;
	int b = ballance(root)/2;
	if (b){
		int dir = (1 - b)/2;
		if (ballance(root->kid[dir]) == -b)
			rotate(&root->kid[dir], !dir);
		root = rotate(rootp, dir);
	}
	if (root != nnil) set_height(root);
}

void init_tree(void){
	tree = nnil;
}

// find the node that contains value as payload; or returns 0
variable_t* query_i(struct node *root, char *name){
	if (root==nnil){
		return NULL;
	}
	
	int cmp = strcmp(name, root->payload->name);
	return cmp==0
		? root->payload
		: query_i(root->kid[cmp>0], name);
}

variable_t* query(char *name){
	return query_i(tree, name);
}

int insert_i(struct node **rootp, variable_t *value){
	struct node *root = *rootp;
	
	if (root == nnil){
		*rootp = new_node(value);
		return 1;
	}
	else{
		int cmp = strcmp(value->name, root->payload->name);
		if (cmp==0){ // don't allow dup keys
			return 0;
		}
		else{
			int stat = insert_i(&root->kid[cmp>0], value);
			adjust_balance(rootp);
			return stat;
		}
	}
}

int insert(variable_t *var){
	return insert_i(&tree, var);
}

variable_t* delete_i(struct node **rootp, char *name){
	variable_t *var;
	struct node *root = *rootp;
	if (root == nnil) return NULL; // not found
	
	int cmp = strcmp(root->payload->name, name);
	// if this is the node we want, rotate until off the tree
	if (!cmp){
		var = root->payload;
		if (nnil == (root = rotate(rootp, ballance(root) < 0)))
			return var;
	}
	
	var = delete_i(&root->kid[cmp>0], name);
	adjust_balance(rootp);
	return var;
}

variable_t* delete(char *name){
	return delete_i(&tree, name);
}

void deallocate_tree(){
	variable_t *var;
	while (tree != nnil){
		var = delete(tree->payload->name);
		deallocate_variable(var);
	}
}

// aux display and verification routines, helpful but not essential
struct trunk {
	struct trunk *prev;
	char * str;
};

void show_trunks(struct trunk *p){
	if (!p) return;
	show_trunks(p->prev);
	printf("%s", p->str);
}

// this is very haphazzard
void show_tree(struct node *root, struct trunk *prev, int is_left){
	if (root == nnil) return;
 
	struct trunk this_disp = { prev, "    " };
	char *prev_str = this_disp.str;
	show_tree(root->kid[0], &this_disp, 1);
 
	if (!prev)
		this_disp.str = "---";
	else if (is_left) {
		this_disp.str = ".--";
		prev_str = "   |";
	} else {
		this_disp.str = "`--";
		prev->str = prev_str;
	}
 
	show_trunks(&this_disp);
	printf("%s\n", root->payload->name);
 
	if (prev) prev->str = prev_str;
	this_disp.str = "   |";
 
	show_tree(root->kid[1], &this_disp, 0);
	if (!prev) puts("");
}

int verify(struct node *p){
	if (p == nnil) return 1;

	int h0 = p->kid[0]->height, h1 = p->kid[1]->height;
	int b = h0 - h1;

	if (p->height != 1 + max(h0, h1) || b < -1 || b > 1) {
		printf("node %s bad, balance %d\n", p->payload->name, b);
		show_tree(p, 0, 0);
		abort();
	}
	return verify(p->kid[0]) && verify(p->kid[1]);
}

void print_tree(){
	show_tree(tree, 0, 0);
}

/*
int main(void){
	int x;
	struct node *root = nnil;
 
	srand(time(0));
	
	variable_t *var = (variable_t*)malloc(sizeof(variable_t));
	var->name = "abcba";
	insert(&root, var);
	
	var = (variable_t*)malloc(sizeof(variable_t));
	var->name = "jarl";
	insert(&root, var);
	
	var = (variable_t*)malloc(sizeof(variable_t));
	var->name = "panopticon";
	insert(&root, var);
	
	var = (variable_t*)malloc(sizeof(variable_t));
	var->name = "rakataka";
	insert(&root, var);
	
	var = (variable_t*)malloc(sizeof(variable_t));
	var->name = "tiovivo";
	insert(&root, var);
	
	var = (variable_t*)malloc(sizeof(variable_t));
	var->name = "dummy";
	insert(&root, var);
	
	var = (variable_t*)malloc(sizeof(variable_t));
	var->name = "dummy";
	insert(&root, var);
	
	var = (variable_t*)malloc(sizeof(variable_t));
	var->name = "isma";
	insert(&root, var);
 
	puts("Tree is:");
	show_tree(root, 0, 0);
	
	return 0;
}*/
