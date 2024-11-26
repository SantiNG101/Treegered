#ifndef VALIDATE_HEADER
#define VALIDATE_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../frontend/syntactic-analysis/BisonParser.h"
#include "../../backend/symbol-table/Table.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include <stdarg.h>
#include <stdio.h>

#define WORLD_HEIGHT 100
#define WORLD_WIDTH 200

#define DEFAULT_FOREST_HEIGHT 25
#define DEFAULT_FOREST_WIDTH 50

// Por los otros?

/** Initialize module's internal state. */
void initializeValidateModule();

/** Shutdown module's internal state. */
void shutdownValidateModule();

/**
 * Generates the final output using the current compiler state.
 */
int validate(CompilerState * compilerState, CompilationStatus * compilationStatus);

#endif
