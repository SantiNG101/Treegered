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

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum ProgramType ProgramType;
typedef enum WorldExpressionsType WorldExpressionsType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;

typedef struct ProgramExpression ProgramExpression;
typedef struct MainExpressions MainExpressions;
typedef struct WorldExpression WorldExpression;

typedef struct MainExpression MainExpression;
typedef struct assignment assignment;
typedef struct WorldExpressions WorldExpressions;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/*
	Enums for the ast
*/
enum ExpressionType { ADDITION, DIVISION, FACTOR, MULTIPLICATION, SUBTRACTION };

enum FactorType { CONSTANT, EXPRESSION };

enum ProgramType { WORLDLESS, WORLD };

enum WorldExpressionsType { ASSIGNMENT, WORLD_EXPRESSIONS };

/* 
	Structs for the ast
*/

struct Constant {
    int value;
};

struct Factor {
    union {
        Constant *constant;
        Expression *expression;
    };
    FactorType type;
};

struct Expression {
    union {
        Factor *factor;
        struct {
            Expression *leftExpression;
            Expression *rightExpression;
        };
    };
    ExpressionType type;
};





struct WorldExpression {
	WorldExpressions *worldExpressions;
};

struct WorldExpressions {
	union {
		assignment *assignment;
		struct {
			assignment *assignment;
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
void releaseExpression(Expression *expression);
void releaseFactor(Factor *factor);
void releaseProgram(Program *program);

#endif
