%{

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include "nodes.h"
	#include "y.tab.h"
	char buffer_str[200];

%}

CHARACTERS			[a-zA-Z_]
DIGITS				[0-9]
IDENTIFIER			{CHARACTERS}({CHARACTERS}|{DIGITS})*
SIGN				(\-|\+){0,1}
INTEGER				{DIGITS}+
REAL				{INTEGER}\.{INTEGER}
STRING				\".*\"
COMMENT				\/\/.*\n
TRASH				[ \t\n]+

%%

{INTEGER} {
	yylval.lval=atol(yytext);
	return LONG;
}

{REAL} {
	yylval.dval=atof(yytext);
	return DOUBLE;
}

"if"					return IF;
"else"					return ELSE;

"while"					return WHILE;
"for"					return FOR;
":"						return IN;

"++"					return INCREMENT;
"--"					return DECREMENT;

";"						return END;

[-()=+*/\%;{}.,!^\[\]]	return *yytext;
"&&"					return AND;
"||"					return OR;
">"						return GREATER;
"<"						return LOWER;
">="					return GEQ;
"<="					return LEQ;
"=="					return EQ;
"!="					return NEQ;

"add"					return ADD;
"pop"					return POP;

"read"					return READ;
"print"					return PRINT;
"nl"					return NL;

{IDENTIFIER} {
	yylval.sval=(char *)malloc(yyleng*sizeof(char));
	strcpy(yylval.sval,yytext);
	return VARIABLE;
}

{STRING} {
	yytext++;
	yyleng--;
	yytext[yyleng-1] = '\0';
	yylval.sval=(char*)malloc(yyleng*sizeof(char));
	strcpy(yylval.sval, yytext);
	return STR;
}

{COMMENT}			;
{TRASH}				;

.					printf("Carácter inválido: %s\n",yytext);

%%

