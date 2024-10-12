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

typedef struct _ID _ID;
typedef struct Program Program;

typedef struct ProgramExpression ProgramExpression;
typedef struct MainExpression MainExpression;
typedef struct WorldExpression WorldExpression;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/*
	Enums for the ast
*/
enum ProgramType { WORLDLESS, WORLDFULL };

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
void releaseProgram(Program * program);
void releaseProgramExpression(ProgramExpression *programExpression);
void releaseWorldExpression(WorldExpression *worldExpression);
void releaseMainExpression(MainExpression *mainExpression);

#endif
