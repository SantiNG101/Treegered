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
typedef enum ForestType ForestType;
typedef enum MainExpressionType MainExpressionType;
typedef enum DeclarationValueType DeclarationValueType;

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
typedef struct MainExpression MainExpression;
typedef struct TreeExpression TreeExpression;
typedef struct TreeAssignment TreeAssignment;
typedef struct ForestExpression ForestExpression;
typedef struct ForestAssignment ForestAssignment;
typedef struct GrowExpression GrowExpression;
typedef struct ForExpression ForExpression;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/*
	Enums for the ast
*/
enum ProgramType { WORLDLESS, WORLDFULL };
enum WorldType { SIMPLE_w, MULTIPLE_w };
enum TreeType { EMPTY_t, SIMPLE_t, MULTIPLE_t };
enum ForestType { EMPTY_f, SIMPLE_f, MULTIPLE_f };
enum MainExpressionType { SIMPLE_m, TREE_m, FOREST_m, GROW_m, FOR_m};
enum DeclarationValueType { IDvalue, STRINGvalue, BOOLEANvalue, HEXCOLORvalue, INTEGERvalue};

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
    };
    DeclarationValueType type;
};

struct ForExpression{
    _ID *id;
    _INTEGER rangeStart;
    _INTEGER rangeEnd;
    MainExpression *mainExpression;
};

struct GrowExpression{
    _ID *id;
};

struct ForestAssignment{
    _ID *id;
    DeclarationValue *declarationValue;
};

struct ForestExpression{
    _ID *id;
    union{
        ForestAssignment *simpleForestAssignment;
        struct{
            ForestAssignment *multipleForestAssignment;
            ForestExpression *forestExpression;
        };
    };
    ForestType type;
};

struct TreeAssignment{
    _ID *id;
    DeclarationValue *declarationValue;
};

struct TreeExpression{
    _ID *id;
    union{
        TreeAssignment *simpleTreeAssignment;
        struct{
            TreeAssignment *multipleTreeAssignment;
            TreeExpression *treeExpression;
        };
    };
    TreeType type;
};

struct MainExpression {
    union{
        _ID *id;
        TreeExpression *treeExpression;
        ForestExpression *forestExpression;
        GrowExpression *growExpression;
        ForExpression *forExpression;
    };
    MainExpressionType type;
};

struct WorldAssignment{
    _ID *id;
    DeclarationValue *declarationValue;
};

struct WorldExpression {
	union {
        WorldAssignment *simpleWorldAssignment;
        struct {
            WorldAssignment *multipleWorldAssignment;
            WorldExpression *worldExpression;
        };
    };
	WorldType type;
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
 */
void release_ID(_ID *ID);
void release_STRING(_STRING * charValue);
void release_HEXCOLOR(_HEXCOLOR * hexcolorValue);
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

#endif
