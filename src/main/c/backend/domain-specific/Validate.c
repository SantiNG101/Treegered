#include "Validate.h"
#include "../symbol-table/table.h"

/*MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeValidateModule() {
	_logger = createLogger("Validate");
    initTable();
}

void shutdownValidateModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
        freeTable();
	}
}

/** PRIVATE FUNCTIONS */

static ComputationResult _invalidComputation();

/**
 * A computation that always returns an invalid result.
 */
static ComputationResult _invalidComputation() {
	ComputationResult computationResult = {
		.succeed = false,
		.value = 0
	};
	return computationResult;
}

/** PUBLIC FUNCTIONS */
/** ARITH FUNCTIONS */
/*ComputationResult checkType(char* type, DeclarationValue * value){
    if(type == NULL || value == NULL) return _invalidComputation();
    switch (){
        //TODO
    }
}*/

/** ARITH FUNCTIONS */

ComputationResult add(const int leftAddend, const int rightAddend) {
	ComputationResult computationResult = {
		.succeed = true,
		.value = leftAddend + rightAddend
	};
	return computationResult;
}

ComputationResult divide(const int dividend, const int divisor) {
	const int sign = dividend < 0 ? -1 : +1;
	const boolean divisionByZero = divisor == 0 ? true : false;
	if (divisionByZero) {
		logError(_logger, "The divisor cannot be zero (the computation was %d/%d).", dividend, divisor);
	}
	ComputationResult computationResult = {
		.succeed = divisionByZero ? false : true,
		.value = divisionByZero ? (sign * INT_MAX) : (dividend / divisor)
	};
	return computationResult;
}

ComputationResult multiply(const int multiplicand, const int multiplier) {
	ComputationResult computationResult = {
		.succeed = true,
		.value = multiplicand * multiplier
	};
	return computationResult;
}

ComputationResult subtract(const int minuend, const int subtract) {
	ComputationResult computationResult = {
		.succeed = true,
		.value = minuend - subtract
	};
	return computationResult;
}

/** MAIN FUNCTIONS */

ComputationResult computeGeneralAssignation(GeneralAssignation * generalAssignation){
    switch (generalAssignation->type){
        case ID_BY_VALUE_DECLARE://TODO sacar hardcodeado a int
            //ComputationResult check = checkType(generalAssignation->classType, generalAssignation->value);
            int res = addToTable(generalAssignation->idDeclared->idValue, INTCLASS, generalAssignation->value->intValue->value);
            if(res == true){
                ComputationResult aux = _invalidComputation();
                aux.succeed = true;
                return aux;
            }
		    if(res == ERROR_MAP) logError(_logger, "%s already exists!\n", generalAssignation->idDeclared->idValue);
		    if(res != true) logError(_logger, "Error declaring %s\n", generalAssignation->idDeclared->idValue);
            return _invalidComputation();
        case ID_BY_OPP_DECLARE:
        case ID_BY_VALUE://TODO ALL
        case ID_BY_OPP:
        case ATT_BY_VALUE:
        case ATT_BY_OPP:
        default:
            logError(_logger, "Unknown AssignationType: %d\n", generalAssignation->type);
            return _invalidComputation();
    }
}

ComputationResult computeMainExpression(MainExpression * mainExpression){
    switch (mainExpression->type){
        case TREE_m://TODO ALL
        case FOREST_m:
        case GROW_m:
        case FOR_m:
        case ARITHMETIC_m:
        case GENERAL_ASSIGNATION_m:
            return computeGeneralAssignation(mainExpression->generalAssignation);
        case CONDITIONAL_m:
        default:
            logError(_logger, "Unknown MainExpressionType: %d\n", mainExpression->type);
            return _invalidComputation();
    }
}

ComputationResult computeMainExpressions(MainExpressions * mainExpressions){
    switch (mainExpressions->type){
        case SIMPLE_e:
            return computeMainExpression(mainExpressions->singleMainExpression);
        case MULTIPLE_e:
            ComputationResult mainExp = computeMainExpression(mainExpressions->multipleMainExpression);
            ComputationResult mainExps = computeMainExpressions(mainExpressions->mainExpressions);
            if(mainExp.succeed && mainExps.succeed) return mainExp;
			return _invalidComputation();
        default:
            logError(_logger, "Unknown ExpressionType: %d\n", mainExpressions->type);
			return _invalidComputation();
    }
}

ComputationResult computeWorldAssignment(WorldAssignment * worldAssignment){
//TODO
}

ComputationResult computeWorldAssignments(WorldAssignments * worldAssignments){
    switch (worldAssignments->wType){
        case SIMPLE_w:
            return computeWorldAssignment(worldAssignments->singleWorldAssignment);
        case MULTIPLE_w:
            ComputationResult worldAssign = computeWorldAssignment(worldAssignments->multipleWorldAssignment);
            ComputationResult worldAssigns = computeWorldAssignments(worldAssignments->worldAssignments);
            if(worldAssign.succeed && worldAssigns.succeed) return worldAssign;
			return _invalidComputation();
        default:
            logError(_logger, "Unknown WorldType: %d\n", worldAssignments->wType);
			return _invalidComputation();
    }
}

ComputationResult computeWorldExpression(WorldExpression * worldExpression){
    return computeWorldAssignments(worldExpression->worldAssignments);
}

ComputationResult computeProgramExpression(ProgramExpression * programExpression) {
	switch (programExpression->type) {
		case WORLDLESS:
            return computeMainExpressions(programExpression->mainExpressions);
		case WORLDFULL:
			ComputationResult worldExpression = computeWorldExpression(programExpression->worldExpression);
			ComputationResult mainExpressions = computeMainExpressions(programExpression->mainExpressions);
			if(worldExpression.succeed && mainExpressions.succeed) return worldExpression;
			return _invalidComputation();
		default:
            logError(_logger, "Unknown ProgramType: %d\n", programExpression->type);
			return _invalidComputation();
	}
}