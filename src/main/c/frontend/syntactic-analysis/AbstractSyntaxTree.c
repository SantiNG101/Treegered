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
		switch(worldAssignment->type){
			case ID_BY_VALUE:
				releaseDeclarationValue(worldAssignment->value);
				break;
			case ID_BY_OPP:
				releaseArithmeticOperation(worldAssignment->arithmeticOperation);
				break;
		}
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
		switch(forestAssignment->type){
			case ID_BY_VALUE:
				releaseDeclarationValue(forestAssignment->value);
				break;
			case ID_BY_OPP:
				releaseArithmeticOperation(forestAssignment->arithmeticOperation);
				break;
		}
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
		switch(treeAssignment->type){
			case ID_BY_VALUE:
				releaseDeclarationValue(treeAssignment->value);
				break;
			case ID_BY_OPP:
				releaseArithmeticOperation(treeAssignment->arithmeticOperation);
				break;
		}
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
		releaseMainExpression(forExpression->mainExpression);
		switch(forExpression->type){
			case CLASSIC_ITERATION:
				release_INTEGER(forExpression->rangeStart);
				release_INTEGER(forExpression->rangeEnd);
				break;
			case FOREST_ITERATION:
				release_ID(forExpression->forestId);
				break;
		}
		free(forExpression);
	}
}

void releaseArithmeticAssignation(ArithmeticAssignation *arithmeticAssignation){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (arithmeticAssignation != NULL) {
		switch(arithmeticAssignation->type){
			case ID_BY_VALUE:
				release_ID(arithmeticAssignation->id);
				releaseDeclarationValue(arithmeticAssignation->value);
				break;
			case ID_BY_OPP:
				release_ID(arithmeticAssignation->id);
				releaseArithmeticOperation(arithmeticAssignation->arithmeticOperation);
				break;
			case ATT_BY_VALUE:
				releaseAttributeValue(arithmeticAssignation->att);
				releaseDeclarationValue(arithmeticAssignation->value);
				break;
			case ATT_BY_OPP:
				releaseAttributeValue(arithmeticAssignation->att);
				releaseArithmeticOperation(arithmeticAssignation->arithmeticOperation);
				break;
		}
		free(arithmeticAssignation);
	}
}

void releaseGeneralAssignation(GeneralAssignation *generalAssignation){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (generalAssignation != NULL) {
		switch(generalAssignation->type){
			case ID_BY_VALUE:
				release_ID(generalAssignation->classType);
				release_ID(generalAssignation->id);
				releaseDeclarationValue(generalAssignation->value);
				break;
			case ID_BY_OPP:
				release_ID(generalAssignation->classType);
				release_ID(generalAssignation->id);
				releaseArithmeticOperation(generalAssignation->arithmeticOperation);
				break;
			case ATT_BY_VALUE:
				releaseAttributeValue(generalAssignation->att);
				releaseDeclarationValue(generalAssignation->value);
				break;
			case ATT_BY_OPP:
				releaseAttributeValue(generalAssignation->att);
				releaseArithmeticOperation(generalAssignation->arithmeticOperation);
				break;
		}
		free(generalAssignation);
	}
}

void releaseMainExpression(MainExpression *mainExpression){
logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (mainExpression != NULL) {
		switch(mainExpression->type){
			case GENERAL_ASSIGNATION_m:
				releaseGeneralAssignation(mainExpression->generalAssignation);
				break;
			case ARITHMETIC_m:
				releaseArithmeticAssignation(mainExpression->arithmeticAssignation);
				break;
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

void release_INTEGER(_INTEGER * intValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (intValue != NULL) {
		free(intValue);
	}
}

void release_BOOLEAN(_BOOLEAN * booleanValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (booleanValue != NULL) {
		free(booleanValue);
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
				release_BOOLEAN(declarationValue->booleanValue);
				break;
			case HEXCOLORvalue:
				release_HEXCOLOR(declarationValue->hexcolorValue);
				break;
			case INTEGERvalue:
				release_INTEGER(declarationValue->intValue);
				break;
			case ATTvalue:
				releaseAttributeValue(declarationValue->attValue);
				break;
		}
		free(declarationValue);
	}
}

void releaseArithmeticOperation(ArithmeticOperation *arithmeticOperation){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (arithmeticOperation != NULL) {
		switch(arithmeticOperation->type){
			case LV_RV:
				releaseDeclarationValue(arithmeticOperation->leftValue);
				releaseDeclarationValue(arithmeticOperation->rightValue);
				break;
			case LV_RO:
				releaseDeclarationValue(arithmeticOperation->leftValue);
				releaseArithmeticOperation(arithmeticOperation->rightOperation);
				break;
			case LO_RV:
				releaseArithmeticOperation(arithmeticOperation->leftOperation);
				releaseDeclarationValue(arithmeticOperation->rightValue);
				break;
			case LO_RO:
				releaseArithmeticOperation(arithmeticOperation->leftOperation);
				releaseArithmeticOperation(arithmeticOperation->rightOperation);
				break;
		}
		free(arithmeticOperation);
	}
}

void releaseAttributeValue(AttributeValue *attributeValue){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (attributeValue != NULL) {
		release_ID(attributeValue->variableID);
		release_ID(attributeValue->attribute);
		switch(attributeValue->type){
			case IDatt:
				release_ID(attributeValue->idValue);
				break;
			case STRINGatt:
				release_STRING(attributeValue->charValue);
				break;
			case BOOLEANatt:
				release_BOOLEAN(attributeValue->booleanValue);
				break;
			case HEXCOLORatt:
				release_HEXCOLOR(attributeValue->hexcolorValue);
				break;
			case INTEGERatt:
				release_INTEGER(attributeValue->intValue);
				break;
		}
		free(attributeValue);
	}
}