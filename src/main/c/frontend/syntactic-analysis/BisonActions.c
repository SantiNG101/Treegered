#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */
Program * ExpressionProgramSemanticAction(CompilerState * compilerState, ProgramExpression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->programExpression = expression;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}


/*ADDED*/

type * TypeSemanticAction(Class class) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	type * _type = calloc(1, sizeof(type));
	_type->class = class;
	return _type;
}

AttributeValue * AttributeValueSemanticAction(Id varId, Id attId, AttributeValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	AttributeValue * attributeValue = calloc(1, sizeof(AttributeValue));

	_ID * aux1 = calloc(1, sizeof(_ID));
	aux1->idValue = varId;
	attributeValue->variableID = aux1;

	_ID * aux2 = calloc(1, sizeof(_ID));
	aux2->idValue = attId;

	attributeValue->attributeID = aux2;
	attributeValue->type = type;

	return attributeValue;
}

AttributeValue * AttributeValueWorldSemanticAction(Id attId, AttributeValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	AttributeValue * worldAttributeValue = calloc(1, sizeof(AttributeValue));

	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = attId;

	worldAttributeValue->attribute = aux;
	worldAttributeValue->type = type;

	return worldAttributeValue;
}

DeclarationValue * DeclarationValueIDSemanticAction(Id id, DeclarationValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeclarationValue * declarationValueID = calloc(1, sizeof(DeclarationValue));
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	declarationValueID->idValue = aux;
	declarationValueID->type = type;
	return declarationValueID;
}

DeclarationValue * DeclarationValueStringSemanticAction(char* string, DeclarationValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeclarationValue * declarationValueString = calloc(1, sizeof(DeclarationValue));
	_STRING * aux = calloc(1, sizeof(_STRING));

	//remove initial and final '
	int newLength = strlen(string) - 2;
	 char *newString = calloc(newLength + 1, sizeof(char));
	strncpy(newString, string + 1, newLength);

	aux->value = newString;
	declarationValueString->charValue = aux;
	declarationValueString->type = type;
	return declarationValueString;
}
DeclarationValue * DeclarationValueBooleanSemanticAction(boolean value, DeclarationValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeclarationValue * declarationValueBoolean = calloc(1, sizeof(DeclarationValue));
	_BOOLEAN * aux = calloc(1, sizeof(_BOOLEAN));
	aux->value = value;
	declarationValueBoolean->booleanValue = aux;
	declarationValueBoolean->type = type;
	return declarationValueBoolean;
}
DeclarationValue * DeclarationValueHexSemanticAction(Hexcolor color, DeclarationValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeclarationValue * declarationValueHex = calloc(1, sizeof(DeclarationValue));
	_HEXCOLOR * aux = calloc(1, sizeof(_HEXCOLOR));
	aux->value = color;
	declarationValueHex->hexcolorValue = aux;
	declarationValueHex->type = type;
	return declarationValueHex;
}
DeclarationValue * DeclarationValueIntSemanticAction(int integer, DeclarationValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeclarationValue * declarationValueInteger = calloc(1, sizeof(DeclarationValue));
	_INTEGER * aux = calloc(1, sizeof(_INTEGER));
	aux->value = integer;
	declarationValueInteger->intValue = aux;
	declarationValueInteger->type = type;
	return declarationValueInteger;
}

DeclarationValue * DeclarationValueAttributeSemanticAction(AttributeValue * attValue, DeclarationValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeclarationValue * declarationValueAttribute = calloc(1, sizeof(DeclarationValue));
	declarationValueAttribute->attValue = attValue;
	declarationValueAttribute->type = type;
	return declarationValueAttribute;
}

DeclarationValue * DeclarationValueInceptionSemanticAction(DeclarationValue * declarationValue, DeclarationValueType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeclarationValue * declarationValueInception = calloc(1, sizeof(DeclarationValue));
	declarationValueInception->declareValue = declarationValue;
	declarationValueInception->type = type;
	return declarationValueInception;
}

ArithmeticOperation * ArithmeticOperationAllDeclarationValueSemanticAction(OperatorType operator, DeclarationValue* left, DeclarationValue* right, ArithmeticOperationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticOperation * arithmeticOperation = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperation->operator = operator;
	arithmeticOperation->leftValue = left;
	arithmeticOperation->rightValue = right;
	arithmeticOperation->type = type;
	return arithmeticOperation;
}

ArithmeticOperation * ArithmeticOperationAllSemanticAction(OperatorType operator, ArithmeticOperation* left, ArithmeticOperation* right, ArithmeticOperationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticOperation * arithmeticOperation = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperation->operator = operator;
	arithmeticOperation->leftOperation = left;
	arithmeticOperation->rightOperation = right;
	arithmeticOperation->type = type;
	return arithmeticOperation;
}

ArithmeticOperation * ArithmeticOperationRightDeclarationLeftSemanticAction(OperatorType operator, DeclarationValue* left, ArithmeticOperation* right, ArithmeticOperationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticOperation * arithmeticOperation = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperation->operator = operator;
	arithmeticOperation->leftValue = left;
	arithmeticOperation->rightOperation = right;
	arithmeticOperation->type = type;
	return arithmeticOperation;
}

ArithmeticOperation * ArithmeticOperationLeftDeclarationRightSemanticAction(OperatorType operator, ArithmeticOperation* left, DeclarationValue* right, ArithmeticOperationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticOperation * arithmeticOperation = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperation->operator = operator;
	arithmeticOperation->leftOperation = left;
	arithmeticOperation->rightValue = right;
	arithmeticOperation->type = type;
	return arithmeticOperation;
}

ArithmeticOperation * ArithmeticOperationInceptionSemanticAction(OperatorType operator, ArithmeticOperation* operation, ArithmeticOperationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticOperation * arithmeticOperationInception = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperationInception->operator = operator;
	arithmeticOperationInception->arithOp = operation;
	arithmeticOperationInception->type = type;
	return arithmeticOperationInception;
}

ProgramExpression * WorldlessProgramExpressionSemanticAction(MainExpressions * mainExpressions, ProgramType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProgramExpression * programExpression = calloc(1, sizeof(ProgramExpression));
	programExpression->worldlessMainExpressions = mainExpressions;
	programExpression->type = type;
	return programExpression;
}

ProgramExpression * WorldProgramExpressionSemanticAction(MainExpressions * mainExpressions, WorldExpression* worldExpression, ProgramType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ProgramExpression * programExpression = calloc(1, sizeof(ProgramExpression));
	programExpression->worldExpression = worldExpression;
	programExpression->mainExpressions = mainExpressions;
	programExpression->type = type;
	return programExpression;
}

WorldAssignment * WorldAssignmentDeclarationSemanticAction(Id id, DeclarationValue* value, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WorldAssignment * worldAssignment = calloc(1, sizeof(WorldAssignment));
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	worldAssignment->id = aux;
	worldAssignment->value = value;
	worldAssignment->type = type;
	return worldAssignment;
}

WorldAssignment * WorldAssignmentArithmeticSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WorldAssignment * worldAssignment = calloc(1, sizeof(WorldAssignment));
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	worldAssignment->id = aux;
	worldAssignment->arithmeticOperation = operation;
	worldAssignment->type = type;
	return worldAssignment;
}

WorldAssignments * SimpleWorldAssignmentsSemanticAction(WorldAssignment * worldAssignment, WorldType wType){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WorldAssignments * simpleWorld = calloc(1, sizeof(WorldAssignments));
	
	simpleWorld->singleWorldAssignment = worldAssignment;
	simpleWorld->wType = wType;
	
	return simpleWorld;
}

WorldAssignments * MultipleWorldAssignmentsSemanticAction(WorldAssignments * worldAssignments, WorldAssignment * worldAssignment, WorldType wType){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WorldAssignments * multiWorld = calloc(1, sizeof(WorldAssignments));
	
	multiWorld->multipleWorldAssignment = worldAssignment;
	multiWorld->worldAssignments = worldAssignments;
	multiWorld->wType = wType;
	
	return multiWorld;
}

WorldExpression * WorldExpressionSemanticAction(WorldAssignments* worldAssignments){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WorldExpression * worldExp = calloc(1, sizeof(WorldExpression));
	
	worldExp->worldAssignments = worldAssignments;
	
	return worldExp;
}

ForestAssignment * DeclarationForestAssignmentSemanticAction(Id id, DeclarationValue* value, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForestAssignment * declareForest = calloc(1, sizeof(ForestAssignment));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	declareForest->id = aux;
	declareForest->value = value;
	declareForest->type = type;
	
	return declareForest;
}
ForestAssignment * ArithmeticForestAssignmentSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForestAssignment * arithForest = calloc(1, sizeof(ForestAssignment));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	arithForest->id = aux;
	arithForest->arithmeticOperation = operation;
	arithForest->type = type;
	
	return arithForest;
}

ForestAssignments * SimpleForestAssignmentsSemanticAction(ForestAssignment * forestAssignment, ForestAssignType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForestAssignments * simpleForest = calloc(1, sizeof(ForestAssignments));
	
	simpleForest->singleForestAssignment = forestAssignment;
	simpleForest->type = type;
	
	return simpleForest;
}

ForestAssignments * MultipleForestAssignmentsSemanticAction(ForestAssignments * forestAssignments, ForestAssignment * forestAssignment, ForestAssignType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForestAssignments * multiForest = calloc(1, sizeof(ForestAssignments));
	
	multiForest->multipleForestAssignment = forestAssignment;
	multiForest->forestAssignments = forestAssignments;
	multiForest->type = type;
	
	return multiForest;
}

ForestExpression * ForestExpressionSemanticAction(Id id, ForestAssignments * forestAssignments, ForestType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForestExpression * notDefaultForest = calloc(1, sizeof(ForestExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	notDefaultForest->id = aux;
	notDefaultForest->forestAssignments = forestAssignments;
	notDefaultForest->type = type;
	
	return notDefaultForest;
}

ForestExpression * DefaultForestExpressionSemanticAction(Id id, ForestType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForestExpression * defaultForest = calloc(1, sizeof(ForestExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	defaultForest->id = aux;
	defaultForest->type = type;
	
	return defaultForest;
}

TreeAssignment * DeclarationTreeAssignmentSemanticAction(Id id, DeclarationValue* value, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TreeAssignment * declarationTree = calloc(1, sizeof(TreeAssignment));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	declarationTree->id = aux;
	declarationTree->value = value;
	declarationTree->type = type;
	
	return declarationTree;	
}

TreeAssignment * ArithmeticTreeAssignmentSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TreeAssignment * arithmeticTree = calloc(1, sizeof(TreeAssignment));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	arithmeticTree->id = aux;
	arithmeticTree->arithmeticOperation = operation;
	arithmeticTree->type = type;
	
	return arithmeticTree;
}

TreeAssignments * SimpleTreeAssignmentsSemanticAction(TreeAssignment * treeAssignment, TreeAssignType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TreeAssignments * singleTrees = calloc(1, sizeof(TreeAssignments));
	
	singleTrees->singleTreeAssignment = treeAssignment;
	singleTrees->type = type;
	
	return singleTrees;
}

TreeAssignments * MultipleTreeAssignmentsSemanticAction(TreeAssignments * treeAssignments, TreeAssignment * treeAssignment, TreeAssignType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TreeAssignments * multipleTrees = calloc(1, sizeof(TreeAssignments));
	
	multipleTrees->treeAssignments = treeAssignments;
	multipleTrees->multipleTreeAssignment = treeAssignment;
	multipleTrees->type = type;
	
	return multipleTrees;
}

TreeExpression * TreeExpressionSemanticAction(Id id, TreeAssignments * treeAssignments, TreeType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TreeExpression * treeExpression = calloc(1, sizeof(TreeExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	treeExpression->treeAssignments = treeAssignments;
	treeExpression->id = aux;
	treeExpression->type = type;
	
	return treeExpression;	
}

TreeExpression * DefaultTreeExpressionSemanticAction(Id id, TreeType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TreeExpression * defaultTree = calloc(1, sizeof(TreeExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	defaultTree->id = aux;
	defaultTree->type = type;
	
	return defaultTree;
}

GrowExpression * GrowExpressionSemanticAction(Id id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GrowExpression * growExpression = calloc(1, sizeof(GrowExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	growExpression->id = aux;
	
	return growExpression;
}

ForExpression * ForExpressionSemanticAction(Id id, int start, int end, MainExpressions * mainExpressions, ForType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForExpression * forExpression = calloc(1, sizeof(ForExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;

	_INTEGER * auxStart = calloc(1, sizeof(_INTEGER));
	auxStart->value = start;

	_INTEGER * auxEnd = calloc(1, sizeof(_INTEGER));
	auxEnd->value = end;
	
	forExpression->id = aux;
	forExpression->rangeStart = auxStart;
	forExpression->rangeEnd = auxEnd;
	forExpression->mainExpressions = mainExpressions;
	forExpression->type = type;
	
	return forExpression;
}

ForExpression * ForNoRangeExpressionSemanticAction(Id id, Id forestId, MainExpressions * mainExpressions, ForType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForExpression * noRangeFor = calloc(1, sizeof(ForExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;

	_ID * auxForest = calloc(1, sizeof(_ID));
	auxForest->idValue = forestId;
	
	noRangeFor->id = aux;
	noRangeFor->forestId = auxForest;
	noRangeFor->mainExpressions = mainExpressions;
	noRangeFor->type = type;
	
	return noRangeFor;
}

ArithmeticAssignation * ArithmeticDeclarationAssignationSemanticAction(Id id, OperatorType operator, DeclarationValue * value, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticAssignation * arithDeclare = calloc(1, sizeof(ArithmeticAssignation));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	arithDeclare->id = aux;
	arithDeclare->operator = operator;
	arithDeclare->value = value;
	arithDeclare->type = type;	
	
	return arithDeclare;
}

ArithmeticAssignation * ArithmeticOperationAssignationSemanticAction(Id id, OperatorType operator, ArithmeticOperation * arithOp, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticAssignation * arithAssign = calloc(1, sizeof(ArithmeticAssignation));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	arithAssign->id = aux;
	arithAssign->operator = operator;
	arithAssign->arithmeticOperation = arithOp;
	arithAssign->type = type;
	
	return arithAssign;
}

ArithmeticAssignation * ArithmeticAttributeDeclarationAssignationSemanticAction(AttributeValue * attributeValue, OperatorType operator, DeclarationValue * value, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticAssignation * arithmeticAssignation = calloc(1, sizeof(ArithmeticAssignation));
	arithmeticAssignation->att = attributeValue;
	arithmeticAssignation->operator = operator;
	arithmeticAssignation->value = value;
	arithmeticAssignation->type = type;
	return arithmeticAssignation;
}

ArithmeticAssignation * ArithmeticAttributeOperationAssignationSemanticAction(AttributeValue * attributeValue, OperatorType operator, ArithmeticOperation * arithOp, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticAssignation * arithmeticAssignation = calloc(1, sizeof(ArithmeticAssignation));
	arithmeticAssignation->att = attributeValue;
	arithmeticAssignation->operator = operator;
	arithmeticAssignation->arithmeticOperation = arithOp;
	arithmeticAssignation->type = type;
	return arithmeticAssignation;
}

GeneralAssignation * GeneralSingleArithmeticOperationAssignationSemanticAction(Id id, ArithmeticOperation * arithOp, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GeneralAssignation * singleArith = calloc(1, sizeof(GeneralAssignation));

	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;

	singleArith->id = aux;
	singleArith->arithmeticOperation = arithOp;
	singleArith->type = type;

	return singleArith;
}

GeneralAssignation * GeneralSingleDeclarationAssignationSemanticAction(Id id, DeclarationValue * value, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GeneralAssignation * singleDeclare = calloc(1, sizeof(GeneralAssignation));

	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;

	singleDeclare->id = aux;
	singleDeclare->value = value;
	singleDeclare->type = type;

	return singleDeclare;
}

GeneralAssignation * GeneralDeclarationAssignationSemanticAction(type * classType, Id id, DeclarationValue * value, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GeneralAssignation * generalDeclare = calloc(1, sizeof(GeneralAssignation));

	_ID * aux2 = calloc(1, sizeof(_ID));
	aux2->idValue = id;

	generalDeclare->classType = classType;
	generalDeclare->idDeclared = aux2;
	generalDeclare->value = value;
	generalDeclare->type = type;

	return generalDeclare;
}

GeneralAssignation * GeneralArithmeticOperationAssignationSemanticAction(type * classType, Id id, ArithmeticOperation * arithOp, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GeneralAssignation * generalArithmetic = calloc(1, sizeof(GeneralAssignation));

	_ID * aux2 = calloc(1, sizeof(_ID));
	aux2->idValue = id;

	generalArithmetic->classType = classType;
	generalArithmetic->idDeclared = aux2;
	generalArithmetic->arithmeticOperation = arithOp;
	generalArithmetic->type = type;

	return generalArithmetic;
}

GeneralAssignation * GeneralAttributeDeclarationAssignationSemanticAction(AttributeValue * attributeValue, DeclarationValue * value, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GeneralAssignation * generalAssignation = calloc(1, sizeof(GeneralAssignation));
	generalAssignation->att = attributeValue;
	generalAssignation->value = value;
	generalAssignation->type = type;
	return generalAssignation;
}

GeneralAssignation * GeneralAttributeArithmeticOperationAssignationSemanticAction(AttributeValue * attributeValue, ArithmeticOperation * arithOp, AssignationType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GeneralAssignation * generalAssignation = calloc(1, sizeof(GeneralAssignation));
	generalAssignation->att = attributeValue;
	generalAssignation->arithmeticOperation = arithOp;
	generalAssignation->type = type;
	return generalAssignation;
}

ConditionalExpression * IfOnlyConditionalExpression(ConditionalClause * conditionalClause, MainExpressions * mainExpressions, ConditionalType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConditionalExpression * conditionalExpression = calloc(1, sizeof(ConditionalExpression));
	conditionalExpression->conditionalClause = conditionalClause;
	conditionalExpression->ifMainExpressions = mainExpressions;
	conditionalExpression->type = type;
	return conditionalExpression;
}

ConditionalExpression * IfElseConditionalExpression(ConditionalClause * conditionalClause, MainExpressions * ifMainExpressions, MainExpressions * elseMainExpressions, ConditionalType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConditionalExpression * conditionalExpression = calloc(1, sizeof(ConditionalExpression));
	conditionalExpression->conditionalClause = conditionalClause;
	conditionalExpression->ifMainExpressions = ifMainExpressions;
	conditionalExpression->elseMainExpressions = elseMainExpressions;
	conditionalExpression->type = type;
	return conditionalExpression;
}

ConditionalClause * AllDeclarationConditionalClauseSemanticAction(DeclarationValue * leftValue, DeclarationValue * rightValue, ComparissonType comparissonType, ConditionalClauseType conditionalType){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConditionalClause * conditionalClause = calloc(1, sizeof(ConditionalClause));
	conditionalClause->leftValue = leftValue;
	conditionalClause->rightValue = rightValue;
	conditionalClause->comparissonType = comparissonType;
	conditionalClause->conditionalType = conditionalType;
	return conditionalClause;
}

ConditionalClause * AllConditionalConditionalClauseSemanticAction(ConditionalClause * leftConditional, ConditionalClause * rightConditional, ComparissonType comparissonType, ConditionalClauseType conditionalType){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConditionalClause * conditionalClause = calloc(1, sizeof(ConditionalClause));
	conditionalClause->leftConditional = leftConditional;
	conditionalClause->rightConditional = rightConditional;
	conditionalClause->comparissonType = comparissonType;
	conditionalClause->conditionalType = conditionalType;
	return conditionalClause;
}

ConditionalClause * LeftDeclarationRightConditionalConditionalClauseSemanticAction(DeclarationValue * leftValue, ConditionalClause * rightConditional, ComparissonType comparissonType, ConditionalClauseType conditionalType){
	_logSyntacticAnalyzerAction(__FUNCTION__);	
	ConditionalClause * leftDeclarationClause = calloc(1, sizeof(ConditionalClause));
	
	leftDeclarationClause->rightConditionalClause = rightConditional;
	leftDeclarationClause->leftValueDeclare = leftValue;
	leftDeclarationClause->comparissonType = comparissonType;
	leftDeclarationClause->conditionalType = conditionalType;

	return leftDeclarationClause;
}

ConditionalClause * LeftConditionalRightDeclarationConditionalClauseSemanticAction(ConditionalClause * leftConditional, DeclarationValue * rightValue, ComparissonType comparissonType, ConditionalClauseType conditionalType){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConditionalClause * leftConditionalClause = calloc(1, sizeof(ConditionalClause));
	
	leftConditionalClause->leftConditionalClause = leftConditional;
	leftConditionalClause->rightValueDeclare = rightValue;
	leftConditionalClause->comparissonType = comparissonType;
	leftConditionalClause->conditionalType = conditionalType;

	return leftConditionalClause;
}

ConditionalClause * ConditionalInceptionConditionalClauseSemanticAction(ConditionalClause * conditional, ComparissonType comparissonType, ConditionalClauseType conditionalType){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConditionalClause * inceptionClause = calloc(1, sizeof(ConditionalClause));
	
	inceptionClause->conditionalClause = conditional;
	inceptionClause->comparissonType = comparissonType;
	inceptionClause->conditionalType = conditionalType;

	return inceptionClause;
}

MainExpressions * SimpleMainExpressionSemanticAction(MainExpression * mainExpression, ExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpressions * simpleMainExpression = calloc(1, sizeof(MainExpressions));

	simpleMainExpression->singleMainExpression = mainExpression;
	simpleMainExpression->type = type;

	return simpleMainExpression;
}

MainExpressions * MultipleMainExpressionSemanticAction(MainExpressions* mainExpressions, MainExpression * mainExpression, ExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpressions * multiMainExpression = calloc(1, sizeof(MainExpressions));

	multiMainExpression->mainExpressions = mainExpressions;
	multiMainExpression->multipleMainExpression = mainExpression;
	multiMainExpression->type = type;

	return multiMainExpression;
}

MainExpression * MainExpressionTreeSemanticAction(TreeExpression * treeExpression, MainExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->treeExpression = treeExpression;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionForestSemanticAction(ForestExpression * forestExpression, MainExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->forestExpression = forestExpression;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionGrowSemanticAction(GrowExpression * growExpression, MainExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->growExpression = growExpression;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionForSemanticAction(ForExpression * forExpression, MainExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->forExpression = forExpression;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionArithmeticAssignationSemanticAction(ArithmeticAssignation * arithmeticAssignation, MainExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->arithmeticAssignation = arithmeticAssignation;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionGeneralSemanticAction(GeneralAssignation * generalAssignation, MainExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->generalAssignation = generalAssignation;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionConditionalSemanticAction(ConditionalExpression * conditionalExpression, MainExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->conditionalExpression = conditionalExpression;
	mainExpression->type = type;

	return mainExpression;
}