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
	boolean bool;

	/** Non-terminals. */
	
	GeneralAssignation * generalAssignation;
	ArithmeticOperation * arithmeticOperation;
	ArithmeticAssignation * arithmeticAssignation;
	ForExpression * forExpression;
	GrowExpression * growExpression;
	ForestAssignment * forestAssignment;
	ForestAssignments * forestAssignments;
	ForestExpression * forestExpression;
	TreeAssignment * treeAssignment;
	TreeAssignments * treeAssignments;
	TreeExpression * treeExpression;
	MainExpression * mainExpression;
	MainExpressions * mainExpressions;
	WorldExpression * worldExpression;
	WorldAssignment * worldAssignment;
	WorldAssignments * worldAssignments;
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
%token <bool> TRUE
%token <bool> FALSE
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
%type <worldAssignments> worldAssignments
%type <declarationValue> declarationValue
%type <mainExpression> mainExpression
%type <mainExpressions> mainExpressions
%type <treeExpression> treeExpression
%type <treeAssignment> treeAssignment
%type <treeAssignments> treeAssignments
%type <forestExpression> forestExpression
%type <forestAssignment> forestAssignment
%type <forestAssignments> forestAssignments
%type <growExpression> growExpression
%type <forExpression> forExpression
%type <arithmeticAssignation> arithmeticAssignation
%type <arithmeticOperation> arithmeticOperation
%type <generalAssignation> generalAssignation


/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: programExpression																									{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

programExpression: mainExpressions																							{ $$ = NULL;}
	|		       worldExpression mainExpressions																			{ $$ = NULL;}
	;

mainExpression: treeExpression																								{ $$ = NULL;} 
	|			forestExpression																							{ $$ = NULL;} 
	|			growExpression																								{ $$ = NULL;} 
	|			forExpression																								{ $$ = NULL;} 
	|			arithmeticAssignation																						{ $$ = NULL;} 
	|			generalAssignation																							{ $$ = NULL;} 
	;

mainExpressions: mainExpression																								{ $$ = NULL;}
	|			 mainExpressions mainExpression																				{ $$ = NULL;}			 
	;

generalAssignation: ID ID EQUAL declarationValue SEMICOLON																	{ $$ = NULL;}
	|				ID EQUAL declarationValue SEMICOLON																		{ $$ = NULL;}
	|				ID ID EQUAL arithmeticOperation SEMICOLON																{ $$ = NULL;}
	|				ID EQUAL arithmeticOperation SEMICOLON																	{ $$ = NULL;}
	;

arithmeticAssignation: ID ADD_EQ declarationValue SEMICOLON																	{ $$ = NULL;}
	|				   ID SUB_EQ declarationValue SEMICOLON																	{ $$ = NULL;}
	|				   ID MUL_EQ declarationValue SEMICOLON																	{ $$ = NULL;}
	|				   ID DIV_EQ declarationValue SEMICOLON																	{ $$ = NULL;}
	|				   ID ADD_EQ arithmeticOperation SEMICOLON																{ $$ = NULL;}
	|				   ID SUB_EQ arithmeticOperation SEMICOLON																{ $$ = NULL;}
	|				   ID MUL_EQ arithmeticOperation SEMICOLON																{ $$ = NULL;}
	|				   ID DIV_EQ arithmeticOperation SEMICOLON																{ $$ = NULL;}
	;

forExpression: FOR ID IN OPEN_BRACE INTEGER COMMA INTEGER CLOSE_BRACE OPEN_CURLY_BRACE mainExpressions CLOSE_CURLY_BRACE	{ $$ = NULL;}
	;

growExpression: GROW OPEN_PARENTHESIS ID CLOSE_PARENTHESIS SEMICOLON														{ $$ = NULL;}
	;

treeExpression: TREE ID WITH OPEN_PARENTHESIS treeAssignments CLOSE_PARENTHESIS SEMICOLON									{ $$ = NULL;}
	|			TREE ID SEMICOLON																							{ $$ = NULL;}
	;

treeAssignment: ID EQUAL declarationValue																					{ $$ = NULL;}
	|			ID EQUAL arithmeticOperation																				{ $$ = NULL;}
	;

treeAssignments: treeAssignment																								{ $$ = NULL;}
    |            treeAssignments COMMA treeAssignment																		{ $$ = NULL;}
	;

forestExpression: FOREST ID WITH OPEN_PARENTHESIS forestAssignments CLOSE_PARENTHESIS SEMICOLON								{ $$ = NULL;}
	|			  FOREST ID SEMICOLON																						{ $$ = NULL;}
	;

forestAssignment: ID EQUAL declarationValue																					{ $$ = NULL;}
	|			  ID EQUAL arithmeticOperation																				{ $$ = NULL;}
	;

forestAssignments: forestAssignment																							{ $$ = NULL;}
    |              forestAssignments COMMA forestAssignment																	{ $$ = NULL;}
	;

worldExpression: WORLD OPEN_CURLY_BRACE worldAssignments CLOSE_CURLY_BRACE													{$$ = NULL;}
	;

worldAssignment: ID EQUAL declarationValue																					{ $$ = NULL;}
	|			 ID EQUAL arithmeticOperation																				{ $$ = NULL;}
	;

worldAssignments: worldAssignment																							{ $$ = NULL;}
    |             worldAssignments COMMA worldAssignment																	{ $$ = NULL;}
	;

arithmeticOperation: declarationValue ADD declarationValue																	{ $$ = NULL;}
	|				 declarationValue SUB declarationValue																	{ $$ = NULL;}
	|				 declarationValue MUL declarationValue																	{ $$ = NULL;}
	|				 declarationValue DIV declarationValue																	{ $$ = NULL;}
	|				 arithmeticOperation ADD declarationValue																{ $$ = NULL;}
	|				 arithmeticOperation SUB declarationValue																{ $$ = NULL;}
	|				 arithmeticOperation MUL declarationValue																{ $$ = NULL;}
	|				 arithmeticOperation DIV declarationValue																{ $$ = NULL;}
	|				 declarationValue ADD arithmeticOperation																{ $$ = NULL;}
	|				 declarationValue SUB arithmeticOperation																{ $$ = NULL;}
	|				 declarationValue MUL arithmeticOperation																{ $$ = NULL;}
	|				 declarationValue DIV arithmeticOperation																{ $$ = NULL;}
	|				 arithmeticOperation ADD arithmeticOperation															{ $$ = NULL;}
	|				 arithmeticOperation SUB arithmeticOperation															{ $$ = NULL;}
	|				 arithmeticOperation MUL arithmeticOperation															{ $$ = NULL;}
	|				 arithmeticOperation DIV arithmeticOperation															{ $$ = NULL;}
	|				 OPEN_PARENTHESIS arithmeticOperation CLOSE_PARENTHESIS													{ $$ = NULL;}
	;

declarationValue: ID																										{ $$ = NULL;}
	|			  STRING																									{ $$ = NULL;}
	|			  TRUE																										{ $$ = NULL;}
	|			  FALSE																										{ $$ = NULL;}
	|			  HEXCOLOR																									{ $$ = NULL;}
	|			  INTEGER																									{ $$ = NULL;}
	|			  OPEN_PARENTHESIS declarationValue CLOSE_PARENTHESIS														{ $$ = NULL;}
	;

%%
