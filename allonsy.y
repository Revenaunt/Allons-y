%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl_tree.h"
#include "mem_manager.h"
#include "nodes.h"

%}

%union{
        long lval;
        double dval;
        char *sval;
        AST_node_t *node;        
}

%token <lval> LONG
%token <dval> DOUBLE
%token <sval> STR VARIABLE
%token IF ELSE WHILE FOR IN
%token READ PRINT NL END

%nonassoc ONLYIF
%nonassoc ELSE
%nonassoc FOREACH


%left OR
%left AND
%left GREATER LOWER GEQ LEQ EQ NEQ
%left '+' '-'
%left '*' '/'
%right '^'
%left '%'
%right INCREMENT DECREMENT
%right MINUS PLUS
%left ADD SET POP GET

%type <node> block line control loop condition assignation declaration expression

%start program

%%

program:
			block					{execute($1); deallocate_node($1); exit(0);}
		;

block:
			block line				{$$ = add_line($1, $2);}
		|	line					{$$ = block($1);}
		;

line:
			assignation	END			{$$ = $1;}
		|	control					{$$ = $1;}
		|	loop					{$$ = $1;}
		|	expression END			{$$ = $1;}
		|	READ VARIABLE END		{$$ = operator(READ, 1, variable($2));}
		|	PRINT expression END	{$$ = operator(PRINT, 1, $2);}
		|	NL END					{$$ = operator(NL, 0);}
		;

control:
			IF '(' condition ')' '{' block '}' %prec ONLYIF			{$$ = operator(IF, 2, $3, $6);}
		|	IF '(' condition ')' '{' block '}' ELSE '{' block '}'		{$$ = operator(IF, 3, $3, $6, $10);}
		;

loop:
			WHILE '(' condition ')' '{' block '}'								{$$ = operator(WHILE, 2, $3, $6);}
		|	FOR '(' declaration ',' condition ',' declaration ')' '{' block '}'	{$$ = operator(FOR, 4, $3, $5, $7, $10);}
		|	FOR '(' VARIABLE IN VARIABLE ')' '{' block '}'						{$$ = operator(FOREACH, 3, variable($3), variable($5), $8);}
		;

condition:
			expression GREATER expression				{$$ = operator(GREATER, 2, $1, $3);}
		|	expression LOWER expression					{$$ = operator(LOWER, 2, $1, $3);}
		|	expression GEQ expression					{$$ = operator(GEQ, 2, $1, $3);}
		|	expression LEQ expression					{$$ = operator(LEQ, 2, $1, $3);}
		|	expression EQ expression					{$$ = operator(EQ, 2, $1, $3);}
		|	expression NEQ expression					{$$ = operator(NEQ, 2, $1, $3);}
		|	condition AND condition						{$$ = operator(AND, 2, $1, $3);}
		|	condition OR condition						{$$ = operator(OR, 2, $1, $3);}
		;

assignation:
			declaration									{$$ = $1;}
		|	VARIABLE '.' ADD '(' expression ')'			{$$ = operator(ADD, 2, variable($1), $5);}
		|	VARIABLE '[' expression ']' '=' expression	{$$ = operator(SET, 3, variable($1), $3, $6);}
		;

declaration:
			VARIABLE '=' expression						{$$ = operator('=', 2, variable($1), $3);}
		;

expression:
			LONG							{$$ = value_l($1);}
		|	DOUBLE							{$$ = value_d($1);}
		|	STR								{$$ = value_s($1);}
		|	VARIABLE						{$$ = variable($1);}
		|	VARIABLE '[' expression ']'		{$$ = operator(GET, 2, variable($1), $3);}
		|	VARIABLE '.' POP '(' ')'		{$$ = operator(POP, 1, variable($1));}
		|	'-' expression %prec MINUS		{$$ = operator(MINUS,1,$2);}
		|	'+' expression  %prec PLUS		{$$ = operator(PLUS,1,$2);}
		|	expression '+' expression		{$$ = operator('+', 2, $1, $3);}
		|	expression '-' expression		{$$ = operator('-', 2, $1, $3);}
		|	expression '*' expression		{$$ = operator('*', 2, $1, $3);}
		|	expression '/' expression		{$$ = operator('/', 2, $1, $3);}
		|	expression '^' expression		{$$ = operator('^', 2, $1, $3);}
		|	expression '%' expression		{$$ = operator('%', 2, $1, $3);}
		|	'(' expression ')'				{$$ = $2;}
		;

%%

int yyerror(char *s){
	printf("%s\n", s);
}

int main(int argc, char *argv[]){
	if (argc==2){
		extern FILE *yyin;
		yyin = fopen(argv[1],"r");
		if (yyin != NULL){
			init_tree();
			init_manager();
			yyparse();
			fclose(yyin);
			deallocate_manager();
			deallocate_tree();
		}
		else{
			printf("The file does not exist.\n");
		}
	}
	else{
		printf("Usage: %s 'filename'\n", argv[0]);
	}
}
