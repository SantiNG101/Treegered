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

DeclarationValue * DeclarationValueIDSemanticAction(Id id){
	DeclarationValue * declarationValueID = calloc(1, sizeof(DeclarationValue));
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	declarationValueID->idValue = aux;
	return declarationValueID;
}

DeclarationValue * DeclarationValueStringSemanticAction(char* string){
	DeclarationValue * declarationValueString = calloc(1, sizeof(DeclarationValue));
	_STRING * aux = calloc(1, sizeof(_STRING));
	aux->value = string;
	declarationValueString->charValue = aux;
	return declarationValueString;
}
DeclarationValue * DeclarationValueBooleanSemanticAction(boolean value){
	DeclarationValue * declarationValueBoolean = calloc(1, sizeof(DeclarationValue));
	_BOOLEAN * aux = calloc(1, sizeof(_BOOLEAN));
	aux->value = value;
	declarationValueBoolean->booleanValue = aux;
	return declarationValueBoolean;
}
DeclarationValue * DeclarationValueHexSemanticAction(Hexcolor color){
	DeclarationValue * declarationValueHex = calloc(1, sizeof(DeclarationValue));
	_HEXCOLOR * aux = calloc(1, sizeof(_HEXCOLOR));
	aux->value = color;
	declarationValueHex->hexcolorValue = aux;
	return declarationValueHex;
}
DeclarationValue * DeclarationValueIntSemanticAction(int integer){
	DeclarationValue * declarationValueInteger = calloc(1, sizeof(DeclarationValue));
	_INTEGER * aux = calloc(1, sizeof(_INTEGER));
	aux->value = integer;
	declarationValueInteger->intValue = aux;
	return declarationValueInteger;
}

ArithmeticOperation * ArithmeticOperationAllDeclarationValueSemanticAction(OperatorType operator, DeclarationValue* left, DeclarationValue* right, ArithmeticOperationType type){
	ArithmeticOperation * arithmeticOperation = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperation->operator = operator;
	arithmeticOperation->leftValue = left;
	arithmeticOperation->rightValue = right;
	arithmeticOperation->type = type;
	return arithmeticOperation;
}
ArithmeticOperation * ArithmeticOperatioAllnSemanticAction(OperatorType operator, ArithmeticOperation* left, ArithmeticOperation* right, ArithmeticOperationType type){
	ArithmeticOperation * arithmeticOperation = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperation->operator = operator;
	arithmeticOperation->leftOperation = left;
	arithmeticOperation->rightOperation = right;
	arithmeticOperation->type = type;
	return arithmeticOperation;
}
ArithmeticOperation * ArithmeticOperationLeftDeclarationRightSemanticAction(OperatorType operator, DeclarationValue* left, ArithmeticOperation* right, ArithmeticOperationType type){
	ArithmeticOperation * arithmeticOperation = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperation->operator = operator;
	arithmeticOperation->leftValue = left;
	arithmeticOperation->rightOperation = right;
	arithmeticOperation->type = type;
	return arithmeticOperation;
}
ArithmeticOperation * ArithmeticOperationRightDeclarationLeftSemanticAction(OperatorType operator, ArithmeticOperation* left, DeclarationValue* right, ArithmeticOperationType type){
	ArithmeticOperation * arithmeticOperation = calloc(1, sizeof(ArithmeticOperation));
	arithmeticOperation->operator = operator;
	arithmeticOperation->leftOperation = left;
	arithmeticOperation->rightValue = right;
	arithmeticOperation->type = type;
	return arithmeticOperation;
}

ProgramExpression * WorldProgramExpressionSemanticAction(MainExpression * mainExpression, ProgramType type){
	ProgramExpression * programExpression = calloc(1, sizeof(ProgramExpression));
	programExpression->worldlessMainExpression = mainExpression;
	programExpression->type = type;
	return programExpression;
}
ProgramExpression * WorldlessProgramExpressionSemanticAction(MainExpression * mainExpression, WorldExpression* worldExpression, ProgramType type){
	ProgramExpression * programExpression = calloc(1, sizeof(ProgramExpression));
	programExpression->worldExpression = worldExpression;
	programExpression->mainExpression = mainExpression;
	return programExpression;
}

WorldAssignment * WorldAssignmentDeclarationSemanticAction(Id id, DeclarationValue* value, AssignationType type){
	WorldAssignment * worldAssignment = calloc(1, sizeof(WorldAssignment));
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	worldAssignment->id = aux;
	worldAssignment->value = value;
	worldAssignment->type = type;
	return worldAssignment;
}
WorldAssignment * WorldAssignmentArithmeticSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type){
	WorldAssignment * worldAssignment = calloc(1, sizeof(WorldAssignment));
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	worldAssignment->id = aux;
	worldAssignment->arithmeticOperation = operation;
	worldAssignment->type = type;
	return worldAssignment;
}

WorldAssignments * SimpleWorldAssignmentsSemanticAction(WorldAssignment * worldAssignment, WorldType wType){
	
	WorldAssignments * simpleWorld = calloc(1, sizeof(WorldAssignments));
	
	simpleWorld->singleWorldAssignment = worldAssignment;
	simpleWorld->wType = wType;
	
	return simpleWorld;
}

WorldAssignments * MultipleWorldAssignmentsSemanticAction(WorldAssignments * worldAssignments, WorldAssignment * worldAssignment, WorldType wType){
	
	WorldAssignments * multiWorld = calloc(1, sizeof(WorldAssignments));
	
	multiWorld->multipleWorldAssignment = worldAssignment;
	multiWorld->worldAssignments = worldAssignments;
	multiWorld->wType = wType;
	
	return multiWorld;
}

WorldExpression * WorldExpressionSemanticAction(WorldAssignments* worldAssignments){
	
	WorldExpression * worldExp = calloc(1, sizeof(WorldExpression));
	
	worldExp->worldAssignments = worldAssignments;
	
	return worldExp;
}

ForestAssignment * DeclarationForestAssignmentSemanticAction(Id id, DeclarationValue* value, AssignationType type){
	
	ForestAssignment * declareForest = calloc(1, sizeof(ForestAssignment));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	declareForest->id = aux;
	declareForest->value = value;
	declareForest->type = type;
	
	return declareForest;
}
ForestAssignment * ArithmeticForestAssignmentSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type){
	
	ForestAssignment * arithForest = calloc(1, sizeof(ForestAssignment));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	arithForest->id = aux;
	arithForest->arithmeticOperation = operation;
	arithForest->type = type;
	
	return arithForest;
}

ForestAssignments * SimpleForestAssignmentsSemanticAction(ForestAssignment * forestAssignment, ForestAssignType type){
	
	ForestAssignments * simpleForest = calloc(1, sizeof(ForestAssignments));
	
	simpleForest->singleForestAssignment = forestAssignment;
	simpleForest->type = type;
	
	return simpleForest;
}

ForestAssignments * MultipleForestAssignmentsSemanticAction(ForestAssignments * forestAssignments, ForestAssignment * forestAssignment, ForestAssignType type){
	
	ForestAssignments * multiForest = calloc(1, sizeof(ForestAssignments));
	
	multiForest->multipleForestAssignment = forestAssignment;
	multiForest->forestAssignments = forestAssignments;
	multiForest->type = type;
	
	return multiForest;
}

ForestExpression * ForestExpressionSemanticAction(Id id, ForestAssignments * forestAssignments, ForestType type){
	
	ForestExpression * notDefaultForest = calloc(1, sizeof(ForestExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	notDefaultForest->id = aux;
	notDefaultForest->forestAssignments = forestAssignments;
	notDefaultForest->type = type;
	
	return notDefaultForest;
}

ForestExpression * DefaultForestExpressionSemanticAction(Id id, ForestType type){
	
	ForestExpression * defaultForest = calloc(1, sizeof(ForestExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	defaultForest->id = aux;
	defaultForest->type = type;
	
	return defaultForest;
}

TreeAssignment * DeclarationTreeAssignmentSemanticAction(Id id, DeclarationValue* value, AssignationType type){
	
	TreeAssignment * declarationTree = calloc(1, sizeof(TreeAssignment));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	declarationTree->id = aux;
	declarationTree->value = value;
	declarationTree->type = type;
	
	return declarationTree;	
}

TreeAssignment * ArithmeticTreeAssignmentSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type){
	
	TreeAssignment * arithmeticTree = calloc(1, sizeof(TreeAssignment));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	arithmeticTree->id = aux;
	arithmeticTree->arithmeticOperation = operation;
	arithmeticTree->type = type;
	
	return arithmeticTree;
}

TreeAssignments * SimpleTreeAssignmentsSemanticAction(TreeAssignment * treeAssignment, TreeAssignType type){
	
	TreeAssignments * singleTrees = calloc(1, sizeof(TreeAssignments));
	
	singleTrees->singleTreeAssignment = treeAssignment;
	singleTrees->type = type;
	
	return singleTrees;
}

TreeAssignments * MultipleTreeAssignmentsSemanticAction(TreeAssignments * treeAssignments, TreeAssignment * treeAssignment, TreeAssignType type){
	
	TreeAssignments * multipleTrees = calloc(1, sizeof(TreeAssignments));
	
	multipleTrees->treeAssignments = treeAssignments;
	multipleTrees->multipleTreeAssignment = treeAssignment;
	multipleTrees->type = type;
	
	return multipleTrees;
}

TreeExpression * TreeExpressionSemanticAction(Id id, TreeAssignments * treeAssignments, TreeType type){
	
	TreeExpression * treeExpression = calloc(1, sizeof(TreeExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	treeExpression->treeAssignments = treeAssignments;
	treeExpression->id = aux;
	treeExpression->type = type;
	
	return treeExpression;	
}

TreeExpression * DefaultTreeExpressionSemanticAction(Id id, TreeType type){
	
	TreeExpression * defaultTree = calloc(1, sizeof(TreeExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	defaultTree->id = aux;
	defaultTree->type = type;
	
	return defaultTree;
}

GrowExpression * GrowExpressionSemanticAction(Id id){
	
	GrowExpression * growExpression = calloc(1, sizeof(GrowExpression));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	growExpression->id = aux;
	
	return growExpression;
}

ForExpression * ForExpressionSemanticAction(Id id, int start, int end, MainExpressions * mainExpressions){
	
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
	
	return forExpression;
}

ArithmeticAssignation * ArithmeticDeclarationAssignationSemanticAction(Id id, OperatorType operator, DeclarationValue * value, AssignationType type){
	
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
	
	ArithmeticAssignation * arithAssign = calloc(1, sizeof(ArithmeticAssignation));
	
	_ID * aux = calloc(1, sizeof(_ID));
	aux->idValue = id;
	
	arithAssign->id = aux;
	arithAssign->operator = operator;
	arithAssign->arithmeticOperation = arithOp;
	arithAssign->type = type;
	
	return arithAssign;
}

GeneralAssignation * GeneralDeclarationAssignationSemanticAction(Id classType, Id id, DeclarationValue * value, AssignationType type){

	GeneralAssignation * generalDeclare = calloc(1, sizeof(GeneralAssignation));

	_ID * aux1 = calloc(1, sizeof(_ID));
	aux1->idValue = classType;

	_ID * aux2 = calloc(1, sizeof(_ID));
	aux2->idValue = id;

	generalDeclare->classType = aux1;
	generalDeclare->id = aux2;
	generalDeclare->value = value;
	generalDeclare->type = type;

	return generalDeclare;
}

GeneralAssignation * GeneralArithmeticOperationAssignationSemanticAction(Id classType, Id id, ArithmeticOperation * arithOp, AssignationType type){
	
	GeneralAssignation * generalArithmetic = calloc(1, sizeof(GeneralAssignation));
	
	_ID * aux1 = calloc(1, sizeof(_ID));
	aux1->idValue = classType;

	_ID * aux2 = calloc(1, sizeof(_ID));
	aux2->idValue = id;

	generalArithmetic->classType = aux1;
	generalArithmetic->id = aux2;
	generalArithmetic->arithmeticOperation = arithOp;
	generalArithmetic->type = type;

	return generalArithmetic;
}

MainExpressions * SimpleMainExpressionSemanticAction(MainExpression * mainExpression, ExpressionType type){

	MainExpressions * simpleMainExpression = calloc(1, sizeof(MainExpressions));

	simpleMainExpression->singleMainExpression = mainExpression;
	simpleMainExpression->type = type;

	return simpleMainExpression;
}

MainExpressions * MultipleMainExpressionSemanticAction(MainExpressions* mainExpressions, MainExpression * mainExpression, ExpressionType type){

	MainExpressions * multiMainExpression = calloc(1, sizeof(MainExpressions));

	multiMainExpression->mainExpressions = mainExpressions;
	multiMainExpression->multipleMainExpression = mainExpression;
	multiMainExpression->type = type;

	return multiMainExpression;
}

MainExpression * MainExpressionTreeSemanticAction(TreeExpression * treeExpression, MainExpressionType type){

	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->treeExpression = treeExpression;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionForestSemanticAction(ForestExpression * forestExpression, MainExpressionType type){

	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->forestExpression = forestExpression;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionGrowSemanticAction(GrowExpression * growExpression, MainExpressionType type){

	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->growExpression = growExpression;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionForSemanticAction(ForExpression * forExpression, MainExpressionType type){

	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->forExpression = forExpression;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionArithmeticAssignationSemanticAction(ArithmeticAssignation * arithmeticAssignation, MainExpressionType type){

	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->arithmeticAssignation = arithmeticAssignation;
	mainExpression->type = type;

	return mainExpression;
}

MainExpression * MainExpressionGeneralSemanticAction(GeneralAssignation * generalAssignation, MainExpressionType type){

	MainExpression * mainExpression = calloc(1, sizeof(MainExpression));

	mainExpression->generalAssignation = generalAssignation;
	mainExpression->type = type;

	return mainExpression;
}