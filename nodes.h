#ifndef __NODES_H__
#define __NODES_H__

//Types of nodes
typedef enum{
	value_type_node,
	var_type_node,
	operator_type_node,
	block_type_node
}node_type;

//Types of literals
typedef enum{
	long_type,
	double_type,
	string_type
}value_val_type;

//Literal node content.
typedef struct{
	value_val_type type;
	union{
		long l;
		double d;
		char *s;
	};
}value_type;

//Variable node content.
typedef struct{
	//The name is used to access the AVL Tree.
	char *name;
}var_type;

//Operator node content
typedef struct{
    int operator;                         
    int num_operators;                  
    struct AST_node_s **operators;  
}operator_type;

typedef struct{
	int num_lines;
	struct AST_node_s **lines;
}block_type;

typedef struct AST_node_s{
	node_type type;
	//Nodes are single typed, therefore, union.
	union{
		value_type value;
		var_type variable;
		operator_type operator;
		block_type block;
	};
}AST_node_t;

extern AST_node_t* value_l(long value);
extern AST_node_t* value_d(double value);
extern AST_node_t* value_s(char *value);
extern AST_node_t* variable(char *name);
extern AST_node_t* operator(int operator, int num_operators, ...);
extern AST_node_t* block(AST_node_t *line);
extern AST_node_t* add_line(AST_node_t *block, AST_node_t *line);
extern void execute(AST_node_t *node);
extern void dellocate_node(AST_node_t *node);

#endif
