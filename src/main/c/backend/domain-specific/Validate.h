#ifndef VALIDATE_HEADER
#define VALIDATE_HEADER

/**
 * We reuse the types from the AST for convenience, but you should separate
 * the layers of the backend and frontend using another group of
 * domain-specific models or DTOs (Data Transfer Objects).
 */
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include <limits.h>

/** Initialize module's internal state. */
void initializeValidateModule();

/** Shutdown module's internal state. */
void shutdownValidateModule();

/**
 * The result of a computation. It's considered valid only if "succeed" is
 * true.
 */
typedef struct {
	boolean succeed;
	int value;
} ComputationResult;

/** Arithmetic operations. */

ComputationResult add(const int leftAddend, const int rightAddend);
ComputationResult divide(const int dividend, const int divisor);
ComputationResult multiply(const int multiplicand, const int multiplier);
ComputationResult subtract(const int minuend, const int subtract);

/**
 * Computes the full treegered program input.
 */
ComputationResult computeProgramExpression(ProgramExpression * programExpression);

ComputationResult computeWorldExpression(WorldExpression * worldExpression);
ComputationResult computeWorldAssignments(WorldAssignments * worldAssignments);
ComputationResult computeWorldAssignment(WorldAssignment * worldAssignment);

ComputationResult computeMainExpressions(MainExpressions * mainExpressions);
ComputationResult computeMainExpression(MainExpression * mainExpression);

ComputationResult computeTreeExpression(TreeExpression * treeExpression);
ComputationResult computeTreeAssignments(TreeAssignments * treeAssignments);
ComputationResult computeTreeAssignment(TreeAssignment * treeAssignment);

ComputationResult computeForestExpression(ForestExpression * forestExpression);
ComputationResult computeForestAssignments(ForestAssignments * forestAssignments);
ComputationResult computeForestAssignment(ForestAssignment * forestAssignment);

ComputationResult computeGrowExpression(GrowExpression * growExpression);

ComputationResult computeForExpression(ForExpression * forExpression);

ComputationResult computeArithmeticAssignation(ArithmeticAssignation * arithmeticAssignation);
ComputationResult computeArithmeticOperation(ArithmeticOperation * arithmeticOperation);

ComputationResult computeGeneralAssignation(GeneralAssignation * generalAssignation);

ComputationResult computeConditionalExpression(ConditionalExpression * conditionalExpression);
ComputationResult computeConditionalClause(ConditionalClause * conditionalClause);

ComputationResult computeAttributeValue(AttributeValue * attributeValue);
ComputationResult computeDeclarationValue(DeclarationValue * declarationValue);

#endif