#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule()
{
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule()
{
	if (_logger != NULL)
	{
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char *functionName, LexicalAnalyzerContext *lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char *functionName, LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	char *escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
				 functionName,
				 escapedLexeme,
				 lexicalAnalyzerContext->currentContext,
				 lexicalAnalyzerContext->length,
				 lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	if (_logIgnoredLexemes)
	{
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	if (_logIgnoredLexemes)
	{
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void IgnoredLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	if (_logIgnoredLexemes)
	{
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token UnknownLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return UNKNOWN;
}

/*ADDED*/

Token WorldLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = WORLD;
	return WORLD;
}

Token TreeLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = TREE;
    return TREE;
}

Token ForestLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = FOREST;
    return FOREST;
}

Token GrowLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = GROW;
    return GROW;
}


Token ArrowLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = ARROW;
    return ARROW;
}

Token EqualLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = EQUAL;
    return EQUAL;
}


Token CommaLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = COMMA;
    return COMMA;
}

Token SemicolonLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = SEMICOLON;
	return SEMICOLON;
}

Token ArithmeticOperatorLexemeAction(
    LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token ParenthesisLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext,
                              Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token CurlyBraceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext,
                             Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token BraceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext,
                        Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token ForLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = FOR;
    return FOR;
}

Token InLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = IN;
    return IN;
}

Token WithLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = WITH;
    return WITH;
}

Token StringLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->string = lexicalAnalyzerContext->lexeme;
	return STRING;
}

Token IdLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->id = lexicalAnalyzerContext->lexeme;
	return ID;
}

Token BooleanLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext,
                             Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token HexcolorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->id = lexicalAnalyzerContext->lexeme;
    return HEXCOLOR;
}

Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
	return INTEGER;
}