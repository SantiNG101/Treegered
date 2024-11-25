#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../frontend/syntactic-analysis/BisonParser.h"
#include "../../backend/symbol-table/Table.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include <stdarg.h>
#include <stdio.h>

// Por los otros?

/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 */
int generate(CompilerState * compilerState, CompilationStatus * compilationStatus);

#endif
