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
static boolean _generateConditionalClause(ConditionalClause * conditionalClause);
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

static boolean _generateConditionalClause(ConditionalClause * conditionalClause){
	//TODOcheck types maybe? idk MENTIRA FALTA AUN EVALUAR POR COMPARISSONtyPE tMB AAAAAAAAAAAAAAAA
	if(conditionalClause->type == PARENTHESIS_c){
		return _generateConditionalClause(conditionalClause->conditionalClause);
	}
	else if(conditionalClause->type == V_V){
		return (_generateConditionalClause(conditionalClause->leftConditional) && _generateConditionalClause(conditionalClause->rightConditional));
	}
	else if(conditionalClause->type == V_C){
		return (_generateConditionalClause(conditionalClause->leftConditional) && _generateConditionalClause(conditionalClause->rightConditional));
	}
	else if(conditionalClause->type == C_V){
		return (_generateConditionalClause(conditionalClause->leftConditional) && _generateConditionalClause(conditionalClause->rightConditional));
	}
	else if(conditionalClause->type == C_C){
		return (_generateConditionalClause(conditionalClause->leftConditional) && _generateConditionalClause(conditionalClause->rightConditional));
	}
	else{
		logError(_logger, "Unknown ConditionalClauseType: %d\n", conditionalClause->type);
		return false;
	}
}

static void _generateConditionalExpression(ConditionalExpression * conditionalExpression){
	if(conditionalExpression->type == IF_c){
		if(_generateConditionalClause(conditionalExpression->conditionalClause)){
			_generateMainExpressions(conditionalExpression->ifMainExpressions);
		}
	}
	else if(conditionalExpression->type == ELSE_c){
		if(_generateConditionalClause(conditionalExpression->conditionalClause)){
			_generateMainExpressions(conditionalExpression->ifMainExpressions);
		}
		else _generateMainExpressions(conditionalExpression->elseMainExpressions);
	}
	else{
		logError(_logger, "Unknown ConditionalType: %d\n", conditionalExpression->type);
	}
}

static void _generateGeneralAssignation(GeneralAssignation * generalAssignation){
	//TODO mismo de siempre, checkear existencia(o no) si es necesario, checkar dataType y hacer el update  en tabla
	if(generalAssignation->type == ID_BY_VALUE_TYPE){

	}
	else if(generalAssignation->type == ID_BY_OPP_TYPE){

	}
	else if(generalAssignation->type == ID_BY_VALUE){

	}
	else if(generalAssignation->type == ID_BY_OPP){

	}
	else if(generalAssignation->type == ATT_BY_VALUE){

	}
	else if(generalAssignation->type == ATT_BY_OPP){

	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", generalAssignation->type);
	}
}

static void _generateArithmeticAssignation(ArithmeticAssignation * arithmeticAssignation){
	//TODO todos, en cualquiera hay que chequear existencia de ser necesario, types, hacer la cuentita(+,-,etc) y luego updatear tabla
	if(arithmeticAssignation->type == ID_BY_VALUE){

	}
	else if(arithmeticAssignation->type == ID_BY_OPP){

	}
	else if(arithmeticAssignation->type == ATT_BY_VALUE){

	}
	else if(arithmeticAssignation->type == ATT_BY_OPP){

	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", arithmeticAssignation->type);
	}
}

static void _generateForExpression(ForExpression * forExpression){
	if(forExpression->type == CLASSIC_ITERATION){
		//TODO check los types del for, luego es hacer un for de toda la vida
	}
	else if(forExpression->type == FOREST_ITERATION){
		//TODO aca lo que sea que esta en el bloque se repite por cada tree en el forest, idk como hacer eso aun
	}
	else{
		logError(_logger, "Unknown ForType: %d\n", forExpression->type);
	}
}

static void _generateGrowExpression(GrowExpression * growExpression){
	//TODO check exista el ID y sea algo valido de growear (encuanto a type) si es asi se guarda en una lista a imprimir?onda en epilogue?
}

static void _generateForestAssignment(ForestAssignment * forestAssignment){
	if(forestAssignment->type == ID_BY_VALUE){
		//TODO mismo q co el de tree pero con forest
	}
	else if(forestAssignment->type == ID_BY_OPP){
		//TODO mismo q co el de tree pero con forest
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", forestAssignment->type);
	}
}

static void _generateForestAssignments(ForestAssignments * forestAssignments){
	if(forestAssignments->type == SIMPLE_fa){
		_generateForestAssignment(forestAssignments->singleForestAssignment);
	}
	else if(forestAssignments->type == MULTIPLE_fa){
		_generateForestAssignment(forestAssignments->multipleForestAssignment);
		_generateForestAssignments(forestAssignments->forestAssignments);
	}
	else{
		logError(_logger, "Unknown ForestAssignType: %d\n", forestAssignments->type);
	}
}

static void _generateForestExpression(ForestExpression * forestExpression){
	if(forestExpression->type == EMPTY_f){
		//TODO guardar forest default con ese id
	}
	else if(forestExpression->type == FULL_f){
		//TODO guardar forest default con ese id (capaz pasar por parametro el id para guardarlo con key id->att?)
		_generateForestAssignments(forestExpression->forestAssignments);
	}
	else{
		logError(_logger, "Unknown ForestType: %d\n", forestExpression->type);
	}
}

static void _generateTreeAssignment(TreeAssignment * treeAssignment){
	if(treeAssignment->type == ID_BY_VALUE){
		//TODO mismo q co el de world pero con tree
	}
	else if(treeAssignment->type == ID_BY_OPP){
		//TODO mismo q co el de world pero con tree
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", treeAssignments->type);
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
		//TODO guardar tree default con ese id
	}
	else if(treeExpression->type == FULL_t){
		//TODO guardar tree default con ese id (capaz pasar por parametro el id para guardarlo con key id->att?)
		_generateTreeAssignments(treeExpression->treeAssignments);
	}
	else{
		logError(_logger, "Unknown TreeType: %d\n", treeExpression->type);
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
