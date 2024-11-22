#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
FILE * file = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
	initTable();
	file = fopen("treegered.txt", "w");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
		fclose(file);
		freeTable();
	}
}

/** PRIVATE FUNCTIONS */

//TODO reemplazar
static int indentPlaceholder = 1;

static void _generateEpilogue(void);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(FILE * file, const unsigned int indentationLevel, const char * const format, ...);

static void _generateProgram(Program * program);
static void _generateProgramExpression(ProgramExpression * programExpression);

/**
 * Creates the epilogue of the generated output.
 */
static void _generateEpilogue(void) {
	_output(file, 0, "%s",
		"            bie\n"
	);
}

static void _generateConditionalClause(ConditionalClause * conditionalClause){
	//TODO o capaz este devolver el valor
	if(conditionalClause->conditionalType == PARENTHESIS_c){

	}
	else if(conditionalClause->conditionalType == V_V){
		
	}
	else if(conditionalClause->conditionalType == V_C){
		
	}
	else if(conditionalClause->conditionalType == C_V){
		
	}
	else if(conditionalClause->conditionalType == C_C){
		
	}
	else{
		logError(_logger, "Unknown ConditionalClauseType: %d\n", conditionalExpression->type);
	}
}

static void _generateConditionalExpression(ConditionalExpression * conditionalExpression){
	//TODO devolver true false? accionar?
	if(conditionalExpression->type == IF_c){
		
	}
	else if(conditionalExpression->type == ELSE_c){
		
	}
	else{
		logError(_logger, "Unknown ConditionalType: %d\n", conditionalExpression->type);
	}
}

static void _generateGeneralAssignation(GeneralAssignation * generalAssignation){
	//TODO casi lo mismo que algun assignation de los de abajo una vez funquen
}

static void _generateArithmeticOperation(ArithmeticOperation * arithmeticOperation){
	//TODO calculo q en realidad seria devolver resultados???
	if(arithmeticOperation->type == LV_RV){

	}
	else if(arithmeticOperation->type == LV_RO){

	}
	else if(arithmeticOperation->type == LO_RV){

	}
	else if(arithmeticOperation->type == LO_RO){

	}
	else if(arithmeticOperation->type == PARENTHESIS){

	}
	else{
		logError(_logger, "Unknown ArithmeticOperationType: %d\n", arithmeticAssignation->type);
	}
}

static void _generateArithmeticAssignation(ArithmeticAssignation * arithmeticAssignation){
	if(arithmeticAssignation->type == ID_BY_VALUE){
		//TODO idk algo con la table?
	}
	else if(arithmeticAssignation->type == ID_BY_OPP){

	}
	else if(arithmeticAssignation->type == ATT_BY_VALUE){

	}
	else if(arithmeticAssignation->type == ATT_BY_OPP){

	}
	else{
		logError(_logger, "Unknown AssignType: %d\n", arithmeticAssignation->type);
	}
}

static void _generateForExpression(ForExpression * forExpression){
	if(forExpression->type == CLASSIC_ITERATION){
		for(int i = forExpression->rangeStart; i <= forExpression->rangeEnd; i++){//TODO asumimos siempre ascendente??
			_generateMainExpressions(mainExpressions);
		}
	}
	else if(forExpression->type == FOREST_ITERATION){
		//TODO idk aca
	}
	else{
		logError(_logger, "Unknown ForType: %d\n", forExpression->type);
	}
}

static void _generateGrowExpression(GrowExpression * growExpression){
	if(growExpression->id!=NULL){
		//TODO aca va el actual print al file y qcyo
	}
	else{
		logError(_logger, "Nothing was given to grow");
	}
}

static void _generateForestExpression(ForestExpression * forestExpression){
	//TODO va a ser medio copia y pega de tree
}

static void _generateTreeAssignment(TreeAssignment * treeAssignment){
	if(treeAssignment->type == ID_BY_VALUE){
		//TODO seria algo similar al world assign
	}
	else if(treeAssignment->type == ID_BY_OPP){

	}
	else if(treeAssignment->type == ATT_BY_VALUE){

	}
	else if(treeAssignment->type == ATT_BY_OPP){

	}
	else{
		logError(_logger, "Unknown AssignType: %d\n", treeAssignment->type);
	}
}

static void _generateTreeAssignments(TreeAssignments * treeAssignments){
	if(treeAssignments->type == SIMPLE_ta){
		_generateTreeAssignment(treeAssignments->singleTreeAssignment);
	}
	else if(treeAssignments->type == MULTIPLE_ta){
		_generateTreeAssignment(treeAssignments->multipleTreeAssignment);
		_generateTreeAssignments(treeAssignments->treeAssignments);
	}
	else{
		logError(_logger, "Unknown TreeAssignType: %d\n", treeAssignments->type);
	}
}

static void _generateTreeExpression(TreeExpression * treeExpression){
	if(treeExpression->type == EMPTY_t){
		//TODO como completo atributos default
	}
	else if(treeExpression->type == FULL_t){
		_generateTreeAssignments(treeExpression->treeAssignments);
	}
	else{
		logError(_logger, "Unknown TreeExpressionType: %d\n", treeExpression->type);
	}
}

static void _generateMainExpression(MainExpression * mainExpression){
	if(mainExpression->type == TREE_m){
		_generateTreeExpression(mainExpression->treeExpression);
	}
	else if(mainExpression->type == FOREST_m){
		_generateForestExpression(mainExpression->forestExpression);
	}
	else if(mainExpression->type == GROW_m){
		_generateGrowExpression(mainExpression->growExpression);
	}
	else if(mainExpression->type == FOR_m){
		_generateForExpression(mainExpression->forExpression);
	}
	else if(mainExpression->type == ARITHMETIC_m){
		_generateArithmeticAssignation(mainExpression->arithmeticAssignation);
	}
	else if(mainExpression->type == GENERAL_ASSIGNATION_m){
		_generateGeneralExpression(mainExpression->generalExpression);
	}
	else if(mainExpression->type == CONDITIONAL_m){
		_generateConditionalExpression(mainExpression->conditionalExpression);
	}
	else{
		logError(_logger, "Unknown MainExpressionType: %d\n", mainExpression->type);
	}
}

static void _generateMainExpressions(MainExpressions * mainExpressions){
	if(mainExpressions->type == SIMPLE_e){
		_generateMainExpression(mainExpressions->singleMainExpression);
	}
	else if(mainExpressions->type == MULTIPLE_e){
		_generateMainExpression(mainExpressions->multipleMainExpression);
		_generateMainExpressions(mainExpressions->mainExpressions);
	}
	else{
		logError(_logger, "Unknown ExpressionType: %d\n", mainExpressions->type);		
	}
}

static void _generateWorldAssignment(WorldAssignment * worldAssignment){
	if(worldAssignment->type == ID_BY_VALUE){
		//TODO checkear esto me hace ruido idk
		if(getType(worldAssignment->id) == worldAssignment->value->type){
			switch(getType(worldAssignment->id){
				case(IDvalue):
					_addToTable(worldAssignment->id, getType(worldAssignment->id), worldAssignment->value->Value);
					break;
				case(STRINGvalue):
					_addToTable(worldAssignment->id, getType(worldAssignment->id), worldAssignment->value->Value);
					break;
				case(BOOLEANvalue):
					_addToTable(worldAssignment->id, getType(worldAssignment->id), worldAssignment->value->Value);
					break;
				case(HEXCOLORvalue):
					_addToTable(worldAssignment->id, getType(worldAssignment->id), worldAssignment->value->Value);
					break;
				case(INTEGERvalue):
					_addToTable(worldAssignment->id, getType(worldAssignment->id), worldAssignment->value->Value);
					break;
				//TODO attValue y declValue???
			}
		}
		else{
			logError(_logger, "Incorrect assignation type for %s. %d was expected", worldAssignment->id, getType(worldAssignment->id));
		}
	}
	else if(worldAssignment->type == ID_BY_OPP){
		//TODO get OPP result?
		//TODO set in table
	}
	else if(worldAssignment->type == ATT_BY_VALUE){
		//TODO set in table, ademas como chequeas que sea un attributo valido del world?
	}
	else if(worldAssignment->type == ATT_BY_OPP){
		//TODO get OPP result?
		//TODO set in table
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", worldAssignment->type);
	}
}

static void _generateWorldAssignments(WorldAssignments * worldAssignments){
	if(worldAssignments->type == SIMPLE_w){
		_generateWorldAssignment(worldAssignments->singleWorldAssignment);
	}
	else if(worldAssignments->type == MULTIPLE_w){
		_generateWorldAssignment(worldAssignments->multipleWorldAssignment);
		_generateWorldAssignments(worldAssignments->worldAssignments);
	}
	else{
		logError(_logger, "Unknown WorldType: %d\n", worldAssignments->type);		
	}
}

static void _generateWorldExpression(WorldExpression * worldExpression){
	_generateWorldAssignments(indentationLevel, worldExpression->worldAssignments);
}

static void _generateProgramExpression(ProgramExpression * programExpression){
	if(programExpression->type == WORLDLESS){
		_generateMainExpressions(programExpression->worldlessMainExpressions);
	}
	else if(programExpression->type == WORLDFULL){
		_generateWorldExpression(programExpression->worldExpression);
		_generateMainExpressions(programExpression->mainExpressions);
	}
	else{
		logError(_logger, "Unknown ProgramType: %d\n", programExpression->type);
	}
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
	_generateProgramExpression(program->_programExpression);
}

/**
 * Creates the prologue of the generated output.
 */
static void _generatePrologue(void) {
	_output(file, 0, "%s",
		"prologue heyo\n"
	);
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(FILE * file, const unsigned int indentationLevel, const char * const format, ...) {
	if (file == NULL) {
        perror("Error opening file");
        return;
	}
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
}
