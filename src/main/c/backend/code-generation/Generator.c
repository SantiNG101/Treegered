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

static void _generateGeneralAssignation(GeneralAssignation * generalAssignation){
	//TODO complete commented ones
	if(generalAssignation->type == ID_BY_VALUE_DECLARE){
		//int res = addToTable(generalAssignation->idDeclared->idValue, generalAssignation->classType->idValue, generalAssignation->value);
		//if(if == ERROR_MAP) logError(_logger, "%s already exists!\n", generalAssignation->idDeclared->idValue);
		//if(if != TRUE) logError(_logger, "Error declaring %s\n", generalAssignation->idDeclared->idValue);
	}
	else if(generalAssignation->type == ID_BY_OPP_DECLARE){
		//TODO add operation resolution
	}
	if(generalAssignation->type == ID_BY_VALUE){
		int type = getType(generalAssignation->id->idValue);
		if(type != ERROR_MAP){
			switch (generalAssignation->value->type){
				case (IDvalue):
					//_ID value = (generalAssignation->value)->idValue;
					break;
				case (STRINGvalue):
					//_STRING value = generalAssignation->value->charValue;
					break;
				case (BOOLEANvalue):
					//_BOOLEAN value = generalAssignation->value->booleanValue;
					break;
				case (HEXCOLORvalue):
					//_HEXCOLOR value = generalAssignation->value->hexcolorValue;
					break;
				case (INTEGERvalue):
					//_INTEGER value = generalAssignation->value->intValue;
					break;
			}
			if(updateToTable(generalAssignation->id->idValue, type, generalAssignation->value->idValue->idValue) != true)
				logError(_logger, "Error declaring %s\n", generalAssignation->idDeclared->idValue);
		}
		else logError(_logger, "%s is undeclared\n", generalAssignation->idDeclared->idValue);
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
	else{
		logError(_logger, "Unknown AssignationType: %d\n", generalAssignation->type);
	}
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

static void _generateProgramExpression(ProgramExpression * programExpression){
	//TODO complete commented ones
	if(programExpression->type == WORLDLESS){
		_generateMainExpressions(programExpression->worldlessMainExpressions);
	}
	else if(programExpression->type == WORLDFULL){
		//_generateWorldExpression(programExpression->worldExpression);
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
