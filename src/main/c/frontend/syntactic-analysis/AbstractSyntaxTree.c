#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseProgramExpression(ProgramExpression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch(expression->type){
			case WORLDFULL:
				releaseWorldExpression(expression->worldExpression);
			case WORLDLESS:
				releaseMainExpression(expression->mainExpression);
				break;
		}
		free(expression);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseProgramExpression(program->programExpression);
		free(program);
	}
}

/*ADDED*/
void releaseWorldExpression(WorldExpression *worldExpression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (worldExpression != NULL) {
		releaseID(worldExpression->id);
		free(worldExpression);
	}
}

void releaseMainExpression(MainExpression *mainExpression){
logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (mainExpression != NULL) {
		releaseID(mainExpression->id);
		free(mainExpression);
	}
}

void release_ID(_ID * idValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (idValue != NULL) {
		if (idValue->idValue != NULL) {
			free(idValue->idValue);
		}
		free(idValue);
	}
}