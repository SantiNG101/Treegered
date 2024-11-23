#include "Generator.h"
#include "../symbol-table/table.h"

#define INTCLASS 600
#define STRCLASS 601
#define TREECLASS 610

/* WORLD DEFAULTS */
int DEFAULT_WORLD_HEIGHT =    100;
int DEFAULT_WORLD_WIDTH  =    100;
int DEFAULT_WORLD_UNEVENESS = 0;
char* DEFAULT_WORLD_MESSAGE = "";

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
static void _generateWorldExpression(WorldExpression * worldExpression);
static void _generateWorldAssignments(WorldAssignments * worldAssignments);
static void _generateWorldAssignment(WorldAssignment * worldAssignment);
static void _generateMainExpressions(MainExpressions * mainExpressions);
static void _generateMainExpression(MainExpression * mainExpression);
static void _generateGeneralAssignation(GeneralAssignation * generalAssignation);


/**
 * Creates the epilogue of the generated output.
 */
static void _generateEpilogue(void) {
	_output(file, 0, "%s",
		"            bie\n"
	);
}

static void _generateGeneralAssignation(GeneralAssignation * generalAssignation){
	//TODO complete commented ones
	if(generalAssignation->type == ID_BY_VALUE_DECLARE){//TODO asumo INT or ahora
		int res = addToTable(generalAssignation->idDeclared->idValue, INTCLASS, generalAssignation->value->intValue->value);
		if(res == ERROR_MAP) logError(_logger, "%s already exists!\n", generalAssignation->idDeclared->idValue);
		if(res != true) logError(_logger, "Error declaring %s\n", generalAssignation->idDeclared->idValue);
	}
	else if(generalAssignation->type == ID_BY_OPP_DECLARE){
		//TODO add operation resolution
	}
	else if(generalAssignation->type == ID_BY_VALUE){
		int type = getType(generalAssignation->id->idValue);
		if(type != ERROR_MAP){
			if(updateToTable(generalAssignation->id->idValue, type, generalAssignation->value->intValue->value) != true)
				logError(_logger, "Error declaring %s\n", generalAssignation->idDeclared->idValue);
		}
		else logError(_logger, "variable %s is undeclared\n", generalAssignation->idDeclared->idValue);
	}
	else if(generalAssignation->type == ID_BY_OPP){
		//
	}
	else if(generalAssignation->type == ATT_BY_VALUE){
		//
	}
	else if(generalAssignation->type == ATT_BY_OPP){
		//
	}
	else logError(_logger, "Unknown AssignationType: %d\n", generalAssignation->type);
}

static void _generateMainExpression(MainExpression * mainExpression){
	//TODO complete commented ones
	if(mainExpression->type == TREE_m){
		//_generateTreeExpression(mainExpression->treeExpression);
	}
	else if(mainExpression->type == FOREST_m){
		//_generateForestExpression(mainExpression->forestExpression);
	}
	else if(mainExpression->type == GROW_m){
		//_generateGrowExpression(mainExpression->growExpression);
	}
	else if(mainExpression->type == FOR_m){
		//_generateForExpression(mainExpression->forExpression);
	}
	else if(mainExpression->type == ARITHMETIC_m){
		//_generateArithmeticAssignation(mainExpression->arithmeticAssignation);
	}
	else if(mainExpression->type == GENERAL_ASSIGNATION_m){
		_generateGeneralAssignation(mainExpression->generalAssignation);
	}
	else if(mainExpression->type == CONDITIONAL_m){
		//_generateConditionalExpression(mainExpression->conditionalExpression);
	}
	else{
		logError(_logger, "Unknown MainExpressionType: %d\n", mainExpression->type);
		logError(_logger, "tree%d fores%d grow%d for%d arith%d gen%d cond%d\n", TREE_m, FOREST_m, GROW_m, FOR_m, ARITHMETIC_m, GENERAL_ASSIGNATION_m, CONDITIONAL_m);
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
	logError(_logger, "worldAssign\n");
	//scope WORLD
	if(worldAssignment->type == ID_BY_VALUE){
		logError(_logger, "heyo\n");
		logError(_logger, "%s\n", worldAssignment->id->idValue);
		//TODO ahora solo int
		int type = getType(worldAssignment->id->idValue);
		if(type != ERROR_MAP){
			if(updateToTable(worldAssignment->id->idValue, type, worldAssignment->value->intValue->value) != true)
				logError(_logger, "Error setting world->%s\n", worldAssignment->id->idValue);
		}
		else logError(_logger, "%s is not a world attribute\n", worldAssignment->id->idValue);
	}
	else if(worldAssignment->type == ID_BY_OPP){
		//TODO get OPP result?
		//TODO set in table
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", worldAssignment->type);
	}
}

static void _generateWorldAssignments(WorldAssignments * worldAssignments){
	if(worldAssignments->wType == SIMPLE_w){
		_generateWorldAssignment(worldAssignments->singleWorldAssignment);
	}
	else if(worldAssignments->wType == MULTIPLE_w){
		_generateWorldAssignment(worldAssignments->multipleWorldAssignment);
		_generateWorldAssignments(worldAssignments->worldAssignments);
	}
	else{
		logError(_logger, "Unknown WorldType: %d\n", worldAssignments->wType);		
	}
}

static void _generateWorldExpression(WorldExpression * worldExpression){
	_generateWorldAssignments(worldExpression->worldAssignments);
}

static void _generateProgramExpression(ProgramExpression * programExpression){
	//TODO complete commented ones
	//setup default world TODO:capaz con scope?
	addToTable("height", INTCLASS, DEFAULT_WORLD_HEIGHT);
	addToTable("width", INTCLASS, DEFAULT_WORLD_WIDTH);
	addToTable("uneveness", INTCLASS, DEFAULT_WORLD_UNEVENESS);
	addToTable("message", STRCLASS, DEFAULT_WORLD_MESSAGE);

	if(programExpression->type == WORLDLESS){
		logError(_logger, "worldless\n");
		_generateMainExpressions(programExpression->worldlessMainExpressions);
	}
	else if(programExpression->type == WORLDFULL){
		logError(_logger, "worldfull\n");
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
	_generateProgramExpression(program->programExpression);
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
	vfprintf(file, effectiveFormat, arguments);
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
	_generateEpilogue();
	logDebugging(_logger, "Generation is done.");
}
