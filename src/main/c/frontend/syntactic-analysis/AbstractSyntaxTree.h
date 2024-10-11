#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include <stdlib.h>

#include "../../shared/Logger.h"

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum ProgramType ProgramType;
typedef enum WorldExpressionsType WorldExpressionsType;
typedef enum MainExpressionsType MainExpressionsType;
typedef enum MainExpressionType MainExpressionType;

typedef struct ID ID;

typedef struct Program Program;

typedef struct ProgramExpression ProgramExpression;
typedef struct MainExpressions MainExpressions;
typedef struct WorldExpression WorldExpression;

typedef struct MainExpression MainExpression;
typedef struct WorldExpressions WorldExpressions;

typedef struct TreeExpression TreeExpression;
typedef struct ForestExpression ForestExpression;
typedef struct ForExpression ForExpression;
typedef struct ConditionalExpression ConditionalExpression;
typedef struct Assignment Assignment;
typedef struct ArithmeticExpression ArithmeticExpression;

typedef struct Attributes Attributes;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/*
	Enums for the ast
*/
enum ProgramType { WORLDLESS, WORLD };

enum WorldExpressionsType { ASSIGNMENT, WORLD_EXPRESSIONS };

enum AssignmentExpressionsType {UNIQUE, MULTIPLE};

enum AssignmentValues {INTEGER, TRUE, FALSE, STRING, HEXCOLOR};

enum ConditionalExpressionType {IF, ELSE_IF};

enum ConditionalClauseExpressionsType {VALUE, EXPRESSION};

enum ConditionalClauseExpressionsType {LESSER, GREATER, EQUAL, LESSER_EQUAL, GREATER_EQUAL, NOT_EQUAL};

enum MainExpressionsType { MAIN_EXPRESSION, MAIN_EXPRESSIONS };

enum MainExpressionType { TREE, FOREST, FOR, ASSIGNMENT, CONDITIONAL, ARITHMETIC };

/* 
	Structs for the ast
*/

struct ID{
	Id idValue;
};

struct AssignmentExpression{
	ID *attribute;
	union{
		char* *value;
		int *value;
		boolean *value;
		hexcolor *value;
	}
	AssignmentValues valueType;
};

struct AssignmentExpressions{
	union{
		AssignmentExpression *AssignmentExpression;
		struct{
			AssignmentExpression *AssignmentExpression;
			AssignmentExpressions *AssignmentExpressions;
		}
	}
	AssignmentExpressionsType type;
};

struct TreeExpression {
    ID * id;
    AssignmentExpressions * AssignmentExpressions;
};

struct ForestExpression {
    ID * id;
    AssignmentExpressions * AssignmentExpressions;
};

struct ForExpression{
	ID *id;
	int rangeStart;
	int rangeEnd;
	MainExpressions *mainExpressions;
};

struct ConditionalClauseExpression{
	union{
		char* *value;
		int *value;
		boolean *value;
		hexcolor *value;
		MainExpressions *expression;
	}
	ConditionalClauseExpressionType type;
}

struct ConditionalClauseExpressions{
	union{
		ConditionalClauseExpression *conditionalClauseExpression;
		struct{
			ConditionalClauseExpression *leftConditionalClauseExpression;
			ConditionalClauseExpression *rightConditionalClauseExpressions;
		}
	}
	ConditionalClauseExpressionsType type;
};

struct ConditionalExpression{
	ConditionalClauseExpressions *conditionalClauseExpressions;
	MainExpressions *mainExpressions;
	ConditionalExpressionType *conditionalExpressionType;
};

struct GrowExpression{
	ID *id;
};

struct MainExpression {
    union {
        TreeExpression *treeExpression;
        ForestExpression *forestExpression;
		AssignmentExpression *assignmentExpression;
        ForExpression *forExpression;
        ArithmeticExpression *arithmeticExpression;
        ConditionalExpression *conditionalExpression;
		GrowExpression *growExpression;
    };
    MainExpressionType type;
};

struct MainExpressions {
    union {
        MainExpression *mainExpression;
        struct {
            MainExpression *mainExpression;
            MainExpressions *mainExpressions;
        };
    };
    MainExpressionsType type;
};

struct WorldExpression {
	WorldExpressions *worldExpressions;
};

struct WorldExpressions {
	union {
		AssignmentExpression *AssignmentExpression;
		struct {
			AssignmentExpression *AssignmentExpression;
			WorldExpressions *worldExpressions;
		};
	};
	WorldExpressionsType type;
};

struct ProgramExpression {
    union {
        MainExpressions *mainExpressions;
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

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant *constant);
void releaseID(ID *ID);
void releaseProgram(Program *program);
void releaseProgramExpression(ProgramExpression *programExpression);
void releaseWorldExpression(WorldExpression *worldExpression);
void releaseWorldExpressions(WorldExpressions *worldExpressions);
void releaseMainExpression(MainExpression *mainExpression);
void releaseMainExpressions(MainExpressions *mainExpressions);
void releaseTreeExpression(TreeExpression *treeExpression);

#endif
