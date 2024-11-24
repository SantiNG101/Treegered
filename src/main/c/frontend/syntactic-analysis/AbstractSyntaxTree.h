#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */
typedef enum ProgramType ProgramType;
typedef enum WorldType WorldType;
typedef enum TreeType TreeType;
typedef enum TreeAssignType TreeAssignType;
typedef enum ForestType ForestType;
typedef enum ForestAssignType ForestAssignType;
typedef enum MainExpressionType MainExpressionType;
typedef enum ExpressionType ExpressionType;
typedef enum OperatorType OperatorType;
typedef enum AssignationType AssignationType;
typedef enum ForType ForType;
typedef enum ConditionalType ConditionalType;
typedef enum ConditionalClauseType ConditionalClauseType;
typedef enum ComparissonType ComparissonType;
typedef enum ArithmeticOperationType ArithmeticOperationType;
typedef enum DeclarationValueType DeclarationValueType;
typedef enum AttributeValueType AttributeValueType;

typedef struct _ID _ID;
typedef struct _HEXCOLOR _HEXCOLOR;
typedef struct _STRING _STRING;
typedef struct _BOOLEAN _BOOLEAN;
typedef struct _INTEGER _INTEGER;
typedef struct type type;
typedef struct DeclarationValue DeclarationValue;
typedef struct Program Program;

typedef struct ProgramExpression ProgramExpression;
typedef struct WorldExpression WorldExpression;
typedef struct WorldAssignment WorldAssignment;
typedef struct WorldAssignments WorldAssignments;
typedef struct MainExpression MainExpression;
typedef struct MainExpressions MainExpressions;
typedef struct TreeExpression TreeExpression;
typedef struct TreeAssignment TreeAssignment;
typedef struct TreeAssignments TreeAssignments;
typedef struct ForestExpression ForestExpression;
typedef struct ForestAssignment ForestAssignment;
typedef struct ForestAssignments ForestAssignments;
typedef struct GrowExpression GrowExpression;
typedef struct ForExpression ForExpression;
typedef struct ArithmeticAssignation ArithmeticAssignation;
typedef struct ArithmeticOperation ArithmeticOperation;
typedef struct GeneralAssignation GeneralAssignation;
typedef struct AttributeValue AttributeValue;
typedef struct ConditionalExpression ConditionalExpression;
typedef struct ConditionalClause ConditionalClause;


/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/*
	Enums for the ast
*/
enum ProgramType { WORLDLESS, WORLDFULL };
enum WorldType { SIMPLE_w, MULTIPLE_w };
enum TreeType { EMPTY_t, FULL_t };
enum TreeAssignType {SIMPLE_ta, MULTIPLE_ta};
enum ForestType { EMPTY_f, FULL_f };
enum ForestAssignType {SIMPLE_fa, MULTIPLE_fa};
enum MainExpressionType { SIMPLE_m, TREE_m, FOREST_m, GROW_m, FOR_m, ARITHMETIC_m, GENERAL_ASSIGNATION_m, CONDITIONAL_m};
enum ExpressionType {SIMPLE_e, MULTIPLE_e};
enum OperatorType {ADD_o, SUB_o, MUL_o, DIV_o, NONE};
enum ArithmeticOperationType {LV_RV, LV_RO, LO_RV, LO_RO, PARENTHESIS};
enum AssignationType {ID_BY_VALUE_TYPE, ID_BY_OPP_TYPE, ID_BY_VALUE, ID_BY_OPP, ATT_BY_VALUE, ATT_BY_OPP};
enum ForType{CLASSIC_ITERATION, FOREST_ITERATION};
enum ConditionalType{IF_c, ELSE_c};
enum ConditionalClauseType{PARENTHESIS_c, V_V, V_C, C_V, C_C}; //distinguish left and rights between declarationValues and conditionalClauses
enum ComparissonType{NONE_c, EQUIVALENT_c, DIFFERENT_c, LESSER_EQUAL_c, GREATER_EQUAL_c, LESSERTHAN_c, GREATERTHAN_c};
enum DeclarationValueType { IDvalue, STRINGvalue, BOOLEANvalue, HEXCOLORvalue, INTEGERvalue, ATTvalue, DECLARATIONvalue};
enum AttributeValueType { IDatt, WORLDatt};

/* 
	Structs for the ast
*/

struct _ID{
	Id idValue;
};

struct _HEXCOLOR{
	Hexcolor value;
};

struct _STRING{
	char* value;
};

struct _BOOLEAN{
	boolean value;
};

struct _INTEGER{
	int value;
};

struct type{
    Class class;
};

struct DeclarationValue{
    union{
        _ID *idValue;
        _STRING *charValue;
        _BOOLEAN *booleanValue;
        _HEXCOLOR *hexcolorValue;
        _INTEGER *intValue;
        AttributeValue *attValue;
		DeclarationValue * declareValue;
    };
    DeclarationValueType type;
};

struct AttributeValue{
    union{
        struct{
            _ID *variableID;
            _ID *attributeID;
        };
        _ID *attribute;
    };
    
    AttributeValueType type;
};

struct ConditionalClause{
	    union{
    		struct{
        		DeclarationValue *leftValueDeclare;
        		ConditionalClause *rightConditionalClause;
    		};
    		struct{
        		DeclarationValue *rightValueDeclare;
       			ConditionalClause *leftConditionalClause;
    		};
            struct{
        		DeclarationValue *leftValue;
       			DeclarationValue *rightValue;
    		};
            struct{
        		ConditionalClause *leftConditional;
       			ConditionalClause *rightConditional;
    		};
			ConditionalClause * conditionalClause;
		};
    ComparissonType comparissonType;//aca seria para identificar si fue ==, !=, >, etc  USAR NONE PARA EL DE PARENTHESIS   
    ConditionalClauseType conditionalType;//aca para saber si fue (), dvalue > condition, etc
};

struct ConditionalExpression{
    ConditionalClause *conditionalClause;
    MainExpressions *ifMainExpressions;
    MainExpressions *elseMainExpressions;//holii, este nullable asi es null si es solo if, o con vaor si hubo un else
    ConditionalType type;
};

struct GeneralAssignation{
    union{
        union{
            _ID *id;
            struct{
                _ID *idDeclared;
                type *classType;
            };
        };        
        AttributeValue *att;
    };
    union{
        DeclarationValue *value;
        ArithmeticOperation *arithmeticOperation;
    };
    AssignationType type;
};

struct ArithmeticOperation{
    OperatorType operator;
	union{
        ArithmeticOperation * arithOp;
        struct{
    		union{
        		DeclarationValue *leftValue;
        		ArithmeticOperation *leftOperation;
    		};
    		union{
        		DeclarationValue *rightValue;
       			ArithmeticOperation *rightOperation;
    		};
		};
	};
    ArithmeticOperationType type;
};

struct ArithmeticAssignation{
    union{
        _ID *id;
        AttributeValue *att;
    };
    OperatorType operator;
    union{
        DeclarationValue *value;
        ArithmeticOperation *arithmeticOperation;
    };
    AssignationType type;
};

struct ForExpression{
    _ID *id;
    union{
        struct{
            _INTEGER *rangeStart;
            _INTEGER *rangeEnd;
        };
        _ID *forestId;
    };
    MainExpressions *mainExpressions;
    ForType type;
};

struct GrowExpression{
    _ID *id;
};

struct ForestAssignment{
    _ID *id;
    union{
        DeclarationValue *value;
        ArithmeticOperation *arithmeticOperation;
    };
    AssignationType type;
};

struct ForestAssignments{
	union{
		ForestAssignment * singleForestAssignment;
		struct{
			ForestAssignment * multipleForestAssignment;
			ForestAssignments * forestAssignments;
		};
	};
	ForestAssignType type;
};

struct ForestExpression{
    _ID *id;
	ForestAssignments * forestAssignments;
    ForestType type;
};


struct TreeAssignment{
    _ID *id;
    union{
        DeclarationValue *value;
        ArithmeticOperation *arithmeticOperation;
    };
    AssignationType type;
};

struct TreeAssignments{
	union{
		TreeAssignment * singleTreeAssignment;
		struct{
			TreeAssignment * multipleTreeAssignment;
			TreeAssignments * treeAssignments;
		};
	};
	TreeAssignType type;
};

struct TreeExpression{
    _ID *id;
	TreeAssignments * treeAssignments;
    TreeType type;
};

struct MainExpression {
    union{
        TreeExpression *treeExpression;
        ForestExpression *forestExpression;
        GrowExpression *growExpression;
        ForExpression *forExpression;
        ArithmeticAssignation *arithmeticAssignation;
        GeneralAssignation *generalAssignation;
        ConditionalExpression *conditionalExpression;
    };
    MainExpressionType type;
};

struct MainExpressions {
	union{
		MainExpression * singleMainExpression;
		struct{
			MainExpression * multipleMainExpression;
			MainExpressions * mainExpressions;
		};
	};
	ExpressionType type;
};

struct WorldAssignment{
    _ID *id;
    union{
		DeclarationValue *value;
		ArithmeticOperation *arithmeticOperation;
    };
    AssignationType type;
};

struct WorldAssignments{
	union{
		WorldAssignment * singleWorldAssignment;
		struct{
			WorldAssignment * multipleWorldAssignment;
			WorldAssignments * worldAssignments;
		};
	};
    WorldType wType;
};

struct WorldExpression {
	WorldAssignments * worldAssignments;
};

struct ProgramExpression {
    union {
        MainExpressions *worldlessMainExpressions;
        struct {
            WorldExpression *worldExpression;
            MainExpressions *mainExpressions;
        };
    };
	ProgramType type;
};

struct Program {
    ProgramExpression *programExpression;
};

/* Node recursive destructors. */
 
void release_type(type * classType);

void release_ID(_ID *ID);  /*✅*/

void release_STRING(_STRING * charValue);  /*✅*/

void release_HEXCOLOR(_HEXCOLOR * hexcolorValue);  /*✅*/

void release_INTEGER(_INTEGER *intValue);  /*✅*/

void release_BOOLEAN(_BOOLEAN *booleanValue);  /*✅*/

void releaseProgram(Program * program); /*✅*/

void releaseProgramExpression(ProgramExpression *programExpression); /*✅*/

void releaseWorldExpression(WorldExpression *worldExpression); /*✅*/

void releaseWorldAssignments(WorldAssignments * worldAssignments); /*✅*/

void releaseWorldAssignment(WorldAssignment *worldAssignment); /*✅*/

void releaseDeclarationValue(DeclarationValue *declarationValue); /*✅*/

void releaseMainExpressions(MainExpressions *mainExpressions);  /*✅*/

void releaseMainExpression(MainExpression *mainExpression); /*✅*/

void releaseTreeExpression(TreeExpression *treeExpression); /*✅*/

void releaseTreeAssignment(TreeAssignment *treeAssignment); /*✅*/

void releaseTreeAssignments(TreeAssignments *treeAssignments); /*✅*/

void releaseForestExpression(ForestExpression *forestExpression);/*✅*/

void releaseForestAssignment(ForestAssignment *forestAssignment);/*✅*/

void releaseForestAssignments(ForestAssignments *forestAssignments);/*✅*/

void releaseGrowExpression(GrowExpression *growExpression); /*✅*/

void releaseForExpression(ForExpression *forExpression); /*✅*/

void releaseArithmeticAssignation(ArithmeticAssignation *arithmeticAssignation); /*✅*/

void releaseArithmeticOperation(ArithmeticOperation *arithmeticOperation); /*✅*/

void releaseGeneralAssignation(GeneralAssignation *generalAssignation); /*✅*/

void releaseConditionalExpression(ConditionalExpression *conditionalExpression); /*✅*/

void releaseConditionalClause(ConditionalClause *conditionalClause); /*✅*/

void releaseAttributeValue(AttributeValue *attributeValue);  /*✅*/



#endif