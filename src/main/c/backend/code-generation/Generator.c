#include "Generator.h"
//#include "../symbol-table/table.h"

/* MODULE INTERNAL STATE */
CompilationStatus * compi = NULL;
int ERROR_OCCURED = false;
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
static int _generateArithmeticOperation(ArithmeticOperation * arithmeticOperation);

static boolean _getConditionalClauseResultVV(DeclarationValue * dv1, DeclarationValue * dv2, ComparissonType ct);
static boolean _getConditionalClauseResultBool(boolean b1, boolean b2, ComparissonType ct);
static boolean _getConditionalClauseResultInt(int i1, int i2, ComparissonType ct);

static int _getArithOpResult(int v1, int v2, OperatorType ot);


/* AUX FUNCTIONS */ //TODO son placeholders, cambiar con lo de la tabla luego
boolean checkWorldAttribute(char * name, int type){
	return ((strcmp(name, "height") == 0 && (type == INTCLASS))
		|| (strcmp(name, "width") == 0 && (type == INTCLASS))
		|| (strcmp(name, "uneveness") == 0 && (type == INTCLASS))
		|| (strcmp(name, "message") == 0 && (type == STRCLASS)));
}

boolean checkTreeAttribute(char * name, int type){
	return true;//TODO
}

boolean checkForestAttribute(char * name, int type){
	return true;//TODO
}

boolean checkGrow(char * name){
	//TODO chequear q exista en la tabla, y que ademas de existir sea TREECLASS/FORESTCLASS
	return true;//placeholder
}

/**************************************** */


/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(void) {
	_output(file, 0, "%s",
		"            bie\n"
	);
}

static int _getArithOpResult(int v1, int v2, OperatorType ot){
	if(ot == ADD_o){
		return v1 + v2;
	}
	else if(ot == SUB_o){
		return v1 - v2;
	}
	else if(ot == MUL_o){
		return v1 * v2;
	}
	else if(ot == DIV_o){
		if(v2 == 0){
			logError(_logger, "ZeroDivError\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return 0;
		}
		return v1 / v2;
	}
	else{
		logError(_logger, "Unknown OperatorType: %d\n", ot);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return 0;
	}
}

static int _generateArithmeticOperation(ArithmeticOperation * arithmeticOperation){
	if(ERROR_OCCURED==true) return 0;
	if(arithmeticOperation->type == LV_RV){
		if(arithmeticOperation->leftValue->type == ATTvalue || arithmeticOperation->leftValue->type == DECLARATIONvalue){
			//TODO
			return 0;
		}
		if(arithmeticOperation->rightValue->type == ATTvalue || arithmeticOperation->rightValue->type == DECLARATIONvalue){
			//TODO
			return 0;
		}
		if(arithmeticOperation->leftValue->type != INTEGERvalue || arithmeticOperation->rightValue->type != INTEGERvalue){
			logError(_logger, "Cannot operate with type other than int\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return false;
		}
		return _getArithOpResult(arithmeticOperation->leftValue->intValue->value, arithmeticOperation->rightValue->intValue->value, arithmeticOperation->operator);
	}
	else if(arithmeticOperation->type == LV_RO){
		if(arithmeticOperation->leftValue->type == ATTvalue || arithmeticOperation->leftValue->type == DECLARATIONvalue){
			//TODO
			return 0;
		}
		if(arithmeticOperation->leftValue->type != INTEGERvalue){
			logError(_logger, "Cannot operate with type other than int\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return false;
		}
		return _getArithOpResult(arithmeticOperation->leftValue->intValue->value, _generateArithmeticOperation(arithmeticOperation->rightOperation), arithmeticOperation->operator);
	}
	else if(arithmeticOperation->type == LO_RV){
		if(arithmeticOperation->rightValue->type == ATTvalue || arithmeticOperation->rightValue->type == DECLARATIONvalue){
			//TODO
			return 0;
		}
		if(arithmeticOperation->rightValue->type != INTEGERvalue){
			logError(_logger, "Cannot operate with type other than int\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return false;
		}
		return _getArithOpResult(_generateArithmeticOperation(arithmeticOperation->leftOperation), arithmeticOperation->rightValue->intValue->value, arithmeticOperation->operator);
	}
	else if(arithmeticOperation->type == LO_RO){
		return _getArithOpResult(_generateArithmeticOperation(arithmeticOperation->leftOperation), _generateArithmeticOperation(arithmeticOperation->rightOperation), arithmeticOperation->operator);
	}
	else if(arithmeticOperation->type == PARENTHESIS){
		return _generateArithmeticOperation(arithmeticOperation->arithOp);
	}
	else{	
		logError(_logger, "Unknown ArithmeticOperationType: %d\n", arithmeticOperation->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return 0;
	}
}

static boolean _getConditionalClauseResultVV(DeclarationValue * dv1, DeclarationValue * dv2, ComparissonType ct){
	if(ERROR_OCCURED==true) return false;
	if(dv1->type != dv2->type){
		logError(_logger, "Cannot compare different types\n");
		ERROR_OCCURED = true;
		*compi=FAILED;
		return false;
	}
	else if(dv1->type == BOOLEANvalue){
		return _getConditionalClauseResultBool(dv1->booleanValue->value, dv2->booleanValue->value, ct);
	}
	else if(dv1->type == INTEGERvalue){
		return _getConditionalClauseResultInt(dv1->intValue->value, dv2->intValue->value, ct);
	}
	else{
		logError(_logger, "Unknown DeclarationValueType or invalid for conditionalComparisson: %d\n", dv1->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return false;
	}
}

static boolean _getConditionalClauseResultInt(int i1, int i2, ComparissonType ct){
	if(ERROR_OCCURED==true) return false;
	if(ct == EQUIVALENT_c){
		return i1 == i2;
	}
	else if(ct == DIFFERENT_c){
		return i1 != i2;
	}
	else if(ct == LESSERTHAN_c){
		return i1 < i2;
	}
	else if(ct == LESSER_EQUAL_c){
		return i1 <= i2;
	}
	else if(ct == GREATERTHAN_c){
		return i1 > i2;
	}
	else if(ct == GREATER_EQUAL_c){
		return i1 >= i2;
	}
	else{
		logError(_logger, "Unknown Comparissontype: %d\n", ct);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return false;
	}
}

static boolean _getConditionalClauseResultBool(boolean b1, boolean b2, ComparissonType ct){
	if(ERROR_OCCURED==true) return false;
	if(ct == EQUIVALENT_c){
		return b1 == b2;
	}
	else if(ct == DIFFERENT_c){
		return b1 != b2;
	}
	else if(ct == LESSERTHAN_c){
		return b1 < b2;
	}
	else if(ct == LESSER_EQUAL_c){
		return b1 <= b2;
	}
	else if(ct == GREATERTHAN_c){
		return b1 > b2;
	}
	else if(ct == GREATER_EQUAL_c){
		return b1 >= b2;
	}
	else{
		logError(_logger, "Unknown Comparissontype: %d\n", ct);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return false;
	}
}

static boolean _generateConditionalClause(ConditionalClause * conditionalClause){
	if(ERROR_OCCURED==true) return false;
	if(conditionalClause->conditionalType == PARENTHESIS_c){
		return _generateConditionalClause(conditionalClause->conditionalClause);
	}
	else if(conditionalClause->conditionalType == V_V){
		if(conditionalClause->leftValue->type == ATTvalue){//TODO check que exista la variabl, recuperarla de la tabla y ver q coincidaa el tipo
			if(conditionalClause->leftValue->attValue->type == WORLDatt){
				return true;//TODO
			}
			else if(conditionalClause->leftValue->attValue->type == IDatt){
				return true;//TODO
			}
			else{
				logError(_logger, "Unknown AttributeValueType: %d\n", conditionalClause->leftValue->attValue->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return false;
			}
		}
		return _getConditionalClauseResultVV(conditionalClause->leftValue, conditionalClause->rightValue, conditionalClause->comparissonType);
	}
	else if(conditionalClause->conditionalType == V_C){
		if(conditionalClause->leftValue->type == ATTvalue){//TODO check que exista la variabl, recuperarla de la tabla y ver q coincidaa el tipo
			if(conditionalClause->leftValue->attValue->type == WORLDatt){
				return true;//TODO
			}
			else if(conditionalClause->leftValue->attValue->type == IDatt){
				return true;//TODO
			}
			else{
				logError(_logger, "Unknown AttributeValueType: %d\n", conditionalClause->leftValue->attValue->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return false;
			}
		}
		if(conditionalClause->leftValueDeclare->type != BOOLEANvalue){
			logError(_logger, "Cannot compare different types\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return false;
		}
		return _getConditionalClauseResultBool(conditionalClause->leftValueDeclare->booleanValue->value, _generateConditionalClause(conditionalClause->rightConditionalClause), conditionalClause->comparissonType);
	}
	else if(conditionalClause->conditionalType == C_V){
		if(conditionalClause->rightValue->type == ATTvalue){//TODO check que exista la variabl, recuperarla de la tabla y ver q coincidaa el tipo
			if(conditionalClause->rightValue->attValue->type == WORLDatt){
				return true;//TODO
			}
			else if(conditionalClause->rightValue->attValue->type == IDatt){
				return true;//TODO
			}
			else{
				logError(_logger, "Unknown AttributeValueType: %d\n", conditionalClause->rightValue->attValue->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return false;
			}
		}
		if(conditionalClause->rightValueDeclare->type != BOOLEANvalue){
			logError(_logger, "Cannot compare different types\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return false;
		}
		return _getConditionalClauseResultBool(_generateConditionalClause(conditionalClause->leftConditionalClause), conditionalClause->rightValueDeclare->booleanValue->value, conditionalClause->comparissonType);
	}
	else if(conditionalClause->conditionalType == C_C){
		return _getConditionalClauseResultBool(_generateConditionalClause(conditionalClause->leftConditionalClause), _generateConditionalClause(conditionalClause->rightConditionalClause), conditionalClause->comparissonType);
	}
	else{
		logError(_logger, "Unknown ConditionalClauseType: %d\n", conditionalClause->conditionalType);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return false;
	}
}

static void _generateConditionalExpression(ConditionalExpression * conditionalExpression){
	if(ERROR_OCCURED==true) return;
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
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateGeneralAssignation(GeneralAssignation * generalAssignation){
	if(ERROR_OCCURED==true) return;
	//TODO mismo de siempre, checkear existencia(o no) si es necesario, checkar dataType y hacer el update  en tabla
	if(generalAssignation->type == ID_BY_VALUE_TYPE){
		if(generalAssignation->value->type == IDvalue){
			
		}
		else if(generalAssignation->value->type == INTEGERvalue){
			
		}
		else if(generalAssignation->value->type == STRINGvalue){
			
		}
		else if(generalAssignation->value->type == HEXCOLORvalue){
			
		}
		else if(generalAssignation->value->type == BOOLEANvalue){
			
		}
		else if(generalAssignation->value->type == DECLARATIONvalue){
			
		}
		else if(generalAssignation->value->type == ATTvalue){
			
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for generalAssignation: %d\n", generalAssignation->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(generalAssignation->type == ID_BY_OPP_TYPE){

	}
	else if(generalAssignation->type == ID_BY_VALUE){
		if(generalAssignation->value->type == IDvalue){
			
		}
		else if(generalAssignation->value->type == INTEGERvalue){
			
		}
		else if(generalAssignation->value->type == STRINGvalue){
			
		}
		else if(generalAssignation->value->type == HEXCOLORvalue){
			
		}
		else if(generalAssignation->value->type == BOOLEANvalue){
			
		}
		else if(generalAssignation->value->type == DECLARATIONvalue){
			
		}
		else if(generalAssignation->value->type == ATTvalue){
			
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for generalAssignation: %d\n", generalAssignation->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(generalAssignation->type == ID_BY_OPP){

	}
	else if(generalAssignation->type == ATT_BY_VALUE){
		if(generalAssignation->value->type == IDvalue){
			
		}
		else if(generalAssignation->value->type == INTEGERvalue){
			
		}
		else if(generalAssignation->value->type == STRINGvalue){
			
		}
		else if(generalAssignation->value->type == HEXCOLORvalue){
			
		}
		else if(generalAssignation->value->type == BOOLEANvalue){
			
		}
		else if(generalAssignation->value->type == DECLARATIONvalue){
			
		}
		else if(generalAssignation->value->type == ATTvalue){
			
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for generalAssignation: %d\n", generalAssignation->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(generalAssignation->type == ATT_BY_OPP){
		
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", generalAssignation->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateArithmeticAssignation(ArithmeticAssignation * arithmeticAssignation){
	if(ERROR_OCCURED==true) return;
	//TODO todos, en cualquiera hay que chequear existencia de ser necesario, types, hacer la cuentita(+,-,etc) y luego updatear tabla
	if(arithmeticAssignation->type == ID_BY_VALUE){
		if(arithmeticAssignation->value->type == IDvalue){
			
		}
		else if(arithmeticAssignation->value->type == INTEGERvalue){
			
		}
		else if(arithmeticAssignation->value->type == STRINGvalue){
			
		}
		else if(arithmeticAssignation->value->type == HEXCOLORvalue){
			
		}
		else if(arithmeticAssignation->value->type == BOOLEANvalue){
			
		}
		else if(arithmeticAssignation->value->type == DECLARATIONvalue){
			
		}
		else if(arithmeticAssignation->value->type == ATTvalue){
			
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for arithmeticAssignation: %d\n", arithmeticAssignation->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(arithmeticAssignation->type == ID_BY_OPP){
		
	}
	else if(arithmeticAssignation->type == ATT_BY_VALUE){
		if(arithmeticAssignation->value->type == IDvalue){
			
		}
		else if(arithmeticAssignation->value->type == INTEGERvalue){
			
		}
		else if(arithmeticAssignation->value->type == STRINGvalue){
			
		}
		else if(arithmeticAssignation->value->type == HEXCOLORvalue){
			
		}
		else if(arithmeticAssignation->value->type == BOOLEANvalue){
			
		}
		else if(arithmeticAssignation->value->type == DECLARATIONvalue){
			
		}
		else if(arithmeticAssignation->value->type == ATTvalue){
			
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for arithmeticAssignation: %d\n", arithmeticAssignation->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}

	}
	else if(arithmeticAssignation->type == ATT_BY_OPP){

	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", arithmeticAssignation->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateForExpression(ForExpression * forExpression){
	if(ERROR_OCCURED==true) return;
	if(forExpression->type == CLASSIC_ITERATION){
		//TODO check los types del for, luego es hacer un for de toda la vida
		//chequear que forExp->id no exista en tabla ya, setearlo en reangeStart
		for(int i = forExpression->rangeStart->value; i < forExpression->rangeEnd->value; i++){
			_output(file, 0, "insideForLoop:%d\n", i);//TODO sacar es para probar no mas
			_generateMainExpressions(forExpression->mainExpressions);
			//TODO set en la table forExpression->id = 	i+1;
		}
	}
	else if(forExpression->type == FOREST_ITERATION){
		_output(file, 0, "forForest id:%s forest:%s\n", forExpression->id->idValue, forExpression->forestId->idValue);//TODO sacar es para probar no mas
		//TODO aca lo que sea que esta en el bloque se repite por cada tree en el forest, idk como hacer eso aun
		//while(hasNextTree)ponele que sea algo asi este for, la estructura del forest debria tener lista de trees
	}
	else{
		logError(_logger, "Unknown ForType: %d\n", forExpression->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateGrowExpression(GrowExpression * growExpression){
	if(ERROR_OCCURED==true) return;
	//TODO check exista el ID y sea algo valido de growear (encuanto a type) si es asi se guarda en una lista a imprimir?onda en epilogue?
	if(checkGrow(growExpression->id->idValue)){
		_output(file, 0, "grow:%s\n", growExpression->id->idValue);//TODO sacar es para probar no mas
		//TODO print then??
	}
	else{
		logError(_logger, "Wrong parameter for grow");
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateForestAssignment(ForestAssignment * forestAssignment){
	if(ERROR_OCCURED==true) return;
	if(forestAssignment->type == ID_BY_VALUE){
		//TODO mismo q co el de world pero con forest
		if(forestAssignment->value->type == INTEGERvalue){
			if(checkForestAttribute(forestAssignment->id->idValue, INTCLASS)){
				_output(file, 0, "forestAssignment int:%s\n", forestAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO nose como acceder al nodo de world, en particular al id este, capaz se podria integrar en checkWorldAtt el update de tabla y mandar directo el value?
			}
		}
		else if(forestAssignment->value->type == STRINGvalue){
			if(checkForestAttribute(forestAssignment->id->idValue, STRCLASS)){
				_output(file, 0, "forestAssignment str:%s\n", forestAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else if(forestAssignment->value->type == HEXCOLORvalue){
			if(checkForestAttribute(forestAssignment->id->idValue, HEXCOLORCLASS)){
				_output(file, 0, "forestAssignment hex:%s\n", forestAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else if(forestAssignment->value->type == BOOLEANvalue){
			if(checkForestAttribute(forestAssignment->id->idValue, BOOLCLASS)){
				_output(file, 0, "forestAssignment bool:%s\n", forestAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else if(forestAssignment->value->type == DECLARATIONvalue){
			if(checkForestAttribute(forestAssignment->id->idValue, BOOLCLASS)){
				_output(file, 0, "forestAssignment arith:%s\n", forestAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else if(forestAssignment->value->type == ATTvalue){
			if(checkForestAttribute(forestAssignment->id->idValue, BOOLCLASS)){
				_output(file, 0, "forestAssignment att:%s\n", forestAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for forestAssignment: %d\n", forestAssignment->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(forestAssignment->type == ID_BY_OPP){//OBS: only INTCLASS att are possible then
		int op = _generateArithmeticOperation(forestAssignment->arithmeticOperation);
		if(checkForestAttribute(forestAssignment->id->idValue, INTCLASS)){
			_output(file, 0, "forestAssignment arith:%s\n", forestAssignment->id->idValue);//TODO sacar es para probar no mas
			//addToTable();//TODO lo mismo que en el caso anterior, seria con op siempre lo cual es aun mas comodo
		}
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", forestAssignment->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateForestAssignments(ForestAssignments * forestAssignments){
	if(ERROR_OCCURED==true) return;
	if(forestAssignments->type == SIMPLE_fa){
		_generateForestAssignment(forestAssignments->singleForestAssignment);
	}
	else if(forestAssignments->type == MULTIPLE_fa){
		_generateForestAssignment(forestAssignments->multipleForestAssignment);
		_generateForestAssignments(forestAssignments->forestAssignments);
	}
	else{
		logError(_logger, "Unknown ForestAssignType: %d\n", forestAssignments->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateForestExpression(ForestExpression * forestExpression){
	if(ERROR_OCCURED==true) return;
	if(forestExpression->type == EMPTY_f){
		_output(file, 0, "forestExpression att:%s\n", forestExpression->id->idValue);//TODO sacar es para probar no mas
		//TODO guardar forest default con ese id
	}
	else if(forestExpression->type == FULL_f){
		_output(file, 0, "forestExpression att:%s\n", forestExpression->id->idValue);//TODO sacar es para probar no mas
		//TODO guardar forest default con ese id (capaz pasar por parametro el id para guardarlo con key id->att?)
		_generateForestAssignments(forestExpression->forestAssignments);
	}
	else{
		logError(_logger, "Unknown ForestType: %d\n", forestExpression->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateTreeAssignment(TreeAssignment * treeAssignment){
	if(ERROR_OCCURED==true) return;
	if(treeAssignment->type == ID_BY_VALUE){
		//TODO mismo q co el de world pero con tree
		if(treeAssignment->value->type == INTEGERvalue){
			if(checkTreeAttribute(treeAssignment->id->idValue, INTCLASS)){
				_output(file, 0, "treeAssignment int:%s\n", treeAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO nose como acceder al nodo de world, en particular al id este, capaz se podria integrar en checkWorldAtt el update de tabla y mandar directo el value?
			}
		}
		else if(treeAssignment->value->type == STRINGvalue){
			if(checkTreeAttribute(treeAssignment->id->idValue, STRCLASS)){
				_output(file, 0, "treeAssignment str:%s\n", treeAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else if(treeAssignment->value->type == HEXCOLORvalue){
			if(checkTreeAttribute(treeAssignment->id->idValue, HEXCOLORCLASS)){
				_output(file, 0, "treeAssignment hex:%s\n", treeAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else if(treeAssignment->value->type == BOOLEANvalue){
			if(checkTreeAttribute(treeAssignment->id->idValue, BOOLCLASS)){
				_output(file, 0, "treeAssignment bool:%s\n", treeAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else if(treeAssignment->value->type == DECLARATIONvalue){
			if(checkTreeAttribute(treeAssignment->id->idValue, BOOLCLASS)){
				_output(file, 0, "treeAssignment arith:%s\n", treeAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else if(treeAssignment->value->type == ATTvalue){
			if(checkTreeAttribute(treeAssignment->id->idValue, BOOLCLASS)){
				_output(file, 0, "treeAssignment att:%s\n", treeAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for treeAssignment: %d\n", treeAssignment->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(treeAssignment->type == ID_BY_OPP){//OBS: only INTCLASS att are possible then
		if(strcmp(treeAssignment->id->idValue, "leaf") == 0 || strcmp(treeAssignment->id->idValue, "color") == 0 || strcmp(treeAssignment->id->idValue, "snowed") == 0){
			logError(_logger, "Tried to initialize tree->leaf or tree->color or tree->snowed with int result");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
		int op = _generateArithmeticOperation(treeAssignment->arithmeticOperation);
		if(checkTreeAttribute(treeAssignment->id->idValue, INTCLASS)){
			_output(file, 0, "treeAssignment arith:%s\n", treeAssignment->id->idValue);//TODO sacar es para probar no mas
			//addToTable();//TODO lo mismo que en el caso anterior, seria con op siempre lo cual es aun mas comodo
		}
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", treeAssignment->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

//TODO si esto termina devolviendo un struct del tipo de dato tree, hay que chequear q si algun assignment singular da error entonces da error toda la operación
static void _generateTreeAssignments(TreeAssignments * treeAssignments){
	if(ERROR_OCCURED==true) return;
	if(treeAssignments->type == SIMPLE_ta){
		_generateTreeAssignment(treeAssignments->singleTreeAssignment);
	}
	else if(treeAssignments->type == MULTIPLE_ta){
		_generateTreeAssignment(treeAssignments->multipleTreeAssignment);
		_generateTreeAssignments(treeAssignments->treeAssignments);
	}
	else{
		logError(_logger, "Unknown TreeAssignType: %d\n", treeAssignments->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateTreeExpression(TreeExpression * treeExpression){
	if(ERROR_OCCURED==true) return;
	if(treeExpression->type == EMPTY_t){
		//TODO guardar tree default con ese id
		_output(file, 0, "treeExpression:%s\n", treeExpression->id->idValue);//TODO sacar es para probar no mas
	}
	else if(treeExpression->type == FULL_t){
		//TODO guardar tree default con ese id (capaz pasar por parametro el id para guardarlo con key id->att?)
		//la funcion tendira que devolver un struct de lo que seria el tipo de dato tree entonces
		_output(file, 0, "treeExpression:%s\n", treeExpression->id->idValue);//TODO sacar es para probar no mas
		_generateTreeAssignments(treeExpression->treeAssignments);
	}
	else{
		logError(_logger, "Unknown TreeType: %d\n", treeExpression->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateMainExpression(MainExpression * mainExpression){
	if(ERROR_OCCURED==true) return;
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
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateMainExpressions(MainExpressions * mainExpressions){
	if(ERROR_OCCURED==true) return;
	if(mainExpressions->type == SIMPLE_e){
		_generateMainExpression(mainExpressions->singleMainExpression);
	}
	else if(mainExpressions->type == MULTIPLE_e){
		_generateMainExpression(mainExpressions->multipleMainExpression);
		_generateMainExpressions(mainExpressions->mainExpressions);
	}
	else{
		logError(_logger, "Unknown ExpressionType: %d\n", mainExpressions->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateWorldAssignment(WorldAssignment * worldAssignment){
	if(ERROR_OCCURED==true) return;
	if(worldAssignment->type == ID_BY_VALUE){
		if(worldAssignment->value->type == INTEGERvalue){
			if(checkWorldAttribute(worldAssignment->id->idValue, INTCLASS)){
				_output(file, 0, "worldAssignment int :%s\n", worldAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO nose como acceder al nodo de world, en particular al id este, capaz se podria integrar en checkWorldAtt el update de tabla y mandar directo el value?
			}
		}
		else if(worldAssignment->value->type == STRINGvalue){
			if(checkWorldAttribute(worldAssignment->id->idValue, STRCLASS)){
				_output(file, 0, "worldAssignment str:%s\n", worldAssignment->id->idValue);//TODO sacar es para probar no mas
				//addToTable();//TODO idem
			}
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for worldAssignment: %d\n", worldAssignment->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}		
	}
	else if(worldAssignment->type == ID_BY_OPP){//OBS: only INTCLASS att are possible then
		//TODO check ID sea worldAtt, procesar la OPP, er ue coincida con el typr, sobrescribir en tabla
		if(strcmp(worldAssignment->id->idValue, "message") == 0){
			logError(_logger, "Tried to initialize world->message with int result");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
		int op = _generateArithmeticOperation(worldAssignment->arithmeticOperation);
		if(checkWorldAttribute(worldAssignment->id->idValue, INTCLASS)){
			_output(file, 0, "worldAssignment arith:%s\n", worldAssignment->id->idValue);//TODO sacar es para probar no mas
			//addToTable();//TODO lo mismo que en el caso anterior, seria con op siempre lo cual es aun mas comodo
		}
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", worldAssignment->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateWorldAssignments(WorldAssignments * worldAssignments){
	if(ERROR_OCCURED==true) return;
	if(worldAssignments->wType == SIMPLE_w){
		_generateWorldAssignment(worldAssignments->singleWorldAssignment);
	}
	else if(worldAssignments->wType == MULTIPLE_w){
		_generateWorldAssignment(worldAssignments->multipleWorldAssignment);
		_generateWorldAssignments(worldAssignments->worldAssignments);
	}
	else{
		logError(_logger, "Unknown WorldType: %d\n", worldAssignments->wType);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateWorldExpression(WorldExpression * worldExpression){
	if(ERROR_OCCURED==true) return;
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
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
	_generateProgramExpression(program->programExpression);
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

int generate(CompilerState * compilerState, CompilationStatus * compilationStatus) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	compi = compilationStatus;
	_generateProgram(compilerState->abstractSyntaxtTree);
	if(ERROR_OCCURED == false) _generateEpilogue();
	logDebugging(_logger, "Generation is done.");
	return ERROR_OCCURED==true? 1:0;
}
