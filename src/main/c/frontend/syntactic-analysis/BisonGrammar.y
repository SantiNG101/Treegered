%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	Token token;
	char* string;
	Id id;
	Hexcolor hexcolor;

	/** Non-terminals. */

	ArithmeticAssignation * arithmeticAssignation;
	ForExpression * forExpression;
	GrowExpression * growExpression;
	ForestAssignment * forestAssignment;
	ForestExpression * forestExpression;
	TreeAssignment * treeAssignment;
	TreeExpression * treeExpression;
	MainExpression * mainExpression;
	WorldExpression * worldExpression;
	WorldAssignment * worldAssignment;
	DeclarationValue * declarationValue;
	ProgramExpression * programExpression;
	Program * program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <integer> INTEGER
%token <id> ID
%token <string> STRING
%token <token> TRUE
%token <token> FALSE
%token <hexcolor> HEXCOLOR

%token <token> EQUAL

%token <token> COMMA
%token <token> SEMICOLON

%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV
%token <token> ADD_EQ
%token <token> SUB_EQ
%token <token> MUL_EQ
%token <token> DIV_EQ

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_CURLY_BRACE
%token <token> CLOSE_CURLY_BRACE
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE

%token <token> FOR
%token <token> IN
%token <token> WITH
%token <token> GROW

%token <token> WORLD
%token <token> TREE
%token <token> FOREST

%token <token> UNKNOWN

/** Non-terminals. */
%type <program> program
%type <programExpression> programExpression
%type <worldExpression> worldExpression
%type <worldAssignment> worldAssignment
%type <declarationValue> declarationValue
%type <mainExpression> mainExpression
%type <treeExpression> treeExpression
%type <treeAssignment> treeAssignment
%type <forestExpression> forestExpression
%type <forestAssignment> forestAssignment
%type <growExpression> growExpression
%type <forExpression> forExpression
%type <arithmeticAssignation> arithmeticAssignation


/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: programExpression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

programExpression: mainExpression									{ $$ = NULL;}
	|		worldExpression mainExpression							{ $$ = NULL;}
	;

mainExpression: ID SEMICOLON										{ $$ = NULL;}
	|			treeExpression										{ $$ = NULL;} 
	|			mainExpression treeExpression 						{ $$ = NULL;}
	|			forestExpression									{ $$ = NULL;} 
	|			mainExpression forestExpression 					{ $$ = NULL;}
	|			growExpression										{ $$ = NULL;} 
	|			mainExpression growExpression 						{ $$ = NULL;}
	|			forExpression										{ $$ = NULL;} 
	|			mainExpression forExpression 						{ $$ = NULL;}
	|			arithmeticAssignation										{ $$ = NULL;} 
	|			mainExpression arithmeticAssignation 						{ $$ = NULL;}
	;

arithmeticAssignation: ID ADD_EQ declarationValue SEMICOLON		{ $$ = NULL;}
	|				   ID SUB_EQ declarationValue SEMICOLON		{ $$ = NULL;}
	|				   ID MUL_EQ declarationValue SEMICOLON		{ $$ = NULL;}
	|				   ID DIV_EQ declarationValue SEMICOLON		{ $$ = NULL;}
	;

forExpression: FOR ID IN OPEN_BRACE INTEGER COMMA INTEGER CLOSE_BRACE OPEN_CURLY_BRACE mainExpression CLOSE_CURLY_BRACE	{$$=NULL;}
	;

growExpression: GROW OPEN_PARENTHESIS ID CLOSE_PARENTHESIS SEMICOLON	{ $$ = NULL;}
	;

treeExpression: TREE ID WITH OPEN_PARENTHESIS treeAssignment CLOSE_PARENTHESIS SEMICOLON	{ $$ = NULL;}
	|			TREE ID SEMICOLON									{ $$ = NULL;}
	;

treeAssignment: ID EQUAL declarationValue							{ $$ = NULL;}
	|			treeAssignment COMMA ID EQUAL declarationValue		{ $$ = NULL;}
	;

forestExpression: FOREST ID WITH OPEN_PARENTHESIS forestAssignment CLOSE_PARENTHESIS SEMICOLON	{ $$ = NULL;}
	|			  FOREST ID SEMICOLON									{ $$ = NULL;}
	;

forestAssignment: ID EQUAL declarationValue							{ $$ = NULL;}
	|			  forestAssignment COMMA ID EQUAL declarationValue		{ $$ = NULL;}
	;

worldExpression: WORLD OPEN_CURLY_BRACE worldAssignment CLOSE_CURLY_BRACE	{$$ = NULL;}
	;

worldAssignment: ID EQUAL declarationValue									{ $$ = NULL;}
	|			 worldAssignment COMMA ID EQUAL declarationValue			{$$ = NULL;}
	;

declarationValue: ID		{ $$ = NULL;}
	|			  STRING	{ $$ = NULL;}
	|			  TRUE		{ $$ = NULL;}
	|			  FALSE		{ $$ = NULL;}
	|			  HEXCOLOR	{ $$ = NULL;}
	|			  INTEGER	{ $$ = NULL;}
	;

%%
