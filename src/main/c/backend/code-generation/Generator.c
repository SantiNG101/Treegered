#include "Generator.h"
//#include "../symbol-table/table.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
FILE * file = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
	//initTable();
	file = fopen("treegered.txt", "w");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
		fclose(file);
		//freeTable();
	}
}

/** PRIVATE FUNCTIONS */
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
static void _generateTreeExpression(TreeExpression * treeExpression);
static void _generateTreeAssignments(TreeAssignments * treeAssignments);
static void _generateTreeAssignment(TreeAssignment * treeAssignment);
static void _generateForestExpression(ForestExpression * forestExpression);
static void _generateForestAssignments(ForestAssignments * forestAssignments);
static void _generateForestAssignment(ForestAssignment * forestAssignment);
static void _generateGrowExpression(GrowExpression * growExpression);
static void _generateForExpression(ForExpression * forExpression);
static void _generateArithmeticAssignation(ArithmeticAssignation * arithmeticAssignation);
static void _generateGeneralAssignation(GeneralAssignation * generalAssignation);
static void _generateConditionalExpression(ConditionalExpression * conditionalExpression);
static void _generateConditionalClause(ConditionalClause * conditionalClause);
static void _generateArithmeticOperation(ArithmeticOperation * arithmeticOperation);
static void _generateAttributeValue(AttributeValue * attributeValue);
static void _generateDeclarationValue(DeclarationValue * declarationValue);

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(void) {
	_output(file, 0, "%s",
		"            bie\n"
	);
}

static void _generateDeclarationValue(DeclarationValue * declarationValue){
	//TODO
}

static void _generateAttributeValue(AttributeValue * attributeValue){
	//TODO
}

static void _generateArithmeticOperation(ArithmeticOperation * arithmeticOperation){
	//TODO
}

static void _generateConditionalClause(ConditionalClause * conditionalClause){
	//TODO
}

static void _generateConditionalExpression(ConditionalExpression * conditionalExpression){
	//TODO
}

static void _generateGeneralAssignation(GeneralAssignation * generalAssignation){
	//TODO
}

static void _generateArithmeticAssignation(ArithmeticAssignation * arithmeticAssignation){
	//TODO
}

static void _generateForExpression(ForExpression * forExpression){
	//TODO
}

static void _generateGrowExpression(GrowExpression * growExpression){
	//TODO
}

static void _generateForestAssignment(ForestAssignment * forestAssignment){
	//TODO
}

static void _generateForestAssignments(ForestAssignments * forestAssignments){
	//TODO
}

static void _generateForestExpression(ForestExpression * forestExpression){
	//TODO
}

static void _generateTreeAssignment(TreeAssignment * treeAssignment){
	//TODO
}

static void _generateTreeAssignments(TreeAssignments * treeAssignments){
	//TODO
}

static void _generateTreeExpression(TreeExpression * treeExpression){
	//TODO
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
		_generateGeneralAssignation(mainExpression->generalAssignation);
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
		//TODO check ID sea worldAtt, coincida con el type, sobrescribir en tabla
	}
	else if(worldAssignment->type == ID_BY_OPP){
		//TODO check ID sea worldAtt, procesar la OPP, er ue coincida con el typr, sobrescribir en tabla
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
		_generateWorldAssignment(worldAssignments->singleWorldAssignment);
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
	//_generateProgramExpression(program->programExpression);
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
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
