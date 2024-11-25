#include "Generator.h"

/* MODULE INTERNAL STATE */
CompilationStatus * compi = NULL;
int ERROR_OCCURED = false;
const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
FILE * file = NULL;

/* DEFAULT VALUES */
const int DEFAULT_WORLD_HEIGHT = 70;
const int DEFAULT_WORLD_WIDTH = 100;
const int DEFAULT_WORLD_UNEVENESS = 0;
char * DEFAULT_WORLD_MESSAGE = "bie ^_^";

const int DEFAULT_TREE_HEIGHT = 10;
const int DEFAULT_TREE_X = DEFAULT_WORLD_WIDTH/2;
const char DEFAULT_TREE_LEAF = '*';
const Hexcolor DEFAULT_TREE_COLOR = "#FFFFFF";
const int DEFAULT_TREE_DEPTH = 0;
const int DEFAULT_TREE_DENSITY = 0;
const int DEFAULT_TREE_BARK = 0;
const boolean DEFAULT_TREE_SNOWED = false;

const int DEFAULT_FOREST_START = 0;
const int DEFAULT_FOREST_END = DEFAULT_WORLD_WIDTH;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
	initializeTable();
	file = fopen("treegered.txt", "w");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
		fclose(file);
		destroyTable();
	}
}

/** PRIVATE FUNCTIONS */
static void _generateEpilogue(void);
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
static void _generateTreeAssignments(TreeAssignments * treeAssignments, char * treeId);
static void _generateTreeAssignment(TreeAssignment * treeAssignment, char * treeId);
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
		_output(file, 0, "sum %d + %d\n", v1, v2);
		return v1 + v2;
	}
	else if(ot == SUB_o){
		_output(file, 0, "sub %d - %d\n", v1, v2);
		return v1 - v2;
	}
	else if(ot == MUL_o){
		_output(file, 0, "mul %d * %d\n", v1, v2);
		return v1 * v2;
	}
	else if(ot == DIV_o){
		if(v2 == 0){
			logError(_logger, "ZeroDivError\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return 0;
		}
		_output(file, 0, "div %d / %d\n", v1, v2);
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
		int op = _generateArithmeticOperation(generalAssignation->arithmeticOperation);
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
	EntryType type = getType(growExpression->id->idValue);
	if(type == EMPTY_TYPE){
		logError(_logger, "Nonexistent variable: %s\n", growExpression->id->idValue);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return;
	}
	if(type == FOREST_TYPE || type == TREE_TYPE){
		//TODO logica del grow
	}
	else{
		logError(_logger, "Cannot grow variables of type: %s\n", type);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return;
	}
}

static void _generateForestAssignment(ForestAssignment * forestAssignment){
	if(ERROR_OCCURED==true) return;
	if(forestAssignment->type == ID_BY_VALUE){
		//TODO mismo q co el de world pero con forest
		if(forestAssignment->value->type == INTEGERvalue){
			//if(checkForestAttribute(forestAssignment->id->idValue, INTCLASS)){
				//addToTable();//TODO nose como acceder al nodo de world, en particular al id este, capaz se podria integrar en checkWorldAtt el update de tabla y mandar directo el value?
			//}
		}
		else if(forestAssignment->value->type == STRINGvalue){
			//if(checkForestAttribute(forestAssignment->id->idValue, STRCLASS)){
				//addToTable();//TODO idem
			//}
		}
		else if(forestAssignment->value->type == HEXCOLORvalue){
			//if(checkForestAttribute(forestAssignment->id->idValue, HEXCOLORCLASS)){
				//addToTable();//TODO idem
			//}
		}
		else if(forestAssignment->value->type == BOOLEANvalue){
			//if(checkForestAttribute(forestAssignment->id->idValue, BOOLCLASS)){
				//addToTable();//TODO idem
			//}
		}
		else if(forestAssignment->value->type == DECLARATIONvalue){
			//if(checkForestAttribute(forestAssignment->id->idValue, BOOLCLASS)){
				//addToTable();//TODO idem
			//}
		}
		else if(forestAssignment->value->type == ATTvalue){
			//if(checkForestAttribute(forestAssignment->id->idValue, BOOLCLASS)){
				//addToTable();//TODO idem
			//}
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for forestAssignment: %d\n", forestAssignment->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		return;
	}
	else if(forestAssignment->type == ID_BY_OPP){//OBS: only INTCLASS att are possible then
		int op = _generateArithmeticOperation(forestAssignment->arithmeticOperation);
		//if(checkForestAttribute(forestAssignment->id->idValue, INTCLASS)){
			//addToTable();//TODO lo mismo que en el caso anterior, seria con op siempre lo cual es aun mas comodo
		//}
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", forestAssignment->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
	return;
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

static void _generateTreeAssignment(TreeAssignment * treeAssignment, char * treeId){
	if(ERROR_OCCURED==true) return;

	_TREE * tree = getTree(treeId).value._tree;

	if(treeAssignment->type == ID_BY_VALUE){
		if(treeAssignment->value->type == INTEGERvalue){
			if(strcmp(treeAssignment->id->idValue, "x") == 0){
				tree->x = treeAssignment->value->intValue->value;
				insertTree(treeId, tree);
			}
			else if(strcmp(treeAssignment->id->idValue, "height") == 0){
				tree->height = treeAssignment->value->intValue->value;
				insertTree(treeId, tree);
			}
			else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
				tree->depth = treeAssignment->value->intValue->value;
				insertTree(treeId, tree);
			}
			else if(strcmp(treeAssignment->id->idValue, "density") == 0){
				tree->density = treeAssignment->value->intValue->value;
				insertTree(treeId, tree);
			}
			else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
				tree->bark = treeAssignment->value->intValue->value;
				insertTree(treeId, tree);
			}
			else{
				logError(_logger, "Unknown tree attribute assignment by int value by name: %s\n", treeAssignment->id->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(treeAssignment->value->type == STRINGvalue){
			if(strcmp(treeAssignment->id->idValue, "leaf") == 0 && strlen(treeAssignment->value->charValue->value) == 3){
				tree->leaf = treeAssignment->value->charValue->value[1];
				insertTree(treeId, tree);
			}
			else{
				logError(_logger, "Unknown tree attribute assignment by char value by name: %s\n", treeAssignment->id->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(treeAssignment->value->type == HEXCOLORvalue){
			if(strcmp(treeAssignment->id->idValue, "color") == 0){
				tree->color = treeAssignment->value->hexcolorValue->value;
				insertTree(treeId, tree);
			}
			else{
				logError(_logger, "Unknown tree attribute assignment by hexcolor value by name: %s\n", treeAssignment->id->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(treeAssignment->value->type == BOOLEANvalue){
			if(strcmp(treeAssignment->id->idValue, "snowed") == 0){
				tree->snowed = treeAssignment->value->booleanValue->value;
				insertTree(treeId, tree);
			}
			else{
				logError(_logger, "Unknown tree attribute assignment by boolean value by name: %s\n", treeAssignment->id->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(treeAssignment->value->type == DECLARATIONvalue){
			TreeAssignment * aux = calloc(1, sizeof(TreeAssignment));
			aux->id = treeAssignment->id;
			aux->value = treeAssignment->value->declareValue;
			_generateTreeAssignment(aux, treeId);
			free(aux);
			return;
		}
		else if(treeAssignment->value->type == ATTvalue){
			if(treeAssignment->value->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "height") == 0){
					if(strcmp(treeAssignment->id->idValue, "x") == 0){
						tree->x = world->height;
					}
					else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
						tree->depth = world->height;
					}
					else if(strcmp(treeAssignment->id->idValue, "height") == 0){
						tree->height = world->height;
					}
					else if(strcmp(treeAssignment->id->idValue, "density") == 0){
						tree->density = world->height;
					}
					else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
						tree->bark = world->height;
					}
					insertTree(treeId, tree);
				}
				else if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "width") == 0){
					if(strcmp(treeAssignment->id->idValue, "x") == 0){
						tree->x = world->width;
					}
					else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
						tree->depth = world->width;
					}
					else if(strcmp(treeAssignment->id->idValue, "height") == 0){
						tree->height = world->width;
					}
					else if(strcmp(treeAssignment->id->idValue, "density") == 0){
						tree->density = world->width;
					}
					else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
						tree->bark = world->width;
					}
					insertTree(treeId, tree);
				}
				else if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "uneveness") == 0){
					if(strcmp(treeAssignment->id->idValue, "x") == 0){
						tree->x = world->uneveness;
					}
					else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
						tree->depth = world->uneveness;
					}
					else if(strcmp(treeAssignment->id->idValue, "height") == 0){
						tree->height = world->uneveness;
					}
					else if(strcmp(treeAssignment->id->idValue, "density") == 0){
						tree->density = world->uneveness;
					}
					else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
						tree->bark = world->uneveness;
					}
					insertTree(treeId, tree);
				}
				else{
					logError(_logger, "Unkown world attribute of int type: %s\n", treeAssignment->value->attValue->attributeID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;	
			}
			else if(treeAssignment->value->attValue->type == IDatt){
				EntryType type = getType(treeAssignment->value->attValue->variableID->idValue);
				if(type == EMPTY_TYPE){
					logError(_logger, "Nonexistent variable: %s\n", treeAssignment->value->attValue->variableID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}

				if((strcmp(treeAssignment->id->idValue, "x") == 0) || (strcmp(treeAssignment->id->idValue, "depth") == 0) || (strcmp(treeAssignment->id->idValue, "density") == 0) || (strcmp(treeAssignment->id->idValue, "bark") == 0) || (strcmp(treeAssignment->id->idValue, "height") == 0)){
					if(type == TREE_TYPE){
						_TREE * tree2 = getForest(treeAssignment->value->attValue->variableID->idValue).value._tree;
						if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "x") == 0){
							if(strcmp(treeAssignment->id->idValue, "x") == 0){
								tree->x = tree2->x;
							}
							else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
								tree->depth = tree2->x;
							}
							else if(strcmp(treeAssignment->id->idValue, "height") == 0){
								tree->height = tree2->x;
							}
							else if(strcmp(treeAssignment->id->idValue, "density") == 0){
								tree->density = tree2->x;
							}
							else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
								tree->bark = tree2->x;
							}
							insertTree(treeId, tree);
						}
						else if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "height") == 0){
							if(strcmp(treeAssignment->id->idValue, "x") == 0){
								tree->x = tree2->height;
							}
							else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
								tree->depth = tree2->height;
							}
							else if(strcmp(treeAssignment->id->idValue, "height") == 0){
								tree->height = tree2->height;
							}
							else if(strcmp(treeAssignment->id->idValue, "density") == 0){
								tree->density = tree2->height;
							}
							else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
								tree->bark = tree2->height;
							}
							insertTree(treeId, tree);
						}
						else if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "depth") == 0){
							if(strcmp(treeAssignment->id->idValue, "x") == 0){
								tree->x = tree2->depth;
							}
							else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
								tree->depth = tree2->depth;
							}
							else if(strcmp(treeAssignment->id->idValue, "height") == 0){
								tree->height = tree2->depth;
							}
							else if(strcmp(treeAssignment->id->idValue, "density") == 0){
								tree->density = tree2->depth;
							}
							else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
								tree->bark = tree2->depth;
							}
							insertTree(treeId, tree);
						}
						else if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "density") == 0){
							if(strcmp(treeAssignment->id->idValue, "x") == 0){
								tree->x = tree2->density;
							}
							else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
								tree->depth = tree2->density;
							}
							else if(strcmp(treeAssignment->id->idValue, "height") == 0){
								tree->height = tree2->density;
							}
							else if(strcmp(treeAssignment->id->idValue, "density") == 0){
								tree->density = tree2->density;
							}
							else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
								tree->bark = tree2->density;
							}
							insertTree(treeId, tree);
						}
						else if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "bark") == 0){
							if(strcmp(treeAssignment->id->idValue, "x") == 0){
								tree->x = tree2->bark;
							}
							else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
								tree->depth = tree2->bark;
							}
							else if(strcmp(treeAssignment->id->idValue, "height") == 0){
								tree->height = tree2->bark;
							}
							else if(strcmp(treeAssignment->id->idValue, "density") == 0){
								tree->density = tree2->bark;
							}
							else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
								tree->bark = tree2->bark;
							}
							insertTree(treeId, tree);
						}
						else{
							logError(_logger, "Unkown tree attribute of int type: %s\n", treeAssignment->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
						}
						return;
					}
					else if(type == FOREST_TYPE){
						_FOREST * forest = getForest(treeAssignment->value->attValue->variableID->idValue).value._forest;
						if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "start") == 0){
							if(strcmp(treeAssignment->id->idValue, "x") == 0){
								tree->x = forest->start;
							}
							else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
								tree->depth = forest->start;
							}
							else if(strcmp(treeAssignment->id->idValue, "height") == 0){
								tree->height = forest->start;
							}
							else if(strcmp(treeAssignment->id->idValue, "density") == 0){
								tree->density = forest->start;
							}
							else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
								tree->bark = forest->start;
							}
							insertTree(treeId, tree);
						}
						else if(strcmp(treeAssignment->value->attValue->attributeID->idValue, "end") == 0){
							if(strcmp(treeAssignment->id->idValue, "x") == 0){
								tree->x = forest->end;
							}
							else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
								tree->depth = forest->end;
							}
							else if(strcmp(treeAssignment->id->idValue, "height") == 0){
								tree->height = forest->end;
							}
							else if(strcmp(treeAssignment->id->idValue, "density") == 0){
								tree->density = forest->end;
							}
							else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
								tree->bark = forest->end;
							}
							insertTree(treeId, tree);
						}
						else{
							logError(_logger, "Unkown forest attribute of int type: %s\n", treeAssignment->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
						}
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else{
					logError(_logger, "Unknown treeAttribute: %s\n", treeAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;
			}
			else{
				logError(_logger, "Unknown attributeValueType: %d\n", treeAssignment->value->attValue->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for treeAssignment: %d\n", treeAssignment->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		return;
	}
	else if(treeAssignment->type == ID_BY_OPP){//OBS: only INTCLASS att are possible then
		if(strcmp(treeAssignment->id->idValue, "leaf") == 0 || strcmp(treeAssignment->id->idValue, "color") == 0 || strcmp(treeAssignment->id->idValue, "snowed") == 0){
			logError(_logger, "Tried to initialize tree->leaf or tree->color or tree->snowed with int result");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}

		int op = _generateArithmeticOperation(treeAssignment->arithmeticOperation);
		if(ERROR_OCCURED==true) return;

		if(strcmp(treeAssignment->id->idValue, "x") == 0){
			tree->x = op;
			insertTree(treeId, tree);
		}
		else if(strcmp(treeAssignment->id->idValue, "height") == 0){
			tree->height = op;
			insertTree(treeId, tree);
		}
		else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
			tree->depth = op;
			insertTree(treeId, tree);
		}
		else if(strcmp(treeAssignment->id->idValue, "density") == 0){
			tree->density = op;
			insertTree(treeId, tree);
		}
		else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
			tree->bark = op;
			insertTree(treeId, tree);
		}
		else{
			logError(_logger, "Unknown tree attribute assign by operation: name(%s)\n", treeAssignment->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		return;
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", treeAssignment->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateTreeAssignments(TreeAssignments * treeAssignments, char * treeId){
	if(ERROR_OCCURED==true) return;
	if(treeAssignments->type == SIMPLE_ta){
		_generateTreeAssignment(treeAssignments->singleTreeAssignment, treeId);
	}
	else if(treeAssignments->type == MULTIPLE_ta){
		_generateTreeAssignment(treeAssignments->multipleTreeAssignment, treeId);
		_generateTreeAssignments(treeAssignments->treeAssignments, treeId);
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
		if(exists(treeExpression->id->idValue)){
			logError(_logger, "Existent variable: %s\n", treeExpression->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		else{
			_TREE * tree = calloc(1, sizeof(_TREE));
			tree->x = DEFAULT_TREE_X;
			tree->height = DEFAULT_TREE_HEIGHT;
			tree->leaf = DEFAULT_TREE_LEAF;
			tree->color = DEFAULT_TREE_COLOR;
			tree->depth = DEFAULT_TREE_DEPTH;
			tree->density = DEFAULT_TREE_DENSITY;
			tree->bark = DEFAULT_TREE_BARK;
			tree->snowed = DEFAULT_TREE_SNOWED;
			insertTree(treeExpression->id->idValue, tree);
		}
	}
	else if(treeExpression->type == FULL_t){
		if(exists(treeExpression->id->idValue)){
			logError(_logger, "Existent variable: %s\n", treeExpression->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		else{
			_TREE * tree = calloc(1, sizeof(_TREE));
			tree->x = DEFAULT_TREE_X;
			tree->height = DEFAULT_TREE_HEIGHT;
			tree->leaf = DEFAULT_TREE_LEAF;
			tree->color = DEFAULT_TREE_COLOR;
			tree->depth = DEFAULT_TREE_DEPTH;
			tree->density = DEFAULT_TREE_DENSITY;
			tree->bark = DEFAULT_TREE_BARK;
			tree->snowed = DEFAULT_TREE_SNOWED;
			insertTree(treeExpression->id->idValue, tree);
			_generateTreeAssignments(treeExpression->treeAssignments, treeExpression->id->idValue);
		}
		
	}
	else{
		logError(_logger, "Unknown TreeType: %d\n", treeExpression->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
	return;
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
	
	_WORLD * world = getWorld("world").value._world;

	if(worldAssignment->type == ID_BY_VALUE){
		if(worldAssignment->value->type == INTEGERvalue){
			if(strcmp(worldAssignment->id->idValue, "height") == 0){
				world->height = worldAssignment->value->intValue->value;
				insertWorld("world", world);
			}
			else if(strcmp(worldAssignment->id->idValue, "width") == 0){
				world->width = worldAssignment->value->intValue->value;
				insertWorld("world", world);
			}
			else if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
				world->uneveness = worldAssignment->value->intValue->value;
				insertWorld("world", world);
			}
			else{
				logError(_logger, "Unknown world attribute assignment by int value by name: %s\n", worldAssignment->id->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(worldAssignment->value->type == STRINGvalue){
			if(strcmp(worldAssignment->id->idValue, "message") == 0){
				world->message = worldAssignment->value->charValue->value;
				insertWorld("world", world);
			}
			else{
				logError(_logger, "Unknown world attribute assignment by str value by name: %s\n", worldAssignment->id->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(worldAssignment->value->type == DECLARATIONvalue){
			WorldAssignment * aux = calloc(1, sizeof(WorldAssignment));
			aux->id = worldAssignment->id;
			aux->value = worldAssignment->value->declareValue;
			_generateWorldAssignment(aux);
			free(aux);
			return;
		}
		else if(worldAssignment->value->type == ATTvalue){
			if(worldAssignment->value->attValue->type == WORLDatt){
				if(strcmp(worldAssignment->id->idValue, "height") == 0){
					if(strcmp(worldAssignment->value->attValue->attribute->idValue, "height") == 0){
						world->height = world->height;
						insertWorld("world", world);
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "width") == 0){
						world->height = world->width;
						insertWorld("world", world);
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "uneveness") == 0){
						world->height = world->uneveness;
						insertWorld("world", world);
					}
					else{
						logError(_logger, "Unknown worldAttribute to assign to world->height: %s\n", worldAssignment->value->attValue->attribute->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(strcmp(worldAssignment->id->idValue, "width") == 0){
					if(strcmp(worldAssignment->value->attValue->attribute->idValue, "height") == 0){
						world->width = world->height;
						insertWorld("world", world);
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "width") == 0){
						world->width = world->width;
						insertWorld("world", world);
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "uneveness") == 0){
						world->width = world->uneveness;
						insertWorld("world", world);
					}
					else{
						logError(_logger, "Unknown worldAttribute to assign to world->width: %s\n", worldAssignment->value->attValue->attribute->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
					if(strcmp(worldAssignment->value->attValue->attribute->idValue, "height") == 0){
						world->uneveness = world->height;
						insertWorld("world", world);
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "width") == 0){
						world->uneveness = world->width;
						insertWorld("world", world);
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "uneveness") == 0){
						world->uneveness = world->uneveness;
						insertWorld("world", world);
					}
					else{
						logError(_logger, "Unknown worldAttribute to assign to world->uneveness: %s\n", worldAssignment->value->attValue->attribute->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(strcmp(worldAssignment->id->idValue, "message") == 0){
					if(strcmp(worldAssignment->value->attValue->attribute->idValue, "message") == 0){
						world->message = world->message;
						insertWorld("world", world);
					}
					else{
						logError(_logger, "Unknown worldAttribute to assign to world->message: %s\n", worldAssignment->value->attValue->attribute->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else{
					logError(_logger, "Unknown worldAttribute: %s\n", worldAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
			else if(worldAssignment->value->attValue->type == IDatt){
				EntryType type = getType(worldAssignment->value->attValue->variableID->idValue);
				if(type == EMPTY_TYPE){
					logError(_logger, "Nonexistent variable: %s\n", worldAssignment->value->attValue->variableID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}

				if((strcmp(worldAssignment->id->idValue, "uneveness") == 0) || (strcmp(worldAssignment->id->idValue, "width") == 0) || (strcmp(worldAssignment->id->idValue, "height") == 0)){
					if(type == TREE_TYPE){
						_TREE * tree = getForest(worldAssignment->value->attValue->variableID->idValue).value._tree;
						if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "x") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = tree->x;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = tree->x;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = tree->x;
							}
							insertWorld("world", world);
						}
						else if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "height") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = tree->height;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = tree->height;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = tree->height;
							}
							insertWorld("world", world);
						}
						else if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "depth") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = tree->depth;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = tree->depth;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = tree->depth;
							}
							insertWorld("world", world);
						}
						else if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "density") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = tree->density;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = tree->density;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = tree->density;
							}
							insertWorld("world", world);
						}
						else if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "bark") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = tree->bark;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = tree->bark;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = tree->bark;
							}
							insertWorld("world", world);
						}
						else{
							logError(_logger, "Unkown tree attribute of int type: %s\n", worldAssignment->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
						}
						return;
					}
					else if(type == FOREST_TYPE){
						_FOREST * forest = getForest(worldAssignment->value->attValue->variableID->idValue).value._forest;
						if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "start") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = forest->start;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = forest->start;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = forest->start;
							}
							insertWorld("world", world);
						}
						else if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "end") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = forest->end;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = forest->end;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = forest->end;
							}
							insertWorld("world", world);
						}
						else{
							logError(_logger, "Unkown forest attribute of int type: %s\n", worldAssignment->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
						}
						return;
					}
					else if(type == WORLD_TYPE){
						if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "height") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = world->height;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = world->height;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = world->height;
							}
							insertWorld("world", world);
						}
						else if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "width") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = world->width;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = world->width;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = world->width;
							}
							insertWorld("world", world);
						}
						else if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "uneveness") == 0){
							if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
								world->uneveness = world->uneveness;
							}
							else if(strcmp(worldAssignment->id->idValue, "width") == 0){
								world->width = world->uneveness;
							}
							else if(strcmp(worldAssignment->id->idValue, "height") == 0){
								world->height = world->uneveness;
							}
							insertWorld("world", world);
						}
						else{
							logError(_logger, "Unkown world attribute of int type: %s\n", worldAssignment->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
						}
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(strcmp(worldAssignment->id->idValue, "mesage") == 0){
					if(type == WORLD_TYPE){
						if(strcmp(worldAssignment->value->attValue->attributeID->idValue, "message") == 0){
							world->message = world->message;
							insertWorld("world", world);
						}
						else{
							logError(_logger, "Unkown world attribute of str type: %s\n", worldAssignment->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
						}
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes of str type\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else{
					logError(_logger, "Unknown worldAttribute: %s\n", worldAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;
			}
			else{
				logError(_logger, "Unknown attributeValueType: %d\n", worldAssignment->value->attValue->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else{
			logError(_logger, "Wrong DeclarationValueType for worldAssignment: %d\n", worldAssignment->value->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}		
	}
	else if(worldAssignment->type == ID_BY_OPP){//OBS: only INTCLASS att are possible then
		if(strcmp(worldAssignment->id->idValue, "message") == 0){
			logError(_logger, "Tried to initialize world->message with int result");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}

		int op = _generateArithmeticOperation(worldAssignment->arithmeticOperation);
		if(ERROR_OCCURED==true) return;

		if(strcmp(worldAssignment->id->idValue, "height") == 0){
			world->height = op;
			insertWorld("world", world);

		}
		else if(strcmp(worldAssignment->id->idValue, "width") == 0){
			world->width = op;
			insertWorld("world", world);
		}
		else if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
			world->uneveness = op;
			insertWorld("world", world);
		}
		else{
			logError(_logger, "Unknown world attribute assign by operation: name(%s)\n", worldAssignment->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		return;
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
	_WORLD * world = calloc(1, sizeof(_WORLD));
	world->height = DEFAULT_WORLD_HEIGHT;
	world->width = DEFAULT_WORLD_WIDTH;
	world->uneveness = DEFAULT_WORLD_UNEVENESS;
	world->message = DEFAULT_WORLD_MESSAGE;
	insertWorld("world", world);

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
	compi = compilationStatus;
	_generateProgram(compilerState->abstractSyntaxtTree);
	if(ERROR_OCCURED == false) _generateEpilogue();//TODO podria cambiar el nombre mas descriptivo
	logDebugging(_logger, "Generation is done.");
	return ERROR_OCCURED==true? 1:0;
}
