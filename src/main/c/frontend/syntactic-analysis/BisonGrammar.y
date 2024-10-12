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

	/** Non-terminals. */

	MainExpression * mainExpression;
	WorldExpression * worldExpression;
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

%token <token> SEMICOLON

%token <token> WORLD

%token <token> UNKNOWN

/** Non-terminals. */
%type <program> program
%type <programExpression> programExpression
%type <worldExpression> worldExpression
%type <mainExpression> mainExpression


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

programExpression: mainExpression											{ $$ = NULL;}
	|		worldExpression mainExpression							{ $$ = NULL;}
	;

mainExpression: ID SEMICOLON										{ $$ = NULL;}
	;

worldExpression: WORLD SEMICOLON									{ $$ = NULL;}
	;

%%
