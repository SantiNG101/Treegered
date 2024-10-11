#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);

Token UnknownLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

// Our functions
Token ArrowLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token EqualLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token BraceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token SemicolonLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token CurlyBraceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token LesserThanLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token GreaterThanLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token TreeLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token WithLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token ForestLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token InLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token ForLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token ConditionalLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token GrowLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token IdLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token BooleanLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token HexcolorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token CommaLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token GreaterEqualLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token LesserEqualLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token StringLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token WorldLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

#endif
