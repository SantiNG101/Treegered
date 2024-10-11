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
typedef struct GrowExpression GrowExpression;
typedef struct Attributes Attributes;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/*
	Enums for the ast
*/
enum ProgramType { WORLDLESS, WORLD };

/* 
	Structs for the ast
*/

struct _ID{
	Id idValue;
};


struct MainExpression {
    _ID *id;
};

struct WorldExpression {
	_ID *id;
};


struct ProgramExpression {
    union {
        MainExpression *wordlessMainExpression;
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
void releaseProgram(Program *program);
void releaseProgramExpression(ProgramExpression *programExpression);
void releaseWorldExpression(WorldExpression *worldExpression);
void releaseWorldExpressions(WorldExpressions *worldExpressions);
void releaseMainExpression(MainExpression *mainExpression);
void releaseMainExpressions(MainExpressions *mainExpressions);
void releaseTreeExpression(TreeExpression *treeExpression);

#endif
