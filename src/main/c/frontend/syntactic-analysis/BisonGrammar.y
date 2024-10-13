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
	
	ConditionalClause * conditionalClause;
	ConditionalExpression * conditionalExpression;
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

%token <token> EQUIVALENT
%token <token> DIFFERENT
%token <token> LESSERTHAN
%token <token> GREATERTHAN
%token <token> LESSER_EQUAL
%token <token> GREATER_EQUAL

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

%token <token> IF
%token <token> ELSE

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
%type <conditionalExpression> conditionalExpression
%type <conditionalClause> conditionalClause

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV
%left EQUIVALENT 
%left DIFFERENT 
%left LESSER_EQUAL 
%left GREATER_EQUAL 
%left LESSERTHAN
%left GREATERTHAN

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
	|			conditionalExpression																						{ $$ = MainExpressionConditionalSemanticAction($1, CONDITIONAL_m);}
	;

mainExpressions: mainExpression																								{ $$ = SimpleMainExpressionSemanticAction($1, SIMPLE_e);}
	|			 mainExpressions mainExpression																				{ $$ = MultipleMainExpressionSemanticAction($1, $2, MULTIPLE_e);}			 
	;

conditionalExpression: IF OPEN_PARENTHESIS conditionalClause CLOSE_PARENTHESIS 
													OPEN_CURLY_BRACE mainExpressions CLOSE_CURLY_BRACE						{ $$ = IfOnlyConditionalExpression($3, $6, IF_c);}
	|				   IF OPEN_PARENTHESIS conditionalClause CLOSE_PARENTHESIS 
													OPEN_CURLY_BRACE mainExpressions CLOSE_CURLY_BRACE ELSE 
															OPEN_CURLY_BRACE mainExpressions CLOSE_CURLY_BRACE				{ $$ = IfElseConditionalExpression($3, $6, $10, ELSE_c);}
	;

conditionalClause:  declarationValue EQUIVALENT declarationValue															{ $$ = AllDeclarationConditionalClauseSemanticAction($1, $3, EQUIVALENT_c, V_V);}
	|				declarationValue EQUIVALENT conditionalClause															{ $$ = LeftDeclarationRightConditionalConditionalClauseSemanticAction($1, $3, EQUIVALENT_c, V_C);}
	|				conditionalClause EQUIVALENT declarationValue															{ $$ = LeftConditionalRightDeclarationConditionalClauseSemanticAction($1, $3, EQUIVALENT_c, C_V);}
	|				conditionalClause EQUIVALENT conditionalClause															{ $$ = AllConditionalConditionalClauseSemanticAction($1, $3, EQUIVALENT_c, C_C);}
	|				declarationValue DIFFERENT declarationValue																{ $$ = AllDeclarationConditionalClauseSemanticAction($1, $3, DIFFERENT_c, V_V);}
	|				declarationValue DIFFERENT conditionalClause															{ $$ = LeftDeclarationRightConditionalConditionalClauseSemanticAction($1, $3, DIFFERENT_c, V_C);}
	|				conditionalClause DIFFERENT declarationValue															{ $$ = LeftConditionalRightDeclarationConditionalClauseSemanticAction($1, $3, DIFFERENT_c, C_V);}
	|				conditionalClause DIFFERENT conditionalClause															{ $$ = AllConditionalConditionalClauseSemanticAction($1, $3, DIFFERENT_c, C_C);}
	|				declarationValue LESSER_EQUAL declarationValue															{ $$ = AllDeclarationConditionalClauseSemanticAction($1, $3, LESSER_EQUAL_c, V_V);}
	|				declarationValue LESSER_EQUAL conditionalClause															{ $$ = LeftDeclarationRightConditionalConditionalClauseSemanticAction($1, $3, LESSER_EQUAL_c, V_C);}
	|				conditionalClause LESSER_EQUAL declarationValue															{ $$ = LeftConditionalRightDeclarationConditionalClauseSemanticAction($1, $3, LESSER_EQUAL_c, C_V);}
	|				conditionalClause LESSER_EQUAL conditionalClause														{ $$ = AllConditionalConditionalClauseSemanticAction($1, $3, LESSER_EQUAL_c, C_C);}
	|				declarationValue GREATER_EQUAL declarationValue															{ $$ = AllDeclarationConditionalClauseSemanticAction($1, $3, GREATER_EQUAL_c, V_V);}
	|				declarationValue GREATER_EQUAL conditionalClause														{ $$ = LeftDeclarationRightConditionalConditionalClauseSemanticAction($1, $3, GREATER_EQUAL_c, V_C);}
	|				conditionalClause GREATER_EQUAL declarationValue														{ $$ = LeftConditionalRightDeclarationConditionalClauseSemanticAction($1, $3, GREATER_EQUAL_c, C_V);}
	|				conditionalClause GREATER_EQUAL conditionalClause														{ $$ = AllConditionalConditionalClauseSemanticAction($1, $3, GREATER_EQUAL_c, C_C);}
	|				declarationValue LESSERTHAN declarationValue															{ $$ = AllDeclarationConditionalClauseSemanticAction($1, $3, LESSERTHAN_c, V_V);}
	|				declarationValue LESSERTHAN conditionalClause															{ $$ = LeftDeclarationRightConditionalConditionalClauseSemanticAction($1, $3, LESSERTHAN_c, V_C);}
	|				conditionalClause LESSERTHAN declarationValue															{ $$ = LeftConditionalRightDeclarationConditionalClauseSemanticAction($1, $3, LESSERTHAN_c, C_V);}
	|				conditionalClause LESSERTHAN conditionalClause															{ $$ = AllConditionalConditionalClauseSemanticAction($1, $3, LESSERTHAN_c, C_C);}
	|				declarationValue GREATERTHAN declarationValue															{ $$ = AllDeclarationConditionalClauseSemanticAction($1, $3, GREATERTHAN_c, V_V);}
	|				declarationValue GREATERTHAN conditionalClause															{ $$ = LeftDeclarationRightConditionalConditionalClauseSemanticAction($1, $3, GREATERTHAN_c, V_C);}
	|				conditionalClause GREATERTHAN declarationValue															{ $$ = LeftConditionalRightDeclarationConditionalClauseSemanticAction($1, $3, GREATERTHAN_c, C_V);}
	|				conditionalClause GREATERTHAN conditionalClause															{ $$ = AllConditionalConditionalClauseSemanticAction($1, $3, GREATERTHAN_c, C_C);}
	|				OPEN_PARENTHESIS conditionalClause CLOSE_PARENTHESIS													{ $$ = ConditionalInceptionConditionalClauseSemanticAction($2, NONE_c, PARENTHESIS_c);}
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

forExpression: FOR ID IN OPEN_BRACE INTEGER COMMA INTEGER CLOSE_BRACE OPEN_CURLY_BRACE mainExpressions CLOSE_CURLY_BRACE	{ $$ = ForExpressionSemanticAction($2, $5, $7, $10, CLASSIC_ITERATION);}
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

forestAssignments: forestAssignment																							{ $$ = SimpleForestAssignmentsSemanticAction($1, SIMPLE_fa);}
    |              forestAssignments COMMA forestAssignment																	{ $$ = MultipleForestAssignmentsSemanticAction($1, $3, MULTIPLE_fa);}
	;

worldExpression: WORLD OPEN_CURLY_BRACE worldAssignments CLOSE_CURLY_BRACE													{ $$ = WorldExpressionSemanticAction($3);}
	;

worldAssignment: ID EQUAL declarationValue																					{ $$ = WorldAssignmentDeclarationSemanticAction($1, $3, ID_BY_VALUE);}
	|			 ID EQUAL arithmeticOperation																				{ $$ = WorldAssignmentArithmeticSemanticAction($1, $3, ID_BY_OPP);}
	;

worldAssignments: worldAssignment																							{ $$ = SimpleWorldAssignmentsSemanticAction($1, SIMPLE_w);}
    |             worldAssignments COMMA worldAssignment																	{ $$ = MultipleWorldAssignmentsSemanticAction($1, $3, MULTIPLE_w);}
	;

arithmeticOperation: declarationValue ADD declarationValue																	{ $$ = ArithmeticOperationAllDeclarationValueSemanticAction(ADD_o, $1, $3, LV_RV);}
	|				 declarationValue SUB declarationValue																	{ $$ = ArithmeticOperationAllDeclarationValueSemanticAction(SUB_o, $1, $3, LV_RV);}
	|				 declarationValue MUL declarationValue																	{ $$ = ArithmeticOperationAllDeclarationValueSemanticAction(MUL_o, $1, $3, LV_RV);}
	|				 declarationValue DIV declarationValue																	{ $$ = ArithmeticOperationAllDeclarationValueSemanticAction(DIV_o, $1, $3, LV_RV);}
	|				 arithmeticOperation ADD declarationValue																{ $$ = ArithmeticOperationLeftDeclarationRightSemanticAction(ADD_o, $1, $3, LO_RV);}
	|				 arithmeticOperation SUB declarationValue																{ $$ = ArithmeticOperationLeftDeclarationRightSemanticAction(SUB_o, $1, $3, LO_RV);}
	|				 arithmeticOperation MUL declarationValue																{ $$ = ArithmeticOperationLeftDeclarationRightSemanticAction(DIV_o, $1, $3, LO_RV);}
	|				 arithmeticOperation DIV declarationValue																{ $$ = ArithmeticOperationLeftDeclarationRightSemanticAction(MUL_o, $1, $3, LO_RV);}
	|				 declarationValue ADD arithmeticOperation																{ $$ = ArithmeticOperationRightDeclarationLeftSemanticAction(ADD_o, $1, $3, LV_RO);}
	|				 declarationValue SUB arithmeticOperation																{ $$ = ArithmeticOperationRightDeclarationLeftSemanticAction(SUB_o, $1, $3, LV_RO);}
	|				 declarationValue MUL arithmeticOperation																{ $$ = ArithmeticOperationRightDeclarationLeftSemanticAction(MUL_o, $1, $3, LV_RO);}
	|				 declarationValue DIV arithmeticOperation																{ $$ = ArithmeticOperationRightDeclarationLeftSemanticAction(DIV_o, $1, $3, LV_RO);}
	|				 arithmeticOperation ADD arithmeticOperation															{ $$ = ArithmeticOperationAllSemanticAction(ADD_o, $1, $3, LO_RO);}
	|				 arithmeticOperation SUB arithmeticOperation															{ $$ = ArithmeticOperationAllSemanticAction(SUB_o, $1, $3, LO_RO);}
	|				 arithmeticOperation MUL arithmeticOperation															{ $$ = ArithmeticOperationAllSemanticAction(MUL_o, $1, $3, LO_RO);}
	|				 arithmeticOperation DIV arithmeticOperation															{ $$ = ArithmeticOperationAllSemanticAction(DIV_o, $1, $3, LO_RO);}
	|				 OPEN_PARENTHESIS arithmeticOperation CLOSE_PARENTHESIS													{ $$ = ArithmeticOperationInceptionSemanticAction(NONE, $2, PARENTHESIS);}
	;

attributeValue:   ID ARROW ID																								{ $$ = AttributeValueSemanticAction($1, $3, IDatt);}
	|			  WORLD ARROW ID {$$ = NULL;}
	;

declarationValue: ID																										{ $$ = DeclarationValueIDSemanticAction($1, IDvalue);}
	|			  STRING																									{ $$ = DeclarationValueStringSemanticAction($1, STRINGvalue);}
	|			  TRUE																										{ $$ = DeclarationValueBooleanSemanticAction($1, BOOLEANvalue);}
	|			  FALSE																										{ $$ = DeclarationValueBooleanSemanticAction($1, BOOLEANvalue);}
	|			  HEXCOLOR																									{ $$ = DeclarationValueHexSemanticAction($1, HEXCOLORvalue);}
	|			  INTEGER																									{ $$ = DeclarationValueIntSemanticAction($1, INTEGERvalue);}
	|			  attributeValue																							{ $$ = DeclarationValueAttributeSemanticAction($1, ATTvalue);}
	|			  OPEN_PARENTHESIS declarationValue CLOSE_PARENTHESIS														{ $$ = DeclarationValueInceptionSemanticAction($2, DECLARATIONvalue);}
	;

%%
