%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	Token token;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
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
//TODO: there might be non terminals here to

%token <world> WORLD
%token <tree> TREE
%token <forest> FOREST
%token <grow> GROW

%token <integer> INTEGER
%token <id> ID
%token <string> STRING

%token <token> IF
%token <token> ELSE
%token <for> FOR
%token <in> IN
%token <with> WITH

%token <token> DIV
%token <token> MUL
%token <token> ADD
%token <token> SUB
%token <token> ADD_EQ
%token <token> SUB_EQ
%token <token> MUL_EQ

%token <arrow> ARROW

%token <equal> EQUAL
%token <greaterequal> GREATEREQUAL
%token <lesserequal> LESSEREQUAL

%token <lesserthan> LESSERTHAN
%token <greaterthan> GREATERTHAN

%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_BRACE
%token <token> OPEN_BRACE
%token <token> CLOSE_CURLY_BRACE
%token <token> OPEN_CURLY_BRACE

%token <semicolon> SEMICOLON
%token <comma> COMMA

%token <token> UNKNOWN

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

//ADDED

world: OPEN_CURLY_BRACE worldExpression CLOSE_CURLY_BRACE			{$$ = ExpressionWorldSemanticAction($2);}
	;

id: ID																{ $$ = IdSemanticAction($1); }
	;



%%
