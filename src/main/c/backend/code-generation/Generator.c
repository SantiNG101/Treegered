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
static void _generateForestAssignments(ForestAssignments * forestAssignments, char * forestId);
static void _generateForestAssignment(ForestAssignment * forestAssignment, char * forestId);
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
		int left;
		int right;
		boolean noneleft=true;
		boolean noneright=true;
		if(arithmeticOperation->leftValue->type == ATTvalue){
			if(arithmeticOperation->leftValue->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "width") == 0){
					left = world->width;
					noneleft=false;
				}
				else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "height") == 0){
					left = world->height;
					noneleft=false;
				}
				else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "uneveness") == 0){
					left = world->uneveness;
					noneleft=false;
				}
				else{
					logError(_logger, "Unknown int assign by world attribute: name(%s)\n", arithmeticOperation->leftValue->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
			}		
			else if(arithmeticOperation->leftValue->attValue->type == IDatt){
				EntryType type = getType(arithmeticOperation->leftValue->attValue->variableID->idValue);
				if(type == EMPTY_TYPE){
					logError(_logger, "Nonexistent variable: %s\n", arithmeticOperation->leftValue->attValue->variableID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
				if(type == TREE_TYPE){
					_TREE * tree = getTree(arithmeticOperation->leftValue->attValue->variableID->idValue).value._tree;
					if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "x") == 0){
						left = tree->x;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "height") == 0){
						left = tree->height;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "density") == 0){
						left = tree->density;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "depth") == 0){
						left = tree->depth;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "bark") == 0){
						left = tree->bark;
						noneleft=false;
					}
					else{
						logError(_logger, "Unknown int assign by tree attribute: name(%s)\n", arithmeticOperation->leftValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return 0;
					}
				}
				else if(type == FOREST_TYPE){
					_FOREST * forest = getTree(arithmeticOperation->leftValue->attValue->variableID->idValue).value._forest;
					if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "start") == 0){
						left = forest->start;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "end") == 0){
						left = forest->end;
						noneleft=false;
					}
					else{
						logError(_logger, "Unknown int assign by forest attribute: name(%s)\n", arithmeticOperation->leftValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return 0;
					}
				}
				else{
					logError(_logger, "Variables of type %d dont have attributes\n", type);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
			}	
		}
		if(arithmeticOperation->rightValue->type == ATTvalue){
			if(arithmeticOperation->rightValue->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "width") == 0){
					right = world->width;
					noneright=false;
				}
				else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "height") == 0){
					right = world->height;
					noneright=false;
				}
				else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "uneveness") == 0){
					right = world->uneveness;
					noneright=false;
				}
				else{
					logError(_logger, "Unknown int assign by world attribute: name(%s)\n", arithmeticOperation->rightValue->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
			}		
			else if(arithmeticOperation->rightValue->attValue->type == IDatt){
				EntryType type = getType(arithmeticOperation->rightValue->attValue->variableID->idValue);
				if(type == EMPTY_TYPE){
					logError(_logger, "Nonexistent variable: %s\n", arithmeticOperation->rightValue->attValue->variableID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
				if(type == TREE_TYPE){
					_TREE * tree = getTree(arithmeticOperation->rightValue->attValue->variableID->idValue).value._tree;
					if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "x") == 0){
						right = tree->x;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "height") == 0){
						right = tree->height;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "density") == 0){
						right = tree->density;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "depth") == 0){
						right = tree->depth;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "bark") == 0){
						right = tree->bark;
						noneright=false;
					}
					else{
						logError(_logger, "Unknown int assign by tree attribute: name(%s)\n", arithmeticOperation->rightValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return 0;
					}
				}
				else if(type == FOREST_TYPE){
					_FOREST * forest = getTree(arithmeticOperation->rightValue->attValue->variableID->idValue).value._forest;
					if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "start") == 0){
						right = forest->start;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "end") == 0){
						right = forest->end;
						noneright=false;
					}
					else{
						logError(_logger, "Unknown int assign by forest attribute: name(%s)\n", arithmeticOperation->rightValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return 0;
					}
				}
				else{
					logError(_logger, "Variables of type %d dont have attributes\n", type);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
			}	
		}
		if(arithmeticOperation->leftValue->type == DECLARATIONvalue){
			ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
			aux->operator = arithmeticOperation->operator;
			aux->arithOp = arithmeticOperation->arithOp;
			if(arithmeticOperation->type == LV_RV || arithmeticOperation->type == LV_RO) aux->leftValue = arithmeticOperation->leftValue->declareValue;
			if(arithmeticOperation->type == LV_RV || arithmeticOperation->type == LO_RV) aux->rightValue = arithmeticOperation->rightValue;
			if(arithmeticOperation->type == LO_RV || arithmeticOperation->type == LO_RO) aux->leftOperation = arithmeticOperation->leftOperation;
			if(arithmeticOperation->type == LV_RO || arithmeticOperation->type == LO_RO) aux->rightOperation = arithmeticOperation->rightOperation;
			aux->type = arithmeticOperation->type;
			int toRet = _generateArithmeticOperation(aux);
			free(aux);
			left = toRet;
			noneleft=false;
		}
		if(arithmeticOperation->rightValue->type == DECLARATIONvalue){
			ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
			aux->operator = arithmeticOperation->operator;
			aux->arithOp = arithmeticOperation->arithOp;
			if(arithmeticOperation->type == LV_RV || arithmeticOperation->type == LV_RO)aux->leftValue = arithmeticOperation->leftValue;
			if(arithmeticOperation->type == LV_RV || arithmeticOperation->type == LO_RV)aux->rightValue = arithmeticOperation->rightValue->declareValue;
			if(arithmeticOperation->type == LO_RV || arithmeticOperation->type == LO_RO)aux->leftOperation = arithmeticOperation->leftOperation;
			if(arithmeticOperation->type == LV_RO || arithmeticOperation->type == LO_RO)aux->rightOperation = arithmeticOperation->rightOperation;
			aux->type = arithmeticOperation->type;
			int toRet = _generateArithmeticOperation(aux);
			free(aux);
			right = toRet;
			noneright=false;
		}
		if(arithmeticOperation->leftValue->type == INTEGERvalue){
			left = arithmeticOperation->leftValue->intValue->value;
			noneleft=false;
		}
		if(arithmeticOperation->rightValue->type == INTEGERvalue){
			right = arithmeticOperation->rightValue->intValue->value;
			noneright=false;
		}
		if(noneleft==true || noneright==true){
			logError(_logger, "Cannot operate with type other than int\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return false;
		}
		return _getArithOpResult(left, right, arithmeticOperation->operator);
	}
	else if(arithmeticOperation->type == LV_RO){
		int left;
		boolean noneleft=true;
		if(arithmeticOperation->leftValue->type == ATTvalue){
			if(arithmeticOperation->leftValue->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "width") == 0){
					left = world->width;
					noneleft=false;
				}
				else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "height") == 0){
					left = world->height;
					noneleft=false;
				}
				else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "uneveness") == 0){
					left = world->uneveness;
					noneleft=false;
				}
				else{
					logError(_logger, "Unknown int assign by world attribute: name(%s)\n", arithmeticOperation->leftValue->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
			}		
			else if(arithmeticOperation->leftValue->attValue->type == IDatt){
				EntryType type = getType(arithmeticOperation->leftValue->attValue->variableID->idValue);
				if(type == EMPTY_TYPE){
					logError(_logger, "Nonexistent variable: %s\n", arithmeticOperation->leftValue->attValue->variableID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
				if(type == TREE_TYPE){
					_TREE * tree = getTree(arithmeticOperation->leftValue->attValue->variableID->idValue).value._tree;
					if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "x") == 0){
						left = tree->x;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "height") == 0){
						left = tree->height;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "density") == 0){
						left = tree->density;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "depth") == 0){
						left = tree->depth;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "bark") == 0){
						left = tree->bark;
						noneleft=false;
					}
					else{
						logError(_logger, "Unknown int assign by tree attribute: name(%s)\n", arithmeticOperation->leftValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return 0;
					}
				}
				else if(type == FOREST_TYPE){
					_FOREST * forest = getTree(arithmeticOperation->leftValue->attValue->variableID->idValue).value._forest;
					if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "start") == 0){
						left = forest->start;
						noneleft=false;
					}
					else if(strcmp(arithmeticOperation->leftValue->attValue->attribute->idValue, "end") == 0){
						left = forest->end;
						noneleft=false;
					}
					else{
						logError(_logger, "Unknown int assign by forest attribute: name(%s)\n", arithmeticOperation->leftValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return 0;
					}
				}
				else{
					logError(_logger, "Variables of type %d dont have attributes\n", type);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
			}	
		}
		if(arithmeticOperation->leftValue->type == DECLARATIONvalue){
			ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
			aux->operator = arithmeticOperation->operator;
			aux->arithOp = arithmeticOperation->arithOp;
			if(arithmeticOperation->type == LV_RV || arithmeticOperation->type == LV_RO) aux->leftValue = arithmeticOperation->leftValue->declareValue;
			if(arithmeticOperation->type == LV_RV || arithmeticOperation->type == LO_RV) aux->rightValue = arithmeticOperation->rightValue;
			if(arithmeticOperation->type == LO_RV || arithmeticOperation->type == LO_RO) aux->leftOperation = arithmeticOperation->leftOperation;
			if(arithmeticOperation->type == LV_RO || arithmeticOperation->type == LO_RO) aux->rightOperation = arithmeticOperation->rightOperation;
			aux->type = arithmeticOperation->type;
			int toRet = _generateArithmeticOperation(aux);
			free(aux);
			left = toRet;
			noneleft=false;
		}
		if(arithmeticOperation->leftValue->type == INTEGERvalue){
			left = arithmeticOperation->leftValue->intValue->value;
			noneleft=false;
		}
		if(noneleft==true){
			logError(_logger, "Cannot operate with type other than int\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return false;
		}
		return _getArithOpResult(left, _generateArithmeticOperation(arithmeticOperation->rightOperation), arithmeticOperation->operator);
	}
	else if(arithmeticOperation->type == LO_RV){
		int right;
		boolean noneright=true;
		if(arithmeticOperation->rightValue->type == ATTvalue){
			if(arithmeticOperation->rightValue->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "width") == 0){
					right = world->width;
					noneright=false;
				}
				else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "height") == 0){
					right = world->height;
					noneright=false;
				}
				else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "uneveness") == 0){
					right = world->uneveness;
					noneright=false;
				}
				else{
					logError(_logger, "Unknown int assign by world attribute: name(%s)\n", arithmeticOperation->rightValue->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
			}		
			else if(arithmeticOperation->rightValue->attValue->type == IDatt){
				EntryType type = getType(arithmeticOperation->rightValue->attValue->variableID->idValue);
				if(type == EMPTY_TYPE){
					logError(_logger, "Nonexistent variable: %s\n", arithmeticOperation->rightValue->attValue->variableID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
				if(type == TREE_TYPE){
					_TREE * tree = getTree(arithmeticOperation->rightValue->attValue->variableID->idValue).value._tree;
					if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "x") == 0){
						right = tree->x;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "height") == 0){
						right = tree->height;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "density") == 0){
						right = tree->density;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "depth") == 0){
						right = tree->depth;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "bark") == 0){
						right = tree->bark;
						noneright=false;
					}
					else{
						logError(_logger, "Unknown int assign by tree attribute: name(%s)\n", arithmeticOperation->rightValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return 0;
					}
				}
				else if(type == FOREST_TYPE){
					_FOREST * forest = getTree(arithmeticOperation->rightValue->attValue->variableID->idValue).value._forest;
					if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "start") == 0){
						right = forest->start;
						noneright=false;
					}
					else if(strcmp(arithmeticOperation->rightValue->attValue->attribute->idValue, "end") == 0){
						right = forest->end;
						noneright=false;
					}
					else{
						logError(_logger, "Unknown int assign by forest attribute: name(%s)\n", arithmeticOperation->rightValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return 0;
					}
				}
				else{
					logError(_logger, "Variables of type %d dont have attributes\n", type);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return 0;
				}
			}	
		}
		if(arithmeticOperation->rightValue->type == DECLARATIONvalue){
			ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
			aux->operator = arithmeticOperation->operator;
			aux->arithOp = arithmeticOperation->arithOp;
			if(arithmeticOperation->type == LV_RV || arithmeticOperation->type == LV_RO)aux->leftValue = arithmeticOperation->leftValue;
			if(arithmeticOperation->type == LV_RV || arithmeticOperation->type == LO_RV)aux->rightValue = arithmeticOperation->rightValue->declareValue;
			if(arithmeticOperation->type == LO_RV || arithmeticOperation->type == LO_RO)aux->leftOperation = arithmeticOperation->leftOperation;
			if(arithmeticOperation->type == LV_RO || arithmeticOperation->type == LO_RO)aux->rightOperation = arithmeticOperation->rightOperation;
			aux->type = arithmeticOperation->type;
			int toRet = _generateArithmeticOperation(aux);
			free(aux);
			right = toRet;
			noneright=false;
		}
		if(arithmeticOperation->rightValue->type == INTEGERvalue){
			right = arithmeticOperation->rightValue->intValue->value;
			noneright=false;
		}
		if(noneright==true){
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
		DeclarationValue * dvLeft = NULL;
		DeclarationValue * dvRight = NULL;
		boolean freeLeft = false;
		boolean freeRight = false;
		if(conditionalClause->leftValue->type == ATTvalue){
			if(conditionalClause->leftValue->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(conditionalClause->leftValue->attValue->attribute->idValue, "height") == 0){
					DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
					aux->type = INTEGERvalue;
					aux->intValue->value = world->height;
					dvLeft=aux;
					freeLeft = true;
				}
				else if(strcmp(conditionalClause->leftValue->attValue->attribute->idValue, "width") == 0){
					DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
					aux->type = INTEGERvalue;
					aux->intValue->value = world->width;
					dvLeft=aux;
					freeLeft = true;
				}
				else if(strcmp(conditionalClause->leftValue->attValue->attribute->idValue, "uneveness") == 0){
					DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
					aux->type = INTEGERvalue;
					aux->intValue->value = world->uneveness;
					dvLeft=aux;
					freeLeft = true;
				}
				else{
					logError(_logger, "Attribute cant be used in conditional clause\n");
					ERROR_OCCURED = true;
					*compi=FAILED;
					return false;
				}
			}
			else if(conditionalClause->leftValue->attValue->type == IDatt){
				EntryType type = getType(conditionalClause->leftValue->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", conditionalClause->leftValue->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				if(type == TREE_TYPE){
					_TREE * tree = getTree(conditionalClause->leftValue->attValue->variableID->idValue).value._tree;
					if(strcmp(conditionalClause->leftValue->attValue->attributeID->idValue, "x") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->x;
						dvLeft=aux;
						freeLeft = true;
					}
					else if(strcmp(conditionalClause->leftValue->attValue->attributeID->idValue, "height") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->height;
						dvLeft=aux;
						freeLeft = true;
					}
					else if(strcmp(conditionalClause->leftValue->attValue->attributeID->idValue, "depth") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->depth;
						dvLeft=aux;
						freeLeft = true;
					}
					else if(strcmp(conditionalClause->leftValue->attValue->attributeID->idValue, "density") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->density;
						dvLeft=aux;
						freeLeft = true;
					}
					else if(strcmp(conditionalClause->leftValue->attValue->attributeID->idValue, "bark") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->bark;
						dvLeft=aux;
						freeLeft = true;
					}
					else{
						logError(_logger, "Attribute cannot be used in conditional clause\n");
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				}
				else if(type == FOREST_TYPE){
					_FOREST * forest = getForest(conditionalClause->leftValue->attValue->variableID->idValue).value._forest;
					if(strcmp(conditionalClause->leftValue->attValue->attributeID->idValue, "start") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = forest->start;
						dvLeft=aux;
						freeLeft = true;
					}
					else if(strcmp(conditionalClause->leftValue->attValue->attributeID->idValue, "end") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = forest->end;
						dvLeft=aux;
						freeLeft = true;
					}
					else{
						logError(_logger, "Attribute cannot be used in conditional clause\n");
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				}
				else{
					logError(_logger, "Attribute cannot be used in conditional clause\n");
					ERROR_OCCURED = true;
					*compi=FAILED;
					return false;
				}
			}
			else{
				logError(_logger, "Unknown AttributeValueType: %d\n", conditionalClause->leftValue->attValue->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return false;
			}
		}
		if(conditionalClause->rightValue->type == ATTvalue){
			if(conditionalClause->rightValue->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(conditionalClause->rightValue->attValue->attribute->idValue, "height") == 0){
					DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
					aux->type = INTEGERvalue;
					aux->intValue->value = world->height;
					dvRight=aux;
					freeRight = true;
				}
				else if(strcmp(conditionalClause->rightValue->attValue->attribute->idValue, "width") == 0){
					DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
					aux->type = INTEGERvalue;
					aux->intValue->value = world->width;
					dvRight=aux;
					freeRight = true;
				}
				else if(strcmp(conditionalClause->rightValue->attValue->attribute->idValue, "uneveness") == 0){
					DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
					aux->type = INTEGERvalue;
					aux->intValue->value = world->uneveness;
					dvRight=aux;
					freeRight = true;
				}
				else{
					logError(_logger, "Attribute cant be used in conditional clause\n");
					ERROR_OCCURED = true;
					*compi=FAILED;
					return false;
				}
			}
			else if(conditionalClause->rightValue->attValue->type == IDatt){
				EntryType type = getType(conditionalClause->rightValue->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", conditionalClause->rightValue->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				if(type == TREE_TYPE){
					_TREE * tree = getTree(conditionalClause->rightValue->attValue->variableID->idValue).value._tree;
					if(strcmp(conditionalClause->rightValue->attValue->attributeID->idValue, "x") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->x;
						dvRight=aux;
						freeRight = true;
					}
					else if(strcmp(conditionalClause->rightValue->attValue->attributeID->idValue, "height") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->height;
						dvRight=aux;
						freeRight = true;
					}
					else if(strcmp(conditionalClause->rightValue->attValue->attributeID->idValue, "depth") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->depth;
						dvRight=aux;
						freeRight = true;
					}
					else if(strcmp(conditionalClause->rightValue->attValue->attributeID->idValue, "density") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->density;
						dvRight=aux;
						freeRight = true;
					}
					else if(strcmp(conditionalClause->rightValue->attValue->attributeID->idValue, "bark") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = tree->bark;
						dvRight=aux;
						freeRight = true;
					}
					else{
						logError(_logger, "Attribute cannot be used in conditional clause\n");
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				}
				else if(type == FOREST_TYPE){
					_FOREST * forest = getForest(conditionalClause->rightValue->attValue->variableID->idValue).value._forest;
					if(strcmp(conditionalClause->rightValue->attValue->attributeID->idValue, "start") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = forest->start;
						dvRight=aux;
						freeRight = true;
					}
					else if(strcmp(conditionalClause->rightValue->attValue->attributeID->idValue, "end") == 0){
						DeclarationValue * aux = calloc(1, sizeof(DeclarationValue));
						aux->type = INTEGERvalue;
						aux->intValue->value = forest->end;
						dvRight=aux;
						freeRight = true;
					}
					else{
						logError(_logger, "Attribute cannot be used in conditional clause\n");
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				}
				else{
					logError(_logger, "Attribute cannot be used in conditional clause\n");
					ERROR_OCCURED = true;
					*compi=FAILED;
					return false;
				}
			}
			else{
				logError(_logger, "Unknown AttributeValueType: %d\n", conditionalClause->rightValue->attValue->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return false;
			}
		}
		if(dvLeft == NULL) dvLeft = conditionalClause->leftValue;
		if(dvRight == NULL) dvRight = conditionalClause->rightValue;
		int toRet = _getConditionalClauseResultVV(dvLeft, dvRight, conditionalClause->comparissonType);
		if(freeLeft == true) free(dvLeft);
		if(freeRight == true) free(dvRight);
		return toRet;
	}
	else if(conditionalClause->conditionalType == V_C){
		if(conditionalClause->leftValue->type == ATTvalue){
			if(conditionalClause->leftValue->attValue->type == WORLDatt){
				logError(_logger, "No world attribute is boolean\n");
				ERROR_OCCURED = true;
				*compi=FAILED;
				return false;
			}
			else if(conditionalClause->leftValue->attValue->type == IDatt){
				EntryType type = getType(conditionalClause->leftValue->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", conditionalClause->leftValue->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				if(type == TREE_TYPE){
					if(strcmp(conditionalClause->leftValue->attValue->attributeID->idValue, "snowed") == 0){
						_TREE * tree = getTree(conditionalClause->leftValue->attValue->variableID->idValue).value._tree;
						return _getConditionalClauseResultBool(tree->snowed, _generateConditionalClause(conditionalClause->leftConditionalClause), conditionalClause->comparissonType);
					}
					else{
						logError(_logger, "tree->%s is not boolean\n", conditionalClause->leftValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				}
				else{
					logError(_logger, "No boolean attribute in type %d\n", type);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return false;
				}
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
		if(conditionalClause->rightValue->type == ATTvalue){
			if(conditionalClause->rightValue->attValue->type == WORLDatt){
				logError(_logger, "No world attribute is boolean\n");
				ERROR_OCCURED = true;
				*compi=FAILED;
				return false;
			}
			else if(conditionalClause->rightValue->attValue->type == IDatt){
				EntryType type = getType(conditionalClause->rightValue->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", conditionalClause->rightValue->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				if(type == TREE_TYPE){
					if(strcmp(conditionalClause->rightValue->attValue->attributeID->idValue, "snowed") == 0){
						_TREE * tree = getTree(conditionalClause->rightValue->attValue->variableID->idValue).value._tree;
						return _getConditionalClauseResultBool(_generateConditionalClause(conditionalClause->leftConditionalClause), tree->snowed, conditionalClause->comparissonType);
					}
					else{
						logError(_logger, "tree->%s is not boolean\n", conditionalClause->rightValue->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return false;
					}
				}
				else{
					logError(_logger, "No boolean attribute in type %d\n", type);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return false;
				}
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

	if(generalAssignation->type == ID_BY_VALUE_TYPE){
		if(generalAssignation->classType->class == INTCLASS){
			if(generalAssignation->value->type == INTEGERvalue){
				_INTEGER * integer = calloc(1, sizeof(_INTEGER));
				integer->value = generalAssignation->value->intValue->value;
				insertInteger(generalAssignation->idDeclared->idValue, integer);
				return;
			}
			else if(generalAssignation->value->type == ATTvalue){
				if(generalAssignation->value->attValue->type == WORLDatt){
					_WORLD * world = getWorld("world").value._world;

					if(strcmp(generalAssignation->value->attValue->attribute->idValue, "width") == 0){
						_INTEGER * integer = calloc(1, sizeof(_INTEGER));
						integer->value = world->width;
						insertInteger(generalAssignation->idDeclared->idValue, integer);
						return;
					}
					else if(strcmp(generalAssignation->value->attValue->attribute->idValue, "height") == 0){
						_INTEGER * integer = calloc(1, sizeof(_INTEGER));
						integer->value = world->height;
						insertInteger(generalAssignation->idDeclared->idValue, integer);
						return;
					}
					else if(strcmp(generalAssignation->value->attValue->attribute->idValue, "uneveness") == 0){
						_INTEGER * integer = calloc(1, sizeof(_INTEGER));
						integer->value = world->uneveness;
						insertInteger(generalAssignation->idDeclared->idValue, integer);
						return;
					}
					else{
						logError(_logger, "Unknown int assign by world attribute: name(%s)\n", generalAssignation->value->attValue->attribute->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->attValue->type == IDatt){
					EntryType type = getType(generalAssignation->value->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->value->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					if(type == TREE_TYPE){
						_TREE * tree = getTree(generalAssignation->value->attValue->variableID->idValue).value._tree;
						if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "x") == 0){
							_INTEGER * integer = calloc(1, sizeof(_INTEGER));
							integer->value = tree->x;
							insertInteger(generalAssignation->idDeclared->idValue, integer);
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "depth") == 0){
							_INTEGER * integer = calloc(1, sizeof(_INTEGER));
							integer->value = tree->depth;
							insertInteger(generalAssignation->idDeclared->idValue, integer);
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "density") == 0){
							_INTEGER * integer = calloc(1, sizeof(_INTEGER));
							integer->value = tree->density;
							insertInteger(generalAssignation->idDeclared->idValue, integer);
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "bark") == 0){
							_INTEGER * integer = calloc(1, sizeof(_INTEGER));
							integer->value = tree->bark;
							insertInteger(generalAssignation->idDeclared->idValue, integer);
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "height") == 0){
							_INTEGER * integer = calloc(1, sizeof(_INTEGER));
							integer->value = tree->height;
							insertInteger(generalAssignation->idDeclared->idValue, integer);
							return;
						}
						else{
							logError(_logger, "Unknown int assign by tree attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
							return;
						}
					}
					else if(type == FOREST_TYPE){
						_FOREST * forest = getTree(generalAssignation->value->attValue->variableID->idValue).value._forest;
						if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "start") == 0){
							_INTEGER * integer = calloc(1, sizeof(_INTEGER));
							integer->value = forest->start;
							insertInteger(generalAssignation->idDeclared->idValue, integer);
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "end") == 0){
							_INTEGER * integer = calloc(1, sizeof(_INTEGER));
							integer->value = forest->end;
							insertInteger(generalAssignation->idDeclared->idValue, integer);
							return;
						}
						else{
							logError(_logger, "Unknown int assign by forest attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
							return;
						}
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
				}
				else{
					EntryType type = getType(generalAssignation->att->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
				}
			}
			else if(generalAssignation->value->type == DECLARATIONvalue){
				GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
				aux->idDeclared = generalAssignation->idDeclared;
				aux->classType = generalAssignation->classType;
				aux->value = generalAssignation->value->declareValue;
				aux->type = generalAssignation->type;
				_generateGeneralAssignation(aux);
				free(aux);
				return;
			}
			else{
				logError(_logger, "Tried to assign DeclarationType: %d to int classType\n", generalAssignation->value->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
		}
		else if(generalAssignation->classType->class == STRCLASS){
			if(generalAssignation->value->type == STRINGvalue){
				_STRING * string = calloc(1, sizeof(_STRING));
				string->value = generalAssignation->value->charValue->value;
				insertString(generalAssignation->idDeclared->idValue, string);
				return;
			}
			else if(generalAssignation->value->type == ATTvalue){
				if(generalAssignation->value->attValue->type == WORLDatt){
					_WORLD * world = getWorld("world").value._world;

					if(strcmp(generalAssignation->value->attValue->attribute->idValue, "message") == 0){
						_STRING * string = calloc(1, sizeof(_STRING));
						string->value = world->message;
						insertString(generalAssignation->idDeclared->idValue, string);
						return;
					}
					else{
						logError(_logger, "Unknown str assign by world attribute: name(%s)\n", generalAssignation->value->attValue->attribute->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->attValue->type == IDatt){
					EntryType type = getType(generalAssignation->value->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->value->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					if(type == TREE_TYPE){
						logError(_logger, "Unknown str assign by tree attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes to init str\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
				}
				else{
					EntryType type = getType(generalAssignation->att->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
				}
			}
			else if(generalAssignation->value->type == DECLARATIONvalue){
				GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
				aux->idDeclared = generalAssignation->idDeclared;
				aux->classType = generalAssignation->classType;
				aux->value = generalAssignation->value->declareValue;
				aux->type = generalAssignation->type;
				_generateGeneralAssignation(aux);
				free(aux);
				return;
			}
			else{
				logError(_logger, "Tried to assign DeclarationType: %d to str classType\n", generalAssignation->value->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
		}
		else if(generalAssignation->classType->class == BOOLCLASS){
			if(generalAssignation->value->type == BOOLEANvalue){
				_BOOLEAN * boool = calloc(1, sizeof(_BOOLEAN));
				boool->value = generalAssignation->value->booleanValue->value;
				insertBoolean(generalAssignation->idDeclared->idValue, boool);
				return;
			}
			else if(generalAssignation->value->type == ATTvalue){
				if(generalAssignation->value->attValue->type == WORLDatt){
					logError(_logger, "Unknown bool assign by world attribute: name(%s)\n", generalAssignation->value->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
				else if(generalAssignation->value->attValue->type == IDatt){
					EntryType type = getType(generalAssignation->value->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->value->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					if(type == TREE_TYPE){
						_TREE * tree = getTree(generalAssignation->value->attValue->variableID->idValue).value._tree;
						if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "snowed") == 0){
							_BOOLEAN * boool = calloc(1, sizeof(_BOOLEAN));
							boool->value = tree->snowed;
							insertBoolean(generalAssignation->idDeclared->idValue, boool);
							return;
						}
						else{
							logError(_logger, "Unknown boolean assign by tree attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
							return;
						}
					}
					else if(type == FOREST_TYPE){
						logError(_logger, "Unknown boolean assign by forest attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
				}
				else{
					EntryType type = getType(generalAssignation->att->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
				}
			}
			else if(generalAssignation->value->type == DECLARATIONvalue){
				GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
				aux->idDeclared = generalAssignation->idDeclared;
				aux->classType = generalAssignation->classType;
				aux->value = generalAssignation->value->declareValue;
				aux->type = generalAssignation->type;
				_generateGeneralAssignation(aux);
				free(aux);
				return;
			}
			else{
				logError(_logger, "Tried to assign DeclarationType: %d to boolean classType\n", generalAssignation->value->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
		}
		else if(generalAssignation->classType->class == HEXCOLORCLASS){
			if(generalAssignation->value->type == HEXCOLORvalue){
				_HEXCOLOR * color = calloc(1, sizeof(_HEXCOLOR));
				color->value = generalAssignation->value->hexcolorValue->value;
				insertHexcolor(generalAssignation->idDeclared->idValue, color);
				return;
			}
			else if(generalAssignation->value->type == ATTvalue){
				if(generalAssignation->value->attValue->type == WORLDatt){
					logError(_logger, "Unknown hexcolor assign by world attribute: name(%s)\n", generalAssignation->value->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
				else if(generalAssignation->value->attValue->type == IDatt){
					EntryType type = getType(generalAssignation->value->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->value->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					if(type == TREE_TYPE){
						_TREE * tree = getTree(generalAssignation->value->attValue->variableID->idValue).value._tree;
						if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "color") == 0){
							_HEXCOLOR * color = calloc(1, sizeof(_HEXCOLOR));
							color->value = tree->color;
							insertHexcolor(generalAssignation->idDeclared->idValue, color);
							return;
						}
						else{
							logError(_logger, "Unknown hexcolor assign by tree attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
							return;
						}
					}
					else if(type == FOREST_TYPE){
						logError(_logger, "Unknown hexcolor assign by forest attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes for hexcolor init\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
				}
				else{
					EntryType type = getType(generalAssignation->att->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
				}
			}
			else if(generalAssignation->value->type == DECLARATIONvalue){
				GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
				aux->idDeclared = generalAssignation->idDeclared;
				aux->classType = generalAssignation->classType;
				aux->value = generalAssignation->value->declareValue;
				aux->type = generalAssignation->type;
				_generateGeneralAssignation(aux);
				free(aux);
				return;
			}
			else{
				logError(_logger, "Tried to assign DeclarationType: %d to int classType\n", generalAssignation->value->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
		}
		else{
			logError(_logger, "Unkown classType: %d\n", generalAssignation->classType->class);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}		
		return;
	}
	else if(generalAssignation->type == ID_BY_OPP_TYPE){//OBS: only INTCLASS var are possible then
		if(generalAssignation->classType->class != INTCLASS){
			logError(_logger, "Tried to assign int to variable with classType: %d\n", generalAssignation->classType->class);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}

		int op = _generateArithmeticOperation(generalAssignation->arithmeticOperation);
		if(ERROR_OCCURED==true) return;

		_INTEGER * integer = calloc(1, sizeof(_INTEGER));
		integer->value = op;
		insertInteger(generalAssignation->idDeclared->idValue, integer);
	}
	else if(generalAssignation->type == ID_BY_VALUE){
		EntryType type = getType(generalAssignation->id->idValue);
		if(type == EMPTY_TYPE){
			logError(_logger, "Nonexistent variable: %s\n", generalAssignation->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
		if(type == INTEGER_TYPE){
			_INTEGER * integer = getInteger(generalAssignation->id->idValue).value._integer;

			if(generalAssignation->value->type == INTEGERvalue){
				integer->value = generalAssignation->value->intValue->value;
				return;
			}
			else if(generalAssignation->value->type == ATTvalue){
				if(generalAssignation->value->attValue->type == WORLDatt){
					_WORLD * world = getWorld("world").value._world;

					if(strcmp(generalAssignation->value->attValue->attribute->idValue, "width") == 0){
						integer->value = world->width;
						return;
					}
					else if(strcmp(generalAssignation->value->attValue->attribute->idValue, "height") == 0){
						integer->value = world->height;
						return;
					}
					else if(strcmp(generalAssignation->value->attValue->attribute->idValue, "uneveness") == 0){
						integer->value = world->uneveness;
						return;
					}
					else{
						logError(_logger, "Unknown int assign by world attribute: name(%s)\n", generalAssignation->value->attValue->attribute->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->attValue->type == IDatt){
					EntryType type = getType(generalAssignation->value->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->value->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					if(type == TREE_TYPE){
						_TREE * tree = getTree(generalAssignation->value->attValue->variableID->idValue).value._tree;
						if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "x") == 0){
							integer->value = tree->x;
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "depth") == 0){
							integer->value = tree->depth;
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "density") == 0){
							integer->value = tree->density;
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "bark") == 0){
							integer->value = tree->bark;
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "height") == 0){
							integer->value = tree->height;
							return;
						}
						else{
							logError(_logger, "Unknown int assign by tree attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
							return;
						}
					}
					else if(type == FOREST_TYPE){
						_FOREST * forest = getTree(generalAssignation->value->attValue->variableID->idValue).value._forest;
						if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "start") == 0){
							integer->value = forest->start;
							return;
						}
						else if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "end") == 0){
							integer->value = forest->end;
							return;
						}
						else{
							logError(_logger, "Unknown int assign by forest attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
							return;
						}
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
				}
				else{
					EntryType type = getType(generalAssignation->att->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
				}
			}
			else if(generalAssignation->value->type == DECLARATIONvalue){
				GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
				aux->id = generalAssignation->id;
				aux->value = generalAssignation->value->declareValue;
				aux->type = generalAssignation->type;
				_generateGeneralAssignation(aux);
				free(aux);
				return;
			}
			else{
				logError(_logger, "Tried to assign DeclarationType: %d to int classType\n", generalAssignation->value->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
			return;
		}
		else if(type == STRING_TYPE){
			_STRING * string = getString(generalAssignation->id->idValue).value._string;
		
			if(generalAssignation->value->type == STRINGvalue){
				string->value = generalAssignation->value->charValue->value;
				return;
			}
			else if(generalAssignation->value->type == ATTvalue){
				if(generalAssignation->value->attValue->type == WORLDatt){
					_WORLD * world = getWorld("world").value._world;

					if(strcmp(generalAssignation->value->attValue->attribute->idValue, "message") == 0){
						string->value = world->message;
						return;
					}
					else{
						logError(_logger, "Unknown str assign by world attribute: name(%s)\n", generalAssignation->value->attValue->attribute->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->attValue->type == IDatt){
					EntryType type = getType(generalAssignation->value->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->value->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					if(type == TREE_TYPE){
						logError(_logger, "Unknown str assign by tree attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes to init str\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
				}
				else{
					EntryType type = getType(generalAssignation->att->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
				}
			}
			else if(generalAssignation->value->type == DECLARATIONvalue){
				GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
				aux->id = generalAssignation->id;
				aux->value = generalAssignation->value->declareValue;
				aux->type = generalAssignation->type;
				_generateGeneralAssignation(aux);
				free(aux);
				return;
			}
			else{
				logError(_logger, "Tried to assign DeclarationType: %d to str classType\n", generalAssignation->value->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
			return;
		}
		else if(type == BOOL_TYPE){
			_BOOLEAN * boool = getBoolean(generalAssignation->id->idValue).value._boolean;

			if(generalAssignation->value->type == BOOLEANvalue){
				boool->value = generalAssignation->value->booleanValue->value;
				return;
			}
			else if(generalAssignation->value->type == ATTvalue){
				if(generalAssignation->value->attValue->type == WORLDatt){
					logError(_logger, "Unknown bool assign by world attribute: name(%s)\n", generalAssignation->value->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
				else if(generalAssignation->value->attValue->type == IDatt){
					EntryType type = getType(generalAssignation->value->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->value->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					if(type == TREE_TYPE){
						_TREE * tree = getTree(generalAssignation->value->attValue->variableID->idValue).value._tree;
						if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "snowed") == 0){
							boool->value = tree->snowed;
							return;
						}
						else{
							logError(_logger, "Unknown boolean assign by tree attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
							return;
						}
					}
					else if(type == FOREST_TYPE){
						logError(_logger, "Unknown boolean assign by forest attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
				}
				else{
					EntryType type = getType(generalAssignation->att->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
				}
			}
			else if(generalAssignation->value->type == DECLARATIONvalue){
				GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
				aux->id = generalAssignation->id;
				aux->value = generalAssignation->value->declareValue;
				aux->type = generalAssignation->type;
				_generateGeneralAssignation(aux);
				free(aux);
				return;
			}
			else{
				logError(_logger, "Tried to assign DeclarationType: %d to boolean classType\n", generalAssignation->value->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
			return;
		}
		else if(type == HEXCOLOR_TYPE){
			_HEXCOLOR * color = getHexcolor(generalAssignation->id->idValue).value._hexcolor;

			if(generalAssignation->value->type == HEXCOLORvalue){
				color->value = generalAssignation->value->hexcolorValue->value;
				return;
			}
			else if(generalAssignation->value->type == ATTvalue){
				if(generalAssignation->value->attValue->type == WORLDatt){
					logError(_logger, "Unknown hexcolor assign by world attribute: name(%s)\n", generalAssignation->value->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
				else if(generalAssignation->value->attValue->type == IDatt){
					EntryType type = getType(generalAssignation->value->attValue->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->value->attValue->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					if(type == TREE_TYPE){
						_TREE * tree = getTree(generalAssignation->value->attValue->variableID->idValue).value._tree;
						if(strcmp(generalAssignation->value->attValue->attributeID->idValue, "color") == 0){
							color->value = tree->color;
							return;
						}
						else{
							logError(_logger, "Unknown hexcolor assign by tree attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
							return;
						}
					}
					else if(type == FOREST_TYPE){
						logError(_logger, "Unknown hexcolor assign by forest attribute: name(%s)\n", generalAssignation->value->attValue->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
					else{
						logError(_logger, "Variables of type %d dont have attributes for hexcolor init\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
				}
				else{
					EntryType type = getType(generalAssignation->att->variableID->idValue);
					if(type == EMPTY_TYPE){
						logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
						return;
					}
				}
			}
			else if(generalAssignation->value->type == DECLARATIONvalue){
				GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
				aux->id = generalAssignation->id;
				aux->value = generalAssignation->value->declareValue;
				aux->type = generalAssignation->type;
				_generateGeneralAssignation(aux);
				free(aux);
				return;
			}
			else{
				logError(_logger, "Tried to assign DeclarationType: %d to int classType\n", generalAssignation->value->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
			return;
		}
		else{
			logError(_logger, "Unknown classType to assign: %d\n", type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(generalAssignation->type == ID_BY_OPP){//OBS: only INTCLASS var are possible then
		EntryType type = getType(generalAssignation->id->idValue);
		if(type == EMPTY_TYPE){
			logError(_logger, "Nonexistent variable: %s\n", generalAssignation->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
		else if(type != INTEGER_TYPE){
			logError(_logger, "Tried to assign int to variable: %s\n", generalAssignation->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}

		int op = _generateArithmeticOperation(generalAssignation->arithmeticOperation);
		if(ERROR_OCCURED==true) return;

		_INTEGER * integer = getInteger(generalAssignation->id->idValue).value._integer;
		integer->value = op;
	}
	else if(generalAssignation->type == ATT_BY_VALUE){
		if(generalAssignation->att->type == WORLDatt){
			_WORLD * world = getWorld("world").value._world;
			logError(_logger, "Cannot change worldAttributes after initialization:\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
		else if(generalAssignation->att->type == IDatt){
			EntryType type = getType(generalAssignation->att->variableID->idValue);
			if(type == EMPTY_TYPE){
				logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
			if(type == TREE_TYPE){
				_TREE * tree = getTree(generalAssignation->att->variableID->idValue).value._tree;

				if(generalAssignation->value->type == INTEGERvalue){
					if(strcmp(generalAssignation->att->attributeID->idValue, "x") == 0){
						tree->x = generalAssignation->value->intValue->value;
					}
					else if(strcmp(generalAssignation->att->attributeID->idValue, "height") == 0){
						tree->height = generalAssignation->value->intValue->value;
					}
					else if(strcmp(generalAssignation->att->attributeID->idValue, "density") == 0){
						tree->density = generalAssignation->value->intValue->value;
					}
					else if(strcmp(generalAssignation->att->attributeID->idValue, "depth") == 0){
						tree->depth = generalAssignation->value->intValue->value;
					}
					else if(strcmp(generalAssignation->att->attributeID->idValue, "bark") == 0){
						tree->bark = generalAssignation->value->intValue->value;
					}
					else{
						logError(_logger, "Unknown treeAttribute for int assign: %s\n", generalAssignation->att->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->type == STRINGvalue){
					if(strcmp(generalAssignation->att->attributeID->idValue, "leaf") == 0 && strlen(generalAssignation->value->charValue->value) == 3){
						tree->leaf = generalAssignation->value->charValue->value[1];
					}
					else{
						logError(_logger, "Unknown treeAttribute for char assign: %s\n", generalAssignation->att->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->type == BOOLEANvalue){
					if(strcmp(generalAssignation->att->attributeID->idValue, "snowed") == 0){
						tree->snowed = generalAssignation->value->booleanValue->value;
					}
					else{
						logError(_logger, "Unknown treeAttribute for boolean assign: %s\n", generalAssignation->att->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->type == HEXCOLORvalue){
					if(strcmp(generalAssignation->att->attributeID->idValue, "color") == 0){
						tree->color = generalAssignation->value->hexcolorValue->value;
					}
					else{
						logError(_logger, "Unknown treeAttribute for hexcolor assign: %s\n", generalAssignation->att->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->type == ATTvalue){
					logError(_logger, "Cant do general assign directly to attValue from attValue\n");
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
				else if(generalAssignation->value->type == DECLARATIONvalue){
					GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
					aux->att = generalAssignation->att;
					aux->value = generalAssignation->value->declareValue;
					aux->type = generalAssignation->type;
					_generateGeneralAssignation(aux);
					free(aux);
					return;
				}
				else{
					logError(_logger, "Tried to assign DeclarationType: %d to int classType\n", generalAssignation->value->type);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
			else if(type == FOREST_TYPE){
				_FOREST * forest = getForest(generalAssignation->att->variableID->idValue).value._forest;

				if(generalAssignation->value->type == INTEGERvalue){
					if(strcmp(generalAssignation->att->attributeID->idValue, "start") == 0){
						forest->start = generalAssignation->value->intValue->value;
					}
					else if(strcmp(generalAssignation->att->attributeID->idValue, "end") == 0){
						forest->end = generalAssignation->value->intValue->value;
					}
					else{
						logError(_logger, "Unknown forestAttribute: %s\n", generalAssignation->att->attributeID->idValue);
						ERROR_OCCURED = true;
						*compi=FAILED;
					}
					return;
				}
				else if(generalAssignation->value->type == ATTvalue){
					logError(_logger, "Cant do general assign directly to attValue from attValue\n");
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
				else if(generalAssignation->value->type == DECLARATIONvalue){
					GeneralAssignation * aux = calloc(1, sizeof(GeneralAssignation));
					aux->att = generalAssignation->att;
					aux->value = generalAssignation->value->declareValue;
					aux->type = generalAssignation->type;
					_generateGeneralAssignation(aux);
					free(aux);
					return;
				}
				else{
					logError(_logger, "Tried to assign DeclarationType: %d to int classType\n", generalAssignation->value->type);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
			else{
				logError(_logger, "Variables of type %d dont have attributes\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
			}
			return;
		}
		else{
			logError(_logger, "Unknown attributeValueType: %d\n", generalAssignation->att->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		return;
	}
	else if(generalAssignation->type == ATT_BY_OPP){//OBS: only INTCLASS att are possible then
		if(generalAssignation->att->type == WORLDatt){
			_WORLD * world = getWorld("world").value._world;
			if(strcmp(generalAssignation->att->attribute->idValue, "message") ==0){
				logError(_logger, "Tried to initialize world->message with int result");
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}

			int op = _generateArithmeticOperation(generalAssignation->arithmeticOperation);
			if(ERROR_OCCURED==true) return;

			if(strcmp(generalAssignation->att->attribute->idValue, "width") == 0){
				world->width = op;
			}
			else if(strcmp(generalAssignation->att->attribute->idValue, "height") == 0){
				world->height = op;
			}
			else if(strcmp(generalAssignation->att->attribute->idValue, "uneveness") == 0){
				world->uneveness = op;
			}
			else{
				logError(_logger, "Unknown world attribute assign by operation: name(%s)\n", generalAssignation->att->attribute->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(generalAssignation->att->type == IDatt){
			EntryType type = getType(generalAssignation->att->variableID->idValue);
			if(type == EMPTY_TYPE){
				logError(_logger, "Nonexistent variable: %s\n", generalAssignation->att->variableID->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}

			int op = _generateArithmeticOperation(generalAssignation->arithmeticOperation);
			if(ERROR_OCCURED==true) return;

			if(type == TREE_TYPE){
				_TREE * tree = getTree(generalAssignation->att->variableID->idValue).value._tree;
				if(strcmp(generalAssignation->att->attributeID->idValue, "x") == 0){
					tree->x = op;
				}
				else if(strcmp(generalAssignation->att->attributeID->idValue, "depth") == 0){
					tree->depth = op;
				}
				else if(strcmp(generalAssignation->att->attributeID->idValue, "density") == 0){
					tree->density = op;
				}
				else if(strcmp(generalAssignation->att->attributeID->idValue, "bark") == 0){
					tree->bark = op;
				}
				else if(strcmp(generalAssignation->att->attributeID->idValue, "height") == 0){
					tree->height = op;
				}
				else{
					logError(_logger, "Tried to initialize tree->%s int result\n", generalAssignation->att->attributeID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
			else if(type == FOREST_TYPE){
				_FOREST * forest = getForest(generalAssignation->att->variableID->idValue).value._forest;
				if(strcmp(generalAssignation->att->attributeID->idValue, "start") == 0){
					forest->start = op;
				}
				else if(strcmp(generalAssignation->att->attributeID->idValue, "end") == 0){
					forest->end = op;
				}
				else{
					logError(_logger, "Tried to initialize forest->%s int result\n", generalAssignation->att->attributeID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
			else{
				logError(_logger, "Variables of type %d dont have attributes\n", type);
						ERROR_OCCURED = true;
						*compi=FAILED;
			}
			return;
		}
		else{
			logError(_logger, "Unknown attributeValueType: %d\n", generalAssignation->att->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		return;
	}
	else{
		logError(_logger, "Unknown AssignationType for general assignation: %d\n", generalAssignation->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
}

static void _generateArithmeticAssignation(ArithmeticAssignation * arithmeticAssignation){//INTtypes or forest!
	if(ERROR_OCCURED==true) return;
	if(arithmeticAssignation->type == ID_BY_VALUE){
		EntryType type = getType(arithmeticAssignation->id->idValue);
		if(type == EMPTY_TYPE){
			logError(_logger, "Nonexistent variable: %s\n", arithmeticAssignation->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}

		if(type == INTEGER_TYPE){
			_INTEGER * integer = getInteger(arithmeticAssignation->id->idValue).value._integer;

			ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
			aux->operator = arithmeticAssignation->operator;
			aux->type = LV_RV;
			DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
			auxVal->type = INTEGERvalue;
			auxVal->intValue->value = integer->value;
			aux->leftValue = auxVal;
			aux->rightValue = arithmeticAssignation->value;
			int res = _generateArithmeticOperation(aux);
			free(auxVal);
			free(aux);

			integer->value = res;
			return;
		}
		else if(type == FOREST_TYPE){
			if(arithmeticAssignation->operator != ADD_o){
				logError(_logger, "Only can add elements to a tree\n");
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}

			_FOREST * forest = getForest(arithmeticAssignation->id->idValue).value._forest;
			
			if(arithmeticAssignation->value->type == IDvalue){
				EntryType type = getType(arithmeticAssignation->value->idValue->idValue);
				if(type != TREE_TYPE){
					logError(_logger, "Must be a tree to be added to forest: %s\n", arithmeticAssignation->value->idValue->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
				_TREE * tree = getTree(arithmeticAssignation->value->idValue->idValue).value._tree;
				//TODO logica añadir tree al arbol
			}
			else{
				logError(_logger, "Only ids can be added to forest\n");
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
		}
		else{
			logError(_logger, "Wrong members for arithmeticAssignation: both need to be ints\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(arithmeticAssignation->type == ID_BY_OPP){
		EntryType type = getType(arithmeticAssignation->id->idValue);
		if(type == EMPTY_TYPE){
			logError(_logger, "Nonexistent variable: %s\n", arithmeticAssignation->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}

		if(type == INTEGER_TYPE){
			_INTEGER * integer = getInteger(arithmeticAssignation->id->idValue).value._integer;

			ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
			aux->operator = arithmeticAssignation->operator;
			aux->type = LV_RO;
			DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
			auxVal->type = INTEGERvalue;
			auxVal->intValue->value = integer->value;
			aux->leftValue = auxVal;
			aux->rightOperation = arithmeticAssignation->arithmeticOperation;
			int res = _generateArithmeticOperation(aux);
			free(auxVal);
			free(aux);

			integer->value = res;
			return;
		}
		else{
			logError(_logger, "Wrong members for arithmeticAssignation: both need to be ints\n");
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}		
	}
	else if(arithmeticAssignation->type == ATT_BY_VALUE){
		if(arithmeticAssignation->att->type == WORLDatt){
			_WORLD * world = getWorld("world").value._world;
			if(strcmp(arithmeticAssignation->att->attribute->idValue, "height") == 0){
				ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
				aux->operator = arithmeticAssignation->operator;
				aux->type = LV_RV;
				DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
				auxVal->type = INTEGERvalue;
				auxVal->intValue->value = world->height;
				aux->leftValue = auxVal;
				aux->rightValue = arithmeticAssignation->value;
				int res = _generateArithmeticOperation(aux);
				free(auxVal);
				free(aux);

				world->height = res;
				return;
			}
			else if(strcmp(arithmeticAssignation->att->attribute->idValue, "width") == 0){
				ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
				aux->operator = arithmeticAssignation->operator;
				aux->type = LV_RV;
				DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
				auxVal->type = INTEGERvalue;
				auxVal->intValue->value = world->width;
				aux->leftValue = auxVal;
				aux->rightValue = arithmeticAssignation->value;
				int res = _generateArithmeticOperation(aux);
				free(auxVal);
				free(aux);

				world->width = res;
				return;
			}
			else if(strcmp(arithmeticAssignation->att->attribute->idValue, "uneveness") == 0){
				ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
				aux->operator = arithmeticAssignation->operator;
				aux->type = LV_RV;
				DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
				auxVal->type = INTEGERvalue;
				auxVal->intValue->value = world->uneveness;
				aux->leftValue = auxVal;
				aux->rightValue = arithmeticAssignation->value;
				int res = _generateArithmeticOperation(aux);
				free(auxVal);
				free(aux);

				world->uneveness = res;
				return;
			}
			else{
				logError(_logger, "world->%s is not of type ints\n", arithmeticAssignation->att->attribute->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
		}
		else if(arithmeticAssignation->att->type == IDatt){
			EntryType type = getType(arithmeticAssignation->att->variableID->idValue);
			if(type == EMPTY_TYPE){
				logError(_logger, "Nonexistent variable: %s\n", arithmeticAssignation->att->variableID->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}

			if(type == TREE_TYPE){
				_TREE * tree = getTree(arithmeticAssignation->att->variableID->idValue).value._tree;
				if(strcmp(arithmeticAssignation->att->attributeID->idValue, "x") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RV;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->x;
					aux->leftValue = auxVal;
					aux->rightValue = arithmeticAssignation->value;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->x = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "height") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RV;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->height;
					aux->leftValue = auxVal;
					aux->rightValue = arithmeticAssignation->value;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->height = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "depth") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RV;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->depth;
					aux->leftValue = auxVal;
					aux->rightValue = arithmeticAssignation->value;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->depth = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "density") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RV;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->density;
					aux->leftValue = auxVal;
					aux->rightValue = arithmeticAssignation->value;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->density = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "bark") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RV;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->bark;
					aux->leftValue = auxVal;
					aux->rightValue = arithmeticAssignation->value;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->bark = res;
					return;
				}
				else{
					logError(_logger, "tree->%s is not of type int\n", arithmeticAssignation->att->attributeID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
			else if(type == FOREST_TYPE){
				_FOREST * forest = getForest(arithmeticAssignation->att->variableID->idValue).value._forest;
				if(strcmp(arithmeticAssignation->att->attributeID->idValue, "start") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RV;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = forest->start;
					aux->leftValue = auxVal;
					aux->rightValue = arithmeticAssignation->value;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					forest->start = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "end") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RV;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = forest->end;
					aux->leftValue = auxVal;
					aux->rightValue = arithmeticAssignation->value;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					forest->end = res;
					return;
				}
				else{
					logError(_logger, "forest->%s is not of type int\n", arithmeticAssignation->att->attributeID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
		}
		else{
			logError(_logger, "Unknown attribute type %d\n", arithmeticAssignation->att->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
	}
	else if(arithmeticAssignation->type == ATT_BY_OPP){
		if(arithmeticAssignation->att->type == WORLDatt){
			_WORLD * world = getWorld("world").value._world;
			if(strcmp(arithmeticAssignation->att->attribute->idValue, "height") == 0){
				ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
				aux->operator = arithmeticAssignation->operator;
				aux->type = LV_RO;
				DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
				auxVal->type = INTEGERvalue;
				auxVal->intValue->value = world->height;
				aux->leftValue = auxVal;
				aux->rightOperation = arithmeticAssignation->arithmeticOperation;
				int res = _generateArithmeticOperation(aux);
				free(auxVal);
				free(aux);

				world->height = res;
				return;
			}
			else if(strcmp(arithmeticAssignation->att->attribute->idValue, "width") == 0){
				ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
				aux->operator = arithmeticAssignation->operator;
				aux->type = LV_RO;
				DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
				auxVal->type = INTEGERvalue;
				auxVal->intValue->value = world->width;
				aux->leftValue = auxVal;
				aux->rightOperation = arithmeticAssignation->arithmeticOperation;
				int res = _generateArithmeticOperation(aux);
				free(auxVal);
				free(aux);

				world->width = res;
				return;
			}
			else if(strcmp(arithmeticAssignation->att->attribute->idValue, "uneveness") == 0){
				ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
				aux->operator = arithmeticAssignation->operator;
				aux->type = LV_RO;
				DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
				auxVal->type = INTEGERvalue;
				auxVal->intValue->value = world->uneveness;
				aux->leftValue = auxVal;
				aux->rightOperation = arithmeticAssignation->arithmeticOperation;
				int res = _generateArithmeticOperation(aux);
				free(auxVal);
				free(aux);

				world->uneveness = res;
				return;
			}
			else{
				logError(_logger, "world->%s is not of type ints\n", arithmeticAssignation->att->attribute->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
		}
		else if(arithmeticAssignation->att->type == IDatt){
			EntryType type = getType(arithmeticAssignation->att->variableID->idValue);
			if(type == EMPTY_TYPE){
				logError(_logger, "Nonexistent variable: %s\n", arithmeticAssignation->att->variableID->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}

			if(type == TREE_TYPE){
				_TREE * tree = getTree(arithmeticAssignation->att->variableID->idValue).value._tree;
				if(strcmp(arithmeticAssignation->att->attributeID->idValue, "x") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RO;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->x;
					aux->leftValue = auxVal;
					aux->rightOperation = arithmeticAssignation->arithmeticOperation;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->x = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "height") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RO;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->height;
					aux->leftValue = auxVal;
					aux->rightOperation = arithmeticAssignation->arithmeticOperation;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->height = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "depth") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RO;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->depth;
					aux->leftValue = auxVal;
					aux->rightOperation = arithmeticAssignation->arithmeticOperation;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->depth = res;
					return;
				}	
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "density") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RO;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->density;
					aux->leftValue = auxVal;
					aux->rightOperation = arithmeticAssignation->arithmeticOperation;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->density = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "bark") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RO;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = tree->bark;
					aux->leftValue = auxVal;
					aux->rightOperation = arithmeticAssignation->arithmeticOperation;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					tree->bark = res;
					return;
				}
				else{
					logError(_logger, "tree->%s is not of type int\n", arithmeticAssignation->att->attributeID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
			else if(type == FOREST_TYPE){
				_FOREST * forest = getForest(arithmeticAssignation->att->variableID->idValue).value._forest;
				if(strcmp(arithmeticAssignation->att->attributeID->idValue, "start") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RO;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = forest->start;
					aux->leftValue = auxVal;
					aux->rightOperation = arithmeticAssignation->arithmeticOperation;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					forest->start = res;
					return;
				}
				else if(strcmp(arithmeticAssignation->att->attributeID->idValue, "end") == 0){
					ArithmeticOperation * aux = calloc(1, sizeof(ArithmeticOperation));
					aux->operator = arithmeticAssignation->operator;
					aux->type = LV_RO;
					DeclarationValue * auxVal = calloc(1, sizeof(DeclarationValue));
					auxVal->type = INTEGERvalue;
					auxVal->intValue->value = forest->end;
					aux->leftValue = auxVal;
					aux->rightOperation = arithmeticAssignation->arithmeticOperation;
					int res = _generateArithmeticOperation(aux);
					free(auxVal);
					free(aux);

					forest->end = res;
					return;
				}
				else{
					logError(_logger, "forest->%s is not of type int\n", arithmeticAssignation->att->attributeID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}
			}
		}
		else{
			logError(_logger, "Unknown AttributeValueType: %d\n", arithmeticAssignation->att->type);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
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
		boolean not_anidado = false;
		if(exists(forExpression->id->idValue) && not_anidado){
			logError(_logger, "Variable %s already exists\n", forExpression->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
			return;
		}
		_INTEGER * integer = calloc(1, sizeof(_INTEGER));
		integer->value = forExpression->rangeStart->value;
		insertInteger(forExpression->id->idValue, integer);
		for(int i = forExpression->rangeStart->value; i < forExpression->rangeEnd->value; i++){
			integer->value = i;
			_generateMainExpressions(forExpression->mainExpressions);
			if(ERROR_OCCURED==true) return;
		}
		not_anidado = true;
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
		logError(_logger, "Cannot grow variables of type: %d\n", type);
		ERROR_OCCURED = true;
		*compi=FAILED;
		return;
	}
}

static void _generateForestAssignment(ForestAssignment * forestAssignment, char * forestId){
	if(ERROR_OCCURED==true) return;

	_FOREST * forest = getForest(forestId).value._forest;

	if(forestAssignment->type == ID_BY_VALUE){
		if(forestAssignment->value->type == INTEGERvalue){
			if(strcmp(forestAssignment->id->idValue, "start") == 0){
				forest->start = forestAssignment->value->intValue->value;
			}
			else if(strcmp(forestAssignment->id->idValue, "end") == 0){
				forest->end = forestAssignment->value->intValue->value;
			}
			else{
				logError(_logger, "Unknown forest attribute assignment by int value by name: %s\n", forestAssignment->id->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
				return;
		}
		else if(forestAssignment->value->type == IDvalue){
			EntryType type = getType(forestAssignment->value->idValue->idValue);
			if(type == EMPTY_TYPE){
				logError(_logger, "Nonexistent variable: %s\n", forestAssignment->value->idValue->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
			else if(type == INTEGER_TYPE){
				if(strcmp(forestAssignment->id->idValue, "start") == 0){
					forest->start = getInteger(forestAssignment->value->idValue->idValue).value._integer->value;
				}
				else if(strcmp(forestAssignment->id->idValue, "end") == 0){
					forest->end = getInteger(forestAssignment->value->idValue->idValue).value._integer->value;
				}
				else{
					logError(_logger, "Unknown forest attribute assignment by int value by name: %s\n", forestAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;
			}
			else{
				logError(_logger, "Unknown id type for forest assignment: %d\n", type);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(forestAssignment->value->type == DECLARATIONvalue){
			ForestAssignment * aux = calloc(1, sizeof(ForestAssignment));
			aux->id = forestAssignment->id;
			aux->type = forestAssignment->type;
			aux->value = forestAssignment->value->declareValue;
			_generateForestAssignment(aux, forestId);
			free(aux);
			return;
		}
		else if(forestAssignment->value->type == ATTvalue){
			if(forestAssignment->value->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(forestAssignment->value->attValue->attribute->idValue, "height") == 0){
					if(strcmp(forestAssignment->id->idValue, "start") == 0){
						forest->start = world->height;
					}
					else if(strcmp(forestAssignment->id->idValue, "end") == 0){
						forest->end = world->height;
					}
				}
				else if(strcmp(forestAssignment->value->attValue->attribute->idValue, "width") == 0){
					if(strcmp(forestAssignment->id->idValue, "start") == 0){
						forest->start = world->width;
					}
					else if(strcmp(forestAssignment->id->idValue, "end") == 0){
						forest->end = world->width;
					}
				}
				else{
					logError(_logger, "Unkown world attribute of int type: %s\n", forestAssignment->value->attValue->attribute->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;	
			}
			else if(forestAssignment->value->attValue->type == IDatt){
				EntryType type = getType(forestAssignment->value->attValue->variableID->idValue);
				if(type == EMPTY_TYPE){
					logError(_logger, "Nonexistent variable: %s\n", forestAssignment->value->attValue->variableID->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
					return;
				}

				if((strcmp(forestAssignment->id->idValue, "start") == 0) || (strcmp(forestAssignment->id->idValue, "end") == 0)){
					if(type == TREE_TYPE){
						_TREE * tree = getTree(forestAssignment->value->attValue->variableID->idValue).value._tree;
						if(strcmp(forestAssignment->value->attValue->attributeID->idValue, "x") == 0){
							if(strcmp(forestAssignment->id->idValue, "start") == 0){
								forest->start = tree->x;
							}
							else if(strcmp(forestAssignment->id->idValue, "end") == 0){
								forest->end = tree->x;
							}							
						}
						else if(strcmp(forestAssignment->value->attValue->attributeID->idValue, "height") == 0){
							if(strcmp(forestAssignment->id->idValue, "start") == 0){
								forest->start = tree->height;
							}
							else if(strcmp(forestAssignment->id->idValue, "end") == 0){
								forest->end = tree->height;
							}							
						}
						else if(strcmp(forestAssignment->value->attValue->attributeID->idValue, "depth") == 0){
							if(strcmp(forestAssignment->id->idValue, "start") == 0){
								forest->start = tree->depth;
							}
							else if(strcmp(forestAssignment->id->idValue, "end") == 0){
								forest->end = tree->depth;
							}							
						}
						else if(strcmp(forestAssignment->value->attValue->attributeID->idValue, "density") == 0){
							if(strcmp(forestAssignment->id->idValue, "start") == 0){
								forest->start = tree->density;
							}
							else if(strcmp(forestAssignment->id->idValue, "end") == 0){
								forest->end = tree->density;
							}							
						}
						else if(strcmp(forestAssignment->value->attValue->attributeID->idValue, "bark") == 0){
							if(strcmp(forestAssignment->id->idValue, "start") == 0){
								forest->start = tree->bark;
							}
							else if(strcmp(forestAssignment->id->idValue, "end") == 0){
								forest->end = tree->bark;
							}							
						}
						else{
							logError(_logger, "Unkown tree attribute of int type: %s\n", forestAssignment->value->attValue->attributeID->idValue);
							ERROR_OCCURED = true;
							*compi=FAILED;
						}
						return;
					}
					else if(type == FOREST_TYPE){
						_FOREST * forest2 = getForest(forestAssignment->value->attValue->variableID->idValue).value._forest;
						if(strcmp(forestAssignment->value->attValue->attributeID->idValue, "start") == 0){
							if(strcmp(forestAssignment->id->idValue, "start") == 0){
								forest->start = forest2->start;
							}
							else if(strcmp(forestAssignment->id->idValue, "end") == 0){
								forest->end = forest2->start;
							}							
						}
						else if(strcmp(forestAssignment->value->attValue->attributeID->idValue, "end") == 0){
							if(strcmp(forestAssignment->id->idValue, "start") == 0){
								forest->start = forest2->end;
							}
							else if(strcmp(forestAssignment->id->idValue, "end") == 0){
								forest->end = forest2->end;
							}							
						}
						else{
							logError(_logger, "Unkown forest attribute of int type: %s\n", forestAssignment->value->attValue->attributeID->idValue);
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
					logError(_logger, "Unknown forestAttribute: %s\n", forestAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;
			}
			else{
				logError(_logger, "Unknown attributeValueType: %d\n", forestAssignment->value->attValue->type);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
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
		if(ERROR_OCCURED==true) return;

		if(strcmp(forestAssignment->id->idValue, "start") == 0){
			forest->start = op;
		}
		else if(strcmp(forestAssignment->id->idValue, "end") == 0){
			forest->end = op;
		}
		else{
			logError(_logger, "Unknown forest attribute assign by operation: name(%s)\n", forestAssignment->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		return;
	}
	else{
		logError(_logger, "Unknown AssignationType: %d\n", forestAssignment->type);
		ERROR_OCCURED = true;
		*compi=FAILED;
	}
	return;
}

static void _generateForestAssignments(ForestAssignments * forestAssignments, char * forestId){
	if(ERROR_OCCURED==true) return;
	if(forestAssignments->type == SIMPLE_fa){
		_generateForestAssignment(forestAssignments->singleForestAssignment, forestId);
	}
	else if(forestAssignments->type == MULTIPLE_fa){
		_generateForestAssignment(forestAssignments->multipleForestAssignment, forestId);
		_generateForestAssignments(forestAssignments->forestAssignments, forestId);
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
		if(exists(forestExpression->id->idValue)){
			logError(_logger, "Existent variable: %s\n", forestExpression->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		else{
			_FOREST * forest = calloc(1, sizeof(_FOREST));
			forest->start = DEFAULT_FOREST_START;
			forest->end = DEFAULT_FOREST_END;
			insertForest(forestExpression->id->idValue, forest);
		}
	}
	else if(forestExpression->type == FULL_f){
		if(exists(forestExpression->id->idValue)){
			logError(_logger, "Existent variable: %s\n", forestExpression->id->idValue);
			ERROR_OCCURED = true;
			*compi=FAILED;
		}
		else{
			_FOREST * forest = calloc(1, sizeof(_FOREST));
			forest->start = DEFAULT_FOREST_START;
			forest->end = DEFAULT_FOREST_END;
			insertForest(forestExpression->id->idValue, forest);
			_generateForestAssignments(forestExpression->forestAssignments, forestExpression->id->idValue);
		}
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
			}
			else if(strcmp(treeAssignment->id->idValue, "height") == 0){
				tree->height = treeAssignment->value->intValue->value;
			}
			else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
				tree->depth = treeAssignment->value->intValue->value;
			}
			else if(strcmp(treeAssignment->id->idValue, "density") == 0){
				tree->density = treeAssignment->value->intValue->value;
			}
			else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
				tree->bark = treeAssignment->value->intValue->value;
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
			}
			else{
				logError(_logger, "Unknown tree attribute assignment by boolean value by name: %s\n", treeAssignment->id->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(treeAssignment->value->type == IDvalue){
			EntryType type = getType(treeAssignment->value->idValue->idValue);
			if(type == EMPTY_TYPE){
				logError(_logger, "Nonexistent variable: %s\n", treeAssignment->value->idValue->idValue);
				ERROR_OCCURED = true;
				*compi=FAILED;
				return;
			}
			else if(type == INTEGER_TYPE){
				if(strcmp(treeAssignment->id->idValue, "x") == 0){
					tree->x = getInteger(treeAssignment->value->idValue->idValue).value._integer->value;
				}
				else if(strcmp(treeAssignment->id->idValue, "height") == 0){
					tree->height = getInteger(treeAssignment->value->idValue->idValue).value._integer->value;
				}
				else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
					tree->depth = getInteger(treeAssignment->value->idValue->idValue).value._integer->value;
				}
				else if(strcmp(treeAssignment->id->idValue, "density") == 0){
					tree->density = getInteger(treeAssignment->value->idValue->idValue).value._integer->value;
				}
				else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
					tree->bark = getInteger(treeAssignment->value->idValue->idValue).value._integer->value;
				}
				else{
					logError(_logger, "Unknown tree attribute assignment by int value by name: %s\n", treeAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;
			}
			else if(type == BOOL_TYPE){
				if(strcmp(treeAssignment->id->idValue, "snowed") == 0){
					tree->snowed = getBoolean(treeAssignment->value->idValue->idValue).value._boolean->value;
				}
				else{
					logError(_logger, "Unknown tree attribute assignment by boolean value by name: %s\n", treeAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;
			}
			else if(type == STRING_TYPE){
				if(strcmp(treeAssignment->id->idValue, "leaf") == 0 && strlen(treeAssignment->value->charValue->value) == 3){
					tree->leaf = getString(treeAssignment->value->idValue->idValue).value._string->value[1];
				}
				else{
					logError(_logger, "Unknown tree attribute assignment by char value by name: %s\n", treeAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;
			}
			else if(type == HEXCOLOR_TYPE){
				if(strcmp(treeAssignment->id->idValue, "color") == 0){
					tree->color = getHexcolor(treeAssignment->value->idValue->idValue).value._hexcolor->value;
				}
				else{
					logError(_logger, "Unknown tree attribute assignment by hexcolor value by name: %s\n", treeAssignment->id->idValue);
					ERROR_OCCURED = true;
					*compi=FAILED;
				}
				return;
			}
			else{
				logError(_logger, "Unknown id type for tree assignment: %d\n", type);
				ERROR_OCCURED = true;
				*compi=FAILED;
			}
			return;
		}
		else if(treeAssignment->value->type == DECLARATIONvalue){
			TreeAssignment * aux = calloc(1, sizeof(TreeAssignment));
			aux->id = treeAssignment->id;
			aux->type = treeAssignment->type;
			aux->value = treeAssignment->value->declareValue;
			_generateTreeAssignment(aux, treeId);
			free(aux);
			return;
		}
		else if(treeAssignment->value->type == ATTvalue){
			if(treeAssignment->value->attValue->type == WORLDatt){
				_WORLD * world = getWorld("world").value._world;
				if(strcmp(treeAssignment->value->attValue->attribute->idValue, "height") == 0){
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
				}
				else if(strcmp(treeAssignment->value->attValue->attribute->idValue, "width") == 0){
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
				}
				else if(strcmp(treeAssignment->value->attValue->attribute->idValue, "uneveness") == 0){
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
				}
				else{
					logError(_logger, "Unkown world attribute of int type: %s\n", treeAssignment->value->attValue->attribute->idValue);
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
						_TREE * tree2 = getTree(treeAssignment->value->attValue->variableID->idValue).value._tree;
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
		}
		else if(strcmp(treeAssignment->id->idValue, "height") == 0){
			tree->height = op;
		}
		else if(strcmp(treeAssignment->id->idValue, "depth") == 0){
			tree->depth = op;
		}
		else if(strcmp(treeAssignment->id->idValue, "density") == 0){
			tree->density = op;
		}
		else if(strcmp(treeAssignment->id->idValue, "bark") == 0){
			tree->bark = op;
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
			}
			else if(strcmp(worldAssignment->id->idValue, "width") == 0){
				world->width = worldAssignment->value->intValue->value;
			}
			else if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
				world->uneveness = worldAssignment->value->intValue->value;
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
			aux->type = worldAssignment->type;
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
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "width") == 0){
						world->height = world->width;
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "uneveness") == 0){
						world->height = world->uneveness;
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
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "width") == 0){
						world->width = world->width;
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "uneveness") == 0){
						world->width = world->uneveness;
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
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "width") == 0){
						world->uneveness = world->width;
					}
					else if(strcmp(worldAssignment->value->attValue->attribute->idValue, "uneveness") == 0){
						world->uneveness = world->uneveness;
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
						_TREE * tree = getTree(worldAssignment->value->attValue->variableID->idValue).value._tree;
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

		}
		else if(strcmp(worldAssignment->id->idValue, "width") == 0){
			world->width = op;
		}
		else if(strcmp(worldAssignment->id->idValue, "uneveness") == 0){
			world->uneveness = op;
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
