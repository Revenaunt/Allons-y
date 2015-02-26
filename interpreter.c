#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"
#include "mem_manager.h"
#include "nodes.h"
#include "y.tab.h"

//Prints a value type.
void print_val(value_type val){
	if (val.type == long_type){
		printf("%ld", val.l);
	}
	else if (val.type == double_type){
		printf("%f", val.d);
	}
	else if (val.type == string_type){
		printf("%s", val.s);
	}
}

//Gets the variable queried, if it doesn't exists throws an error.
variable_t* get_var_err(char *name){
	variable_t *var = query(name);
	if (var == NULL){
		char aux[11+strlen(name)+19];
		sprintf(aux, "\nVariable '%s' does not exist.", name);
		yyerror(aux);
		exit(0);
	}
	return var;
}

//Gets the variable queried, if it does not exist it creates it.
variable_t* get_var(char *name){
	variable_t *var = query(name);
	if (var == NULL){
		var = create_variable(name);
		insert(var);
	}
	return var;
}

//Prints a variable (list)
void print_var(var_type var_node){
	variable_t *var = get_var_err(var_node.name);
	print_variable(var);
}

void declaration(var_type var_node, value_type node){
	char *name = var_node.name;
	variable_t *var = get_var(var_node.name);
	value_type *item;
	if (var->size == 0){
		item = add_item(var, 0);
	}
	else{
		item = get_item_p(var, 0);
		if (item->type==string_type){
			free(item->s);
		}
	}
	item->type = node.type;
	switch (item->type){
		case long_type:		item->l=node.l; break;
		case double_type:	item->d=node.d; break;
		case string_type:
			item->s=(char*)malloc(strlen(node.s)*sizeof(char));
			strcpy(item->s, node.s);
			break;
	}
}

void add_to_end(var_type var_node, value_type node){
	variable_t *var = get_var(var_node.name);
	int index = var->size;
	value_type *item = add_item(var, index);
	item->type = node.type;
	switch (item->type){
		case long_type:		item->l=node.l; break;
		case double_type:	item->d=node.d; break;
		case string_type:
			item->s=(char*)malloc(strlen(node.s)*sizeof(char));
			strcpy(item->s, node.s);
			break;
	}
}

value_type pop(var_type var_node){
	variable_t *var = get_var_err(var_node.name);
	return del_item(var, var->size-1);
}

value_type get(var_type var_node, value_type index){
	if (index.type != long_type){
		yyerror("Cannot use a non-integer as index");
		exit(0);
	}
	else{
		if (index.l < 0){
			yyerror("Index must be a positive integer.");
			exit(0);
		}
		else{
			variable_t *var = get_var_err(var_node.name);
			return get_item(var, index.l);
		}
	}
}

value_type get_first(var_type var_node){
	variable_t *var = get_var_err(var_node.name);
	return get_item(var, 0);
}

void set(var_type var_node, value_type index, value_type value){
	if (index.type != long_type){
		yyerror("Cannot use a non-integer as index");
		exit(0);
	}
	
	if (index.l < 0){
		yyerror("Index must be a positive integer.");
		exit(0);
	}
	
	variable_t *var = get_var_err(var_node.name);
	value_type *item = get_item_p(var, index.l);
	if (item->type==string_type){
		free(item->s);
	}
	item->type = value.type;
	switch (item->type){
		case long_type:		item->l=value.l; break;
		case double_type:	item->d=value.d; break;
		case string_type:
			item->s=(char*)malloc(strlen(value.s)*sizeof(char));
			strcpy(item->s, value.s);
			break;
	}
}

value_type reverse_sign(value_type node){
	if (node.type == string_type){
		yyerror("Illegal operation.");
		exit(0);
	}
	
	if (node.type == long_type){
		node.l = -node.l;
	}
	else if (node.type == double_type){
		node.d = -node.d;
	}
	
	return node;
}

value_type operation(value_type left, int operator, value_type right){
	value_type value;
	//Operation.
	if (left.type == string_type){
		value.type = string_type;
		if (right.type == string_type){
			if (operator == '+'){
				int length = strlen(left.s)+strlen(right.s);
				char *aux=(char*)malloc((length+1)*sizeof(char));
				strcpy(aux, left.s);
				strcat(aux, right.s);
				value.s = aux;
			}
			else{
				yyerror("Illegal operation.");
				exit(0);
			}
		}
		else if (right.type == long_type){
			if (operator == '+'){
				int length = strlen(left.s);
				long val = right.l;
				while (val>0){
					length++;
					val/=10;
				}
				char *aux=(char*)malloc((length+1)*sizeof(char));
				sprintf(aux, "%s%ld", left.s, right.l);
				value.s = aux;
			}
			else{
				yyerror("Illegal operation.");
				exit(0);
			}
		}
		else if (right.type == double_type){
			if (operator == '+'){
				int length = strlen(left.s);
				long val = right.l;
				while (val>=1){
					length++;
					val/=10;
				}
				//4 accounts for 2 decilals, the dot and null character
				char *aux=(char*)malloc((length+4)*sizeof(char));
				sprintf(aux, "%s%.2f", left.s, right.d);
				value.s = aux;
			}
			else{
				yyerror("Illegal operation.");
				exit(0);
			}
		}
	}
	else if (left.type == long_type){
		if (right.type == string_type){
			value.type = string_type;
			if (operator == '+'){
				int length = strlen(right.s);
				long val = left.l;
				while (val>0){
					length++;
					val/=10;
				}
				char *aux=(char*)malloc((length+1)*sizeof(char));
				sprintf(aux, "%ld%s", left.l, right.s);
				value.s = aux;
			}
			else{
				yyerror("Illegal operation.");
				exit(0);
			}
		}
		else if (right.type == long_type){
			value.type = long_type;
			switch (operator){
				case '+':		value.l = left.l+right.l; break;
				case '-':		value.l = left.l-right.l; break;
				case '*':		value.l = left.l*right.l; break;
				case '/':		value.l = left.l/right.l; break;
				case '^':		value.l = (long)pow(left.l, right.l); break;
				case '%':		value.l = left.l%right.l; break;
			}
		}
		else if (right.type == double_type){
			value.type = long_type;
			switch (operator){
				case '+':		value.l = left.l+right.d; break;
				case '-':		value.l = left.l-right.d; break;
				case '*':		value.l = left.l*right.d; break;
				case '/':		value.l = left.l/right.d; break;
				case '^':		value.l = pow(left.l, right.d); break;
				default:		yyerror("Illegal operation."); exit(0);
			}
		}
	}
	else if (left.type == double_type){
		if (right.type == string_type){
			value.type = string_type;
			if (operator == '+'){
				int length = strlen(right.s);
				long val = left.l;
				while (val>=1){
					length++;
					val/=10;
				}
				char *aux=(char*)malloc((length+4)*sizeof(char));
				sprintf(aux, "%.2f%s", left.d, right.s);
				value.s = aux;
			}
			else{
				yyerror("Illegal operation.\n");
			}
		}
		else if (right.type == long_type){
			value.type = double_type;
			switch (operator){
				case '+':		value.d = left.d+right.l; break;
				case '-':		value.d = left.d-right.l; break;
				case '*':		value.d = left.d*right.l; break;
				case '/':		value.d = left.d/right.l; break;
				case '^':		value.d = pow(left.d, right.l); break;
				default:		yyerror("Illegal operation."); exit(0);
			}
		}
		else if (right.type == double_type){
			value.type = long_type;
			switch (operator){
				case '+':		value.l = left.d+right.d; break;
				case '-':		value.l = left.d-right.d; break;
				case '*':		value.l = left.d*right.d; break;
				case '/':		value.l = left.d/right.d; break;
				case '^':		value.l = pow(left.d, right.d); break;
				default:		yyerror("Illegal operation."); exit(0);
			}
		}
	}
	
	return value;
}

value_type comparison(value_type left, int operator, value_type right){
	value_type value;
	value.type = long_type;
	
	//Comparison.
	if (left.type == string_type){
		if (right.type == string_type){
			int result = strcmp(left.s, right.s);
			switch (operator){
				case GREATER:		value.l = result>0; break;
				case LOWER:			value.l = result<0; break;
				case GEQ:			value.l = result>=0; break;
				case LEQ:			value.l = result<=0; break;
				case EQ:			value.l = result==0; break;
				case NEQ:			value.l = result!=0; break;
			}
		}
		else{
			yyerror("Strings and numerical values cannot be compared.\n");
		}
	}
	else{
		if (right.type == string_type){
			yyerror("Numerical values and strings cannot be compared.\n");
		}
		else{
			double left_val = (left.type==long_type)?left.l:left.d;
			double right_val = (right.type==long_type)?right.l:right.d;
			
			switch (operator){
				case GREATER:		value.l = left_val>right_val; break;
				case LOWER:			value.l = left_val<right_val; break;
				case GEQ:			value.l = left_val>=right_val; break;
				case LEQ:			value.l = left_val<=right_val; break;
				case EQ:			value.l = left_val==right_val; break;
				case NEQ:			value.l = left_val!=right_val; break;
			}
		}
	}
	return value;
}

//Execution for result
value_type execute_r(AST_node_t *node){
	value_type result;
	switch (node->type){
		case value_type_node:		return node->value;
		case var_type_node:			return get_first(node->variable);
		case operator_type_node:
			switch (node->operator.operator){
				//Variable related work
				case POP:		return pop(node->operator.operators[0]->variable);
				case GET:		return get(node->operator.operators[0]->variable, execute_r(node->operator.operators[1]));
				
				//Operations
				case MINUS:		return reverse_sign(execute_r(node->operator.operators[0]));
				case PLUS:		return execute_r(node->operator.operators[0]);
				case '^':		return operation(execute_r(node->operator.operators[0]), '^', execute_r(node->operator.operators[1]));
				case '+':		return operation(execute_r(node->operator.operators[0]), '+', execute_r(node->operator.operators[1]));
				case '-':		return operation(execute_r(node->operator.operators[0]), '-', execute_r(node->operator.operators[1]));
				case '*':		return operation(execute_r(node->operator.operators[0]), '*', execute_r(node->operator.operators[1]));
				case '/':		return operation(execute_r(node->operator.operators[0]), '/', execute_r(node->operator.operators[1]));
				case '%':		return operation(execute_r(node->operator.operators[0]), '%', execute_r(node->operator.operators[1]));
				
				//Comparisons
				case GREATER:	return comparison(execute_r(node->operator.operators[0]), GREATER, execute_r(node->operator.operators[1]));
				case LOWER:		return comparison(execute_r(node->operator.operators[0]), LOWER, execute_r(node->operator.operators[1]));
				case GEQ:		return comparison(execute_r(node->operator.operators[0]), GEQ, execute_r(node->operator.operators[1]));
				case LEQ:		return comparison(execute_r(node->operator.operators[0]), LEQ, execute_r(node->operator.operators[1]));
				case EQ:		return comparison(execute_r(node->operator.operators[0]), EQ, execute_r(node->operator.operators[1]));
				case NEQ:		return comparison(execute_r(node->operator.operators[0]), NEQ, execute_r(node->operator.operators[1]));
				
				case AND:
					result.type=long_type;
					result.l=execute_r(node->operator.operators[0]).l&&execute_r(node->operator.operators[1]).l;
					return result;
				
				case OR:
					result.type=long_type;
					result.l=execute_r(node->operator.operators[0]).l||execute_r(node->operator.operators[1]).l;
					return result;
			}
	}
}

//Regular execution
void execute(AST_node_t *node) {
	int i;
    if (!node) return;
    
    variable_t *var;
    variable_t *it;
    char buffer[200];
    
    switch(node->type){
		case block_type_node:
			for (i=0; i<node->block.num_lines; i++){
				execute(node->block.lines[i]);
			}
			break;
		case operator_type_node:
			switch(node->operator.operator){
				case WHILE:
					while (execute_r(node->operator.operators[0]).l){
						execute(node->operator.operators[1]);
					}
					break;
					
				case FOR:
					execute(node->operator.operators[0]);
					while (execute_r(node->operator.operators[1]).l){
						execute(node->operator.operators[3]);
						execute(node->operator.operators[2]);
					}
					break;
				
				case FOREACH:
					var = get_var_err(node->operator.operators[1]->variable.name);
					it = get_var(node->operator.operators[0]->variable.name);
					if (it->size==0){
						it->size++;
					}
					int i;
					for (i=0; i<var->size; i++){
						it->list[0] = var->list[i];
						execute(node->operator.operators[2]);
					}
					break;
					
				case IF:
					if (execute_r(node->operator.operators[0]).l){
						execute(node->operator.operators[1]);
					}
					else if (node->operator.num_operators > 2){
						execute(node->operator.operators[2]);
					}
					break;
				
				case READ:
					var = get_var(node->operator.operators[0]->variable.name);
					if (var->size==0){
						var->size++;
					}
					char *buffer=get_buffer();
					scanf(get_buffer_format(), buffer);
					if (var->list[0].type==string_type){
						free(var->list[0].s);
					}
					else{
						var->list[0].type = string_type;
					}
					var->list[0].s=(char*)malloc((strlen(buffer)+1)*sizeof(char));
					strcpy(var->list[0].s, buffer);
					break;
				
				case PRINT:
					switch (node->operator.operators[0]->type){
						case value_type_node:		print_val(execute_r(node->operator.operators[0])); break;
						case var_type_node:			print_var(node->operator.operators[0]->variable); break;
						case operator_type_node:	print_val(execute_r(node->operator.operators[0])); break;
					}
					break;
				
				case NL:		printf("\n"); break;
				
				//Variable management
				case '=':		declaration(node->operator.operators[0]->variable, execute_r(node->operator.operators[1])); break;
				case ADD:		add_to_end(node->operator.operators[0]->variable, execute_r(node->operator.operators[1])); break;
				case SET:		set(node->operator.operators[0]->variable,
									execute_r(node->operator.operators[1]),
									execute_r(node->operator.operators[2])); break;
		}
	}
}
