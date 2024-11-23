#include "Validate.h"
//#include "../symbol-table/table.h"

/*MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeValidateModule() {
	_logger = createLogger("Validate");
    //initTable();
}

void shutdownValidateModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
        //freeTable();
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
/** CHECK FUNCTIONS */

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

ComputationResult computeDeclarationValue(DeclarationValue * declarationValue){
    //TODO
}

ComputationResult computeAttributeValue(AttributeValue * attributeValue){
    //TODO
}

ComputationResult computeArithmeticOperation(ArithmeticOperation * arithmeticOperation){
    //TODO
}

ComputationResult computeConditionalClause(ConditionalClause * conditionalClause){
    //TODO
}

ComputationResult computeConditionalExpression(ConditionalExpression * conditionalExpression){
    //TODO
}

ComputationResult computeGeneralAssignation(GeneralAssignation * generalAssignation){
    switch (generalAssignation->type){
        case ID_BY_VALUE_TYPE://TODO
        case ID_BY_OPP_TYPE:
        case ID_BY_VALUE:
        case ID_BY_OPP:
        case ATT_BY_VALUE:
        case ATT_BY_OPP:
        default:
            logError(_logger, "Unknown AssignationType: %d\n", generalAssignation->type);
            return _invalidComputation();
    }
}

ComputationResult computeArithmeticAssignation(ArithmeticAssignation * arithmeticAssignation){
    //TODO
}

ComputationResult computeForExpression(ForExpression * forExpression){
    //TODO
}

ComputationResult computeGrowExpression(GrowExpression * growExpression){
    //TODO
}

ComputationResult computeForestAssignment(ForestAssignment * forestAssignment){
    //TODO
}

ComputationResult computeForestAssignments(ForestAssignments * forestAssignments){
    //TODO
}

ComputationResult computeForestExpression(ForestExpression * forestExpression){
    //TODO
}

ComputationResult computeTreeAssignment(TreeAssignment * treeAssignment){
    //TODO
}

ComputationResult computeTreeAssignments(TreeAssignments * treeAssignments){
    //TODO
}

ComputationResult computeTreeExpression(TreeExpression * treeExpression){
    //TODO
}

ComputationResult computeMainExpression(MainExpression * mainExpression){
    switch (mainExpression->type){
        case TREE_m:
            return computeTreeExpression(mainExpression->treeExpression);
        case FOREST_m:
            return computeForestExpression(mainExpression->forestExpression);
        case GROW_m:
            return computeGrowExpression(mainExpression->growExpression);
        case FOR_m:
            return computeForExpression(mainExpression->forExpression);
        case ARITHMETIC_m:r
            return computeArithmeticAssignation(mainExpression->arithmeticAssignation);
        case GENERAL_ASSIGNATION_m:
            return computeGeneralAssignation(mainExpression->generalAssignation);
        case CONDITIONAL_m:
            return computeConditionalExpression(mainExpression->conditionalExpression);
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
    switch (worldAssignment->type){
        case ID_BY_VALUE:
            //TODO check que el ID sea de un worldAtt, luego sobrescribir en la tabla de simbolos con el valor
        case ID_BY_OPP:
            //TODO check que el ID sea de un worldAtt, luego sobrescribir en la tabla de simbolos con el valor
        default:
            logError(_logger, "Unknown AssignationType: %d\n", worldAssignment->type);
			return _invalidComputation();
    }
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
            return computeMainExpressions(programExpression->worldlessMainExpressions);
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