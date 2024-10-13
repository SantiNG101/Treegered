#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Program * ExpressionProgramSemanticAction(CompilerState * compilerState, ProgramExpression * expression);

DeclarationValue * DeclarationValueIDSemanticAction(Id id);
DeclarationValue * DeclarationValueStringSemanticAction(char* string);
DeclarationValue * DeclarationValueBooleanSemanticAction(boolean value);
DeclarationValue * DeclarationValueHexSemanticAction(Hexcolor color);
DeclarationValue * DeclarationValueIntSemanticAction(int integer);

ArithmeticOperation * ArithmeticOperationAllDeclarationValueSemanticAction(OperatorType operator, DeclarationValue* left, DeclarationValue* right, ArithmeticOperationType type);
ArithmeticOperation * ArithmeticOperatioAllnSemanticAction(OperatorType operator, ArithmeticOperation* left, ArithmeticOperation* right, ArithmeticOperationType type);
ArithmeticOperation * ArithmeticOperationLeftDeclarationRightSemanticAction(OperatorType operator, DeclarationValue* left, ArithmeticOperation* right, ArithmeticOperationType type);
ArithmeticOperation * ArithmeticOperationRightDeclarationLeftSemanticAction(OperatorType operator, ArithmeticOperation* left, DeclarationValue* right, ArithmeticOperationType type);

ProgramExpression * WorldProgramExpressionSemanticAction(MainExpression * mainExpression, ProgramType type);
ProgramExpression * WorldlessProgramExpressionSemanticAction(MainExpression * mainExpression, WorldExpression* worldExpression, ProgramType type);

WorldAssignment * WorldAssignmentDeclarationSemanticAction(Id id, DeclarationValue* value, AssignationType type);
WorldAssignment * WorldAssignmentArithmeticSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type);

WorldAssignments * SimpleWorldAssignmentsSemanticAction(WorldAssignment * worldAssignment, WorldType wType);
WorldAssignments * MultipleWorldAssignmentsSemanticAction(WorldAssignments * worldAssignments, WorldAssignment * worldAssignment, WorldType wType);

WorldExpression * WorldExpressionSemanticAction(WorldAssignments* worldAssignments);

ForestAssignment * DeclarationForestAssignmentSemanticAction(Id id, DeclarationValue* value, AssignationType type);
ForestAssignment * ArithmeticForestAssignmentSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type);

ForestAssignments * SimpleForestAssignmentsSemanticAction(ForestAssignment * forestAssignment, ForestAssignType type);
ForestAssignments * MultipleForestAssignmentsSemanticAction(ForestAssignments * forestAssignments, ForestAssignment * forestAssignment, ForestAssignType type);

ForestExpression * ForestExpressionSemanticAction(Id id, ForestAssignments * forestAssignments, ForestType type);
ForestExpression * DefaultForestExpressionSemanticAction(Id id, ForestType type);

TreeAssignment * DeclarationTreeAssignmentSemanticAction(Id id, DeclarationValue* value, AssignationType type);
TreeAssignment * ArithmeticTreeAssignmentSemanticAction(Id id, ArithmeticOperation* operation, AssignationType type);

TreeAssignments * SimpleTreeAssignmentsSemanticAction(TreeAssignment * treeAssignment, TreeAssignType type);
TreeAssignments * MultipleTreeAssignmentsSemanticAction(TreeAssignments * treeAssignments, TreeAssignment * treeAssignment, TreeAssignType type);

TreeExpression * TreeExpressionSemanticAction(Id id, TreeAssignments * treeAssignments, TreeType type);
TreeExpression * DefaultTreeExpressionSemanticAction(Id id, TreeType type);

GrowExpression * GrowExpressionSemanticAction(Id id);

ForExpression * ForExpressionSemanticAction(Id id, int start, int end, MainExpressions * mainExpressions);

ArithmeticAssignation * ArithmeticDeclarationAssignationSemanticAction(Id id, OperatorType operator, DeclarationValue * value, AssignationType type);
ArithmeticAssignation * ArithmeticOperationAssignationSemanticAction(Id id, OperatorType operator, ArithmeticOperation * arithOp, AssignationType type);

GeneralAssignation * GeneralDeclarationAssignationSemanticAction(Id classType, Id id, DeclarationValue * value, AssignationType type);
GeneralAssignation * GeneralArithmeticOperationAssignationSemanticAction(Id classType, Id id, ArithmeticOperation * arithOp, AssignationType type);

MainExpressions * SimpleMainExpressionSemanticAction(MainExpression * mainExpression, ExpressionType type);
MainExpressions * MultipleMainExpressionSemanticAction(MainExpressions* mainExpressions, MainExpression * mainExpression, ExpressionType type);

MainExpression * MainExpressionTreeSemanticAction(TreeExpression * treeExpression, MainExpressionType type);
MainExpression * MainExpressionForestSemanticAction(ForestExpression * forestExpression, MainExpressionType type);
MainExpression * MainExpressionGrowSemanticAction(GrowExpression * growExpression, MainExpressionType type);
MainExpression * MainExpressionForSemanticAction(ForExpression * forExpression, MainExpressionType type);
MainExpression * MainExpressionArithmeticAssignationSemanticAction(ArithmeticAssignation * arithmeticAssignation, MainExpressionType type);
MainExpression * MainExpressionGeneralSemanticAction(GeneralAssignation * generalAssignation, MainExpressionType type);

#endif
