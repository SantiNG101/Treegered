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
%token <hexcolor> HEXCOLOR
%token <token> TRUE
%token <token> FALSE
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

expression: mainExpression
	|		worldExpression mainExpression
	;

mainExpression: treeExpression mainExpression
	| 			forestExpression mainExpression
	|			assignment mainExpression
	|			forExpression mainExpression
	|			arithmeticAssignationExpression mainExpression
	|			conditionalExpression mainExpression
	|			growExpression mainExpression
	|			treeExpression
	| 			forestExpression
	|			assignment
	|			forExpression
	|			arithmeticAssignationExpression
	|			conditionalExpression
	|			growExpression
	;

worldExpression: WORLD OPEN_CURLY_BRACE worldExpressions CLOSE_CURLY_BRACE			{$$ = ExpressionWorldSemanticAction($2);}
	;

worldExpressions: assignmentExpression
	|			  worldExpressions COMMA worldExpressions
	;

treeExpression: TREE ID WITH OPEN_PARENTHESIS treeExpressions CLOSE_PARENTHESIS SEMICOLON
	;

treeExpressions: assignmentExpression
	|			 treeExpressions COMMA treeExpressions
	;

forestExpression: FOREST ID WITH OPEN_PARENTHESIS forestExpressions CLOSE_PARENTHESIS SEMICOLON
	;

forestExpressions: assignmentExpression
	|			 forestExpressions COMMA forestExpressions
	;

growExpression: GROW OPEN_PARENTHESIS ID CLOSE_PARENTHESIS SEMICOLON
	;

assignment: assignmentExpression SEMICOLON
	;

assignmentExpression: ID EQUAL declarationValue
	|				  ID EQUAL ID arithmeticExpression
	;

arithmeticAssignationExpression: arithmeticAssignationExpressions SEMICOLON
	;

arithmeticAssignationExpressions: ID ADD_EQ declarationValue
	|				  			 ID SUB_EQ declarationValue
	;

id: ID																{ $$ = IdSemanticAction($1); }
	;

declarationValue: INTEGER									{$$ = declarationValueSemanticAction($1);}
	|			  TRUE										{$$ = declarationValueSemanticAction($1);}
	|			  FALSE										{$$ = declarationValueSemanticAction($1);}
	|			  STRING									{$$ = declarationValueSemanticAction($1);}
	|			  HEXCOLOR									{$$ = declarationValueSemanticAction($1);}
	;



%%
