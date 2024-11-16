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

static void _generateConditionalExpression(){

}

static void _generateGeneralAssignation(){

}

static void _generateArithmeticAssignation(){

}

static void _generateForExpression(){

}

static void _generateGrowExpression(){

}

static void _generateForestExpression(){

}

static void _generateTreeAssignment(TreeAssignment * treeAssignment){
	if(treeAssignment->type == ID_BY_VALUE){
		
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
		_addToTable(treeExpression->id, TREECLASS, NULL);//TODO como completo atributos default
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
		
	}
	else if(mainExpression->type == FOREST_m){

	}
	else if(mainExpression->type == GROW_m){

	}
	else if(mainExpression->type == FOR_m){

	}
	else if(mainExpression->type == ARITHMETIC_m){

	}
	else if(mainExpression->type == GENERAL_ASSIGNATION_m){

	}
	else if(mainExpression->type == CONDITIONAL_m){

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

	}
	else if(worldAssignment->type == ID_BY_OPP){
		
	}
	else if(worldAssignment->type == ATT_BY_VALUE){
		
	}
	else if(worldAssignment->type == ATT_BY_OPP){

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
