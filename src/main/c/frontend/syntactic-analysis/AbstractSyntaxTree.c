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
void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseProgramExpression(program->programExpression);
		free(program);
	}
}

void releaseProgramExpression(ProgramExpression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch(expression->type){
			case WORLDFULL:
				releaseWorldExpression(expression->worldExpression);
				releaseMainExpression(expression->mainExpression);
				break;
			case WORLDLESS:
				releaseMainExpression(expression->worldlessMainExpression);
				break;
		}
		free(expression);
	}
}

void releaseWorldExpression(WorldExpression *worldExpression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (worldExpression != NULL) {
		switch(worldExpression->type){
			case MULTIPLE:
				releaseWorldAssignment(worldExpression->simpleWorldAssignment);
				releaseWorldExpression(worldExpression->worldExpression);
				break;
			case SIMPLE:
				releaseWorldAssignment(worldExpression->multipleWorldAssignment);
				break;
		}
		free(worldExpression);
	}
}

void releaseWorldAssignment(WorldAssignment *worldAssignment){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (worldAssignment != NULL) {
		release_ID(worldAssignment->id);
		releaseDeclarationValue(worldAssignment->declarationValue);
		free(worldAssignment);
	}
}

void releaseMainExpression(MainExpression *mainExpression){
logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (mainExpression != NULL) {
		release_ID(mainExpression->id);
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

void release_STRING(_STRING * charValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (charValue != NULL) {
		if (charValue->value != NULL) {
			free(charValue->value);
		}
		free(charValue);
	}
}

void release_HEXCOLOR(_HEXCOLOR * hexcolorValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (hexcolorValue != NULL) {
		if (hexcolorValue->value != NULL) {
			free(hexcolorValue->value);
		}
		free(hexcolorValue);
	}
}

void releaseDeclarationValue(DeclarationValue *declarationValue){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (declarationValue != NULL) {
		switch(declarationValue->type){
			case IDvalue:
				release_ID(declarationValue->idValue);
				break;
			case STRINGvalue:
				release_STRING(declarationValue->charValue);
				break;
			case BOOLEANvalue:
				break;
			case HEXCOLORvalue:
				release_HEXCOLOR(declarationValue->hexcolorValue);
				break;
			case INTEGERvalue:
				break;
		}
		free(declarationValue);
	}
}
