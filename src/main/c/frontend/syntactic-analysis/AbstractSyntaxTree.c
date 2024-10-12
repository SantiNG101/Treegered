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
			case MULTIPLE_w:
				releaseWorldAssignment(worldExpression->simpleWorldAssignment);
				releaseWorldExpression(worldExpression->worldExpression);
				break;
			case SIMPLE_w:
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

void releaseForestExpression(ForestExpression *forestExpression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (forestExpression != NULL) {
		switch(forestExpression->type){
			case MULTIPLE_f:
				releaseForestAssignment(forestExpression->simpleForestAssignment);
				releaseForestExpression(forestExpression->forestExpression);
				break;
			case SIMPLE_f:
				releaseForestAssignment(forestExpression->multipleForestAssignment);
				break;
		}
		free(forestExpression);
	}
}

void releaseForestAssignment(ForestAssignment *forestAssignment){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (forestAssignment != NULL) {
		release_ID(forestAssignment->id);
		releaseDeclarationValue(forestAssignment->declarationValue);
		free(forestAssignment);
	}
}

void releaseTreeExpression(TreeExpression *treeExpression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (treeExpression != NULL) {
		switch(treeExpression->type){
			case MULTIPLE_t:
				releaseTreeAssignment(treeExpression->simpleTreeAssignment);
				releaseTreeExpression(treeExpression->treeExpression);
				break;
			case SIMPLE_t:
				releaseTreeAssignment(treeExpression->multipleTreeAssignment);
				break;
		}
		free(treeExpression);
	}
}

void releaseTreeAssignment(TreeAssignment *treeAssignment){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (treeAssignment != NULL) {
		release_ID(treeAssignment->id);
		releaseDeclarationValue(treeAssignment->declarationValue);
		free(treeAssignment);
	}
}

void releaseGrowExpression(GrowExpression *growExpression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (growExpression != NULL) {
		release_ID(growExpression->id);
		free(growExpression);
	}
}

void releaseForExpression(ForExpression *forExpression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (forExpression != NULL) {
		release_ID(forExpression->id);
		if(forExpression->mainExpression != NULL) releaseMainExpression(forExpression->mainExpression);
		free(forExpression);
	}
}

void releaseMainExpression(MainExpression *mainExpression){
logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (mainExpression != NULL) {
		switch(mainExpression->type){
			case FOR_m:
				releaseForExpression(mainExpression->forExpression);
				break;
			case GROW_m:
				releaseGrowExpression(mainExpression->growExpression);
				break;
			case FOREST_m:
				releaseForestExpression(mainExpression->forestExpression);
				break;
			case TREE_m:
				releaseTreeExpression(mainExpression->treeExpression);
				break;
			case SIMPLE_m:
				release_ID(mainExpression->id);
				break;
		}
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
