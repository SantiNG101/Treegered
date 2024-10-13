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
typedef enum ArithmeticOperationType ArithmeticOperationType;
typedef enum DeclarationValueType DeclarationValueType;
typedef enum AttributeValueType AttributeValueType;

typedef struct _ID _ID;
typedef struct _HEXCOLOR _HEXCOLOR;
typedef struct _STRING _STRING;
typedef struct _BOOLEAN _BOOLEAN;
typedef struct _INTEGER _INTEGER;
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


/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/*
	Enums for the ast
*/
enum ProgramType { WORLDLESS, WORLDFULL };
enum WorldType { SIMPLE_w, MULTIPLE_w };
enum TreeType { EMPTY_t, SIMPLE_t, MULTIPLE_t };
enum TreeAssignType {SIMPLE_ta, MULTIPLE_ta};
enum ForestType { EMPTY_f, SIMPLE_f, MULTIPLE_f };
enum ForestAssignType {SIMPLE_fa, MULTIPLE_fa};
enum MainExpressionType { SIMPLE_m, TREE_m, FOREST_m, GROW_m, FOR_m, ARITHMETIC_m, GENERAL_ASSIGNATION_m};
enum ExpressionType {SIMPLE_e, MULTIPLE_e};
enum OperatorType {ADD_o, SUB_o, MUL_o, DIV_o};
enum ArithmeticOperationType {LV_RV, LV_RO, LO_RV, LO_RO};
enum AssignationType {ID_BY_VALUE, ID_BY_OPP, ATT_BY_VALUE, ATT_BY_OPP};
enum ForType{CLASSIC_ITERATION, FOREST_ITERATION};
enum DeclarationValueType { IDvalue, STRINGvalue, BOOLEANvalue, HEXCOLORvalue, INTEGERvalue, ATTvalue};
enum AttributeValueType { IDatt, STRINGatt, BOOLEANatt, HEXCOLORatt, INTEGERatt};

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

struct DeclarationValue{
    union{
        _ID *idValue;
        _STRING *charValue;
        _BOOLEAN *booleanValue;
        _HEXCOLOR *hexcolorValue;
        _INTEGER *intValue;
        AttributeValue *attValue;
    };
    DeclarationValueType type;
};

struct AttributeValue{
    _ID *variableID;
    _ID *attribute;
    union{
        _ID *idValue;
        _STRING *charValue;
        _BOOLEAN *booleanValue;
        _HEXCOLOR *hexcolorValue;
        _INTEGER *intValue;
    };
    AttributeValueType type;
};

struct GeneralAssignation{
    union{
        struct{
            _ID *id;
            _ID *classType;
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
        DeclarationValue *leftValue;
        ArithmeticOperation *leftOperation;
    };
    union{
        DeclarationValue *rightValue;
        ArithmeticOperation *rightOperation;
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
        MainExpression *worldlessMainExpression;
        struct {
            WorldExpression *worldExpression;
            MainExpression *mainExpression;
        };
    };
	ProgramType type;
};

struct Program {
    ProgramExpression *programExpression;
};

/**
 * Node recursive destructors.
 
void release_ID(_ID *ID);
void release_STRING(_STRING * charValue);
void release_HEXCOLOR(_HEXCOLOR * hexcolorValue);
void release_INTEGER(_INTEGER *intValue);
void release_BOOLEAN(_BOOLEAN *booleanValue);
void releaseProgram(Program * program);
void releaseProgramExpression(ProgramExpression *programExpression);
void releaseWorldExpression(WorldExpression *worldExpression);
void releaseWorldAssignment(WorldAssignment *worldAssignment);
void releaseDeclarationValue(DeclarationValue *declarationValue);
void releaseMainExpression(MainExpression *mainExpression);
void releaseTreeExpression(TreeExpression *treeExpression);
void releaseTreeAssignment(TreeAssignment *treeAssignment);
void releaseForestExpression(ForestExpression *forestExpression);
void releaseForestAssignment(ForestAssignment *forestAssignment);
void releaseGrowExpression(GrowExpression *growExpression);
void releaseForExpression(ForExpression *forExpression);
void releaseArithmeticAssignation(ArithmeticAssignation *arithmeticAssignation);
void releaseArithmeticOperation(ArithmeticOperation *arithmeticOperation);
void releaseGeneralAssignation(GeneralAssignation *generalAssignation);
void releaseAttributeValue(AttributeValue *attributeValue);

*/

#endif