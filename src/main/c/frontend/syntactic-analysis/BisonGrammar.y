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
	
	AttributeValue * attributeValue;
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

%token <token> ARROW

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
%type <attributeValue> attributeValue


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

programExpression: mainExpressions																							{ $$ = WorldlessProgramExpressionSemanticAction($1, WORLDLESS);}
	|		       worldExpression mainExpressions																			{ $$ = WorldProgramExpressionSemanticAction($2, $1, WORLDFULL);}
	;

mainExpression: treeExpression																								{ $$ = MainExpressionTreeSemanticAction($1, TREE_m);} 
	|			forestExpression																							{ $$ = MainExpressionForestSemanticAction($1, FOREST_m);} 
	|			growExpression																								{ $$ = MainExpressionGrowSemanticAction($1, GROW_m);} 
	|			forExpression																								{ $$ = MainExpressionForSemanticAction($1, FOR_m);} 
	|			arithmeticAssignation																						{ $$ = MainExpressionArithmeticAssignationSemanticAction($1, ARITHMETIC_m);} 
	|			generalAssignation																							{ $$ = MainExpressionGeneralSemanticAction($1, GENERAL_ASSIGNATION_m);} 
	;

mainExpressions: mainExpression																								{ $$ = SimpleMainExpressionSemanticAction($1, SIMPLE_e);}
	|			 mainExpressions mainExpression																				{ $$ = MultipleMainExpressionSemanticAction($1, $2, MULTIPLE_e);}			 
	;

generalAssignation: ID ID EQUAL declarationValue SEMICOLON																	{ $$ = GeneralDeclarationAssignationSemanticAction($1, $2, $4, ID_BY_VALUE);}
	|				ID EQUAL declarationValue SEMICOLON																		{ $$ = GeneralSingleDeclarationAssignationSemanticAction($1, $3, ID_BY_VALUE);}
	|				ID ID EQUAL arithmeticOperation SEMICOLON																{ $$ = GeneralArithmeticOperationAssignationSemanticAction($1, $2, $4, ID_BY_OPP);}
	|				ID EQUAL arithmeticOperation SEMICOLON																	{ $$ = GeneralSingleArithmeticOperationAssignationSemanticAction($1, $3, ID_BY_OPP);}
	|				attributeValue EQUAL declarationValue SEMICOLON															{ $$ = GeneralAttributeDeclarationAssignationSemanticAction($1, $3, ATT_BY_VALUE);}
	|				attributeValue EQUAL arithmeticOperation SEMICOLON														{ $$ = GeneralAttributeArithmeticOperationAssignationSemanticAction($1, $3, ATT_BY_OPP);}
	;

arithmeticAssignation: ID ADD_EQ declarationValue SEMICOLON																	{ $$ = ArithmeticDeclarationAssignationSemanticAction($1, ADD_o, $3, ID_BY_VALUE);}
	|				   ID SUB_EQ declarationValue SEMICOLON																	{ $$ = ArithmeticDeclarationAssignationSemanticAction($1, SUB_o, $3, ID_BY_VALUE);}
	|				   ID MUL_EQ declarationValue SEMICOLON																	{ $$ = ArithmeticDeclarationAssignationSemanticAction($1, MUL_o, $3, ID_BY_VALUE);}
	|				   ID DIV_EQ declarationValue SEMICOLON																	{ $$ = ArithmeticDeclarationAssignationSemanticAction($1, DIV_o, $3, ID_BY_VALUE);}
	|				   ID ADD_EQ arithmeticOperation SEMICOLON																{ $$ = ArithmeticOperationAssignationSemanticAction($1, ADD_o, $3, ID_BY_OPP);}
	|				   ID SUB_EQ arithmeticOperation SEMICOLON																{ $$ = ArithmeticOperationAssignationSemanticAction($1, SUB_o, $3, ID_BY_OPP);}
	|				   ID MUL_EQ arithmeticOperation SEMICOLON																{ $$ = ArithmeticOperationAssignationSemanticAction($1, MUL_o, $3, ID_BY_OPP);}
	|				   ID DIV_EQ arithmeticOperation SEMICOLON																{ $$ = ArithmeticOperationAssignationSemanticAction($1, DIV_o, $3, ID_BY_OPP);}
	|				   attributeValue ADD_EQ declarationValue SEMICOLON														{ $$ = ArithmeticAttributeDeclarationAssignationSemanticAction($1, ADD_o, $3, ATT_BY_VALUE);;}
	|				   attributeValue SUB_EQ declarationValue SEMICOLON														{ $$ = ArithmeticAttributeDeclarationAssignationSemanticAction($1, SUB_o, $3, ATT_BY_VALUE);;}
	|				   attributeValue MUL_EQ declarationValue SEMICOLON														{ $$ = ArithmeticAttributeDeclarationAssignationSemanticAction($1, MUL_o, $3, ATT_BY_VALUE);;}
	|				   attributeValue DIV_EQ declarationValue SEMICOLON														{ $$ = ArithmeticAttributeDeclarationAssignationSemanticAction($1, DIV_o, $3, ATT_BY_VALUE);;}
	|				   attributeValue ADD_EQ arithmeticOperation SEMICOLON													{ $$ = ArithmeticAttributeOperationAssignationSemanticAction($1, ADD_o, $3, ATT_BY_OPP);}
	|				   attributeValue SUB_EQ arithmeticOperation SEMICOLON													{ $$ = ArithmeticAttributeOperationAssignationSemanticAction($1, SUB_o, $3, ATT_BY_OPP);}
	|				   attributeValue MUL_EQ arithmeticOperation SEMICOLON													{ $$ = ArithmeticAttributeOperationAssignationSemanticAction($1, MUL_o, $3, ATT_BY_OPP);}
	|				   attributeValue DIV_EQ arithmeticOperation SEMICOLON													{ $$ = ArithmeticAttributeOperationAssignationSemanticAction($1, DIV_o, $3, ATT_BY_OPP);}
	;

forExpression: FOR ID IN OPEN_BRACE INTEGER COMMA INTEGER CLOSE_BRACE OPEN_CURLY_BRACE mainExpressions CLOSE_CURLY_BRACE		{ $$ = ForExpressionSemanticAction($2, $5, $7, $10, CLASSIC_ITERATION);}
	|		   FOR ID IN ID OPEN_CURLY_BRACE mainExpressions CLOSE_CURLY_BRACE												{ $$ = ForNoRangeExpressionSemanticAction($2, $4, $6, FOREST_ITERATION);}
	;

growExpression: GROW OPEN_PARENTHESIS ID CLOSE_PARENTHESIS SEMICOLON														{ $$ = GrowExpressionSemanticAction($3);}
	;

treeExpression: TREE ID WITH OPEN_PARENTHESIS treeAssignments CLOSE_PARENTHESIS SEMICOLON									{ $$ = TreeExpressionSemanticAction($2, $5, FULL_t);}
	|			TREE ID SEMICOLON																							{ $$ = DefaultTreeExpressionSemanticAction($2, EMPTY_t);}
	;

treeAssignment: ID EQUAL declarationValue																					{ $$ = DeclarationTreeAssignmentSemanticAction($1, $3, ID_BY_VALUE);}
	|			ID EQUAL arithmeticOperation																				{ $$ = ArithmeticTreeAssignmentSemanticAction($1, $3, ID_BY_OPP);}
	;

treeAssignments: treeAssignment																								{ $$ = SimpleTreeAssignmentsSemanticAction($1, SIMPLE_ta);}
    |            treeAssignments COMMA treeAssignment																		{ $$ = MultipleTreeAssignmentsSemanticAction($1, $3, MULTIPLE_ta);}
	;

forestExpression: FOREST ID WITH OPEN_PARENTHESIS forestAssignments CLOSE_PARENTHESIS SEMICOLON								{ $$ = ForestExpressionSemanticAction($2, $5, FULL_f);}
	|			  FOREST ID SEMICOLON																						{ $$ = DefaultForestExpressionSemanticAction($2, EMPTY_f);}
	;

forestAssignment: ID EQUAL declarationValue																					{ $$ = DeclarationForestAssignmentSemanticAction($1, $3, ID_BY_VALUE);}
	|			  ID EQUAL arithmeticOperation																				{ $$ = ArithmeticForestAssignmentSemanticAction($1, $3, ID_BY_OPP);}
	;

forestAssignments: forestAssignment																							{ $$ = SimpleForestAssignmentsSemanticAction(ForestAssignment * forestAssignment, ForestAssignType type);}
    |              forestAssignments COMMA forestAssignment																	{ $$ = MultipleForestAssignmentsSemanticAction(ForestAssignments * forestAssignments, ForestAssignment * forestAssignment, ForestAssignType type);}
	;

worldExpression: WORLD OPEN_CURLY_BRACE worldAssignments CLOSE_CURLY_BRACE													{$$ = WorldExpressionSemanticAction(WorldAssignments* worldAssignments);}
	;

worldAssignment: ID EQUAL declarationValue																					{ $$ = WorldAssignmentDeclarationSemanticAction(Id id, DeclarationValue* value, AssignationType type);}
	|			 ID EQUAL arithmeticOperation																				{ $$ = WorldAssignmentArithmeticSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type);}
	;

worldAssignments: worldAssignment																							{ $$ = SimpleWorldAssignmentsSemanticAction(WorldAssignment * worldAssignment, WorldType wType);}
    |             worldAssignments COMMA worldAssignment																	{ $$ = MultipleWorldAssignmentsSemanticAction(WorldAssignments * worldAssignments, WorldAssignment * worldAssignment, WorldType wType);}
	;

arithmeticOperation: declarationValue ADD declarationValue																	{ $$ = ArithmeticOperationAllDeclarationValueSemanticAction(OperatorType operator, DeclarationValue* left, DeclarationValue* right, ArithmeticOperationType type);}
	|				 declarationValue SUB declarationValue																	{ $$ = ArithmeticOperationAllDeclarationValueSemanticAction(OperatorType operator, DeclarationValue* left, DeclarationValue* right, ArithmeticOperationType type);}
	|				 declarationValue MUL declarationValue																	{ $$ = ArithmeticOperationAllDeclarationValueSemanticAction(OperatorType operator, DeclarationValue* left, DeclarationValue* right, ArithmeticOperationType type);}
	|				 declarationValue DIV declarationValue																	{ $$ = ArithmeticOperationAllDeclarationValueSemanticAction(OperatorType operator, DeclarationValue* left, DeclarationValue* right, ArithmeticOperationType type);}
	|				 arithmeticOperation ADD declarationValue																{ $$ = ArithmeticOperationLeftDeclarationRightSemanticAction(OperatorType operator, DeclarationValue* left, ArithmeticOperation* right, ArithmeticOperationType type);}
	|				 arithmeticOperation SUB declarationValue																{ $$ = ArithmeticOperationLeftDeclarationRightSemanticAction(OperatorType operator, DeclarationValue* left, ArithmeticOperation* right, ArithmeticOperationType type);}
	|				 arithmeticOperation MUL declarationValue																{ $$ = ArithmeticOperationLeftDeclarationRightSemanticAction(OperatorType operator, DeclarationValue* left, ArithmeticOperation* right, ArithmeticOperationType type);}
	|				 arithmeticOperation DIV declarationValue																{ $$ = ArithmeticOperationLeftDeclarationRightSemanticAction(OperatorType operator, DeclarationValue* left, ArithmeticOperation* right, ArithmeticOperationType type);}
	|				 declarationValue ADD arithmeticOperation																{ $$ = ArithmeticOperationRightDeclarationLeftSemanticAction(OperatorType operator, ArithmeticOperation* left, DeclarationValue* right, ArithmeticOperationType type);}
	|				 declarationValue SUB arithmeticOperation																{ $$ = ArithmeticOperationRightDeclarationLeftSemanticAction(OperatorType operator, ArithmeticOperation* left, DeclarationValue* right, ArithmeticOperationType type);}
	|				 declarationValue MUL arithmeticOperation																{ $$ = ArithmeticOperationRightDeclarationLeftSemanticAction(OperatorType operator, ArithmeticOperation* left, DeclarationValue* right, ArithmeticOperationType type);}
	|				 declarationValue DIV arithmeticOperation																{ $$ = ArithmeticOperationRightDeclarationLeftSemanticAction(OperatorType operator, ArithmeticOperation* left, DeclarationValue* right, ArithmeticOperationType type);}
	|				 arithmeticOperation ADD arithmeticOperation															{ $$ = AArithmeticOperationAllSemanticAction(OperatorType operator, ArithmeticOperation* left, ArithmeticOperation* right, ArithmeticOperationType type);}
	|				 arithmeticOperation SUB arithmeticOperation															{ $$ = ArithmeticOperationAllSemanticAction(OperatorType operator, ArithmeticOperation* left, ArithmeticOperation* right, ArithmeticOperationType type);}
	|				 arithmeticOperation MUL arithmeticOperation															{ $$ = ArithmeticOperationAllSemanticAction(OperatorType operator, ArithmeticOperation* left, ArithmeticOperation* right, ArithmeticOperationType type);}
	|				 arithmeticOperation DIV arithmeticOperation															{ $$ = ArithmeticOperationAllSemanticAction(OperatorType operator, ArithmeticOperation* left, ArithmeticOperation* right, ArithmeticOperationType type);}
	|				 OPEN_PARENTHESIS arithmeticOperation CLOSE_PARENTHESIS													{ $$ = ArithmeticOperationInceptionSemanticAction(NONE, ArithmeticOperation* operation, PARENTHESIS);}
	;

attributeValue:   ID ARROW ID																								{ $$ = AttributeValueSemanticAction(Id varId, Id attId, AttributeValueType type);}
	;

declarationValue: ID																										{ $$ = DeclarationValueIDSemanticAction(Id id, DeclarationValueType type);}
	|			  STRING																									{ $$ = DeclarationValueStringSemanticAction(char* string, DeclarationValueType type);}
	|			  TRUE																										{ $$ = DeclarationValueBooleanSemanticAction(boolean value, DeclarationValueType type);}
	|			  FALSE																										{ $$ = DeclarationValueBooleanSemanticAction(boolean value, DeclarationValueType type);}
	|			  HEXCOLOR																									{ $$ = DeclarationValueHexSemanticAction(Hexcolor color, DeclarationValueType type);}
	|			  INTEGER																									{ $$ = DeclarationValueIntSemanticAction(int integer, DeclarationValueType type);}
	|			  attributeValue																							{$$ = DeclarationValueAttributeSemanticAction(AttributeValue * attValue, DeclarationValueType type);}
	|			  OPEN_PARENTHESIS declarationValue CLOSE_PARENTHESIS														{ $$ = DeclarationValueInceptionSemanticAction(DeclarationValue * declarationValue, DeclarationValueType type);}
	;

%%
