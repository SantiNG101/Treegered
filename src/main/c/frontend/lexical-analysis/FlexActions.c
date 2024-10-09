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

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	Token token;
	switch (lexicalAnalyzerContext->lexeme[0])
	{
	case '+':
		token = ADD;
		break;
	case '-':
		token = SUB;
		break;
	case '*':
		token = MUL;
		break;
	case '/':
		token = DIV;
		break;
	}
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
	return INTEGER;
}

Token ParenthesisLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	Token token;
	switch (lexicalAnalyzerContext->lexeme[0])
	{
	case '(':
		token = OPEN_PARENTHESIS;
		break;
	case ')':
		token = CLOSE_PARENTHESIS;
		break;
	}
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token UnknownLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return UNKNOWN;
}

Token ArrowLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = ARROW;
	return ARROW;
}

Token EqualLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = EQUAL;
	return EQUAL;
}

Token BraceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	Token token;
	switch (lexicalAnalyzerContext->lexeme[0])
	{
	case '[':
		token = OPEN_BRACE;
		break;
	case ']':
		token = CLOSE_BRACE;
		break;
	}
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token SemicolonLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = SEMICOLON;
	return SEMICOLON;
}

Token CurlyBraceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	Token token;
	switch (lexicalAnalyzerContext->lexeme[0])
	{
	case '{':
		token = OPEN_CURLY;
		break;
	case '}':
		token = CLOSE_CURLY;
		break;
	}
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token LesserThanLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = LESSERTHAN;
	return LESSERTHAN;
}

Token GreaterThanLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = GREATERTHAN;
	return GREATERTHAN;
}

Token TreeLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = TREE;
	return TREE;
}

Token WithLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = WITH;
	return WITH;
}

Token ForestLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = FOREST;
	return FOREST;
}

Token InLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = IN;
	return IN;
}

Token ForLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = FOR;
	return FOR;
}

Token ConditionalLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	Token token;
	if (strcmp(lexicalAnalyzerContext->lexeme, "if") == 0)
		token = IF;
	if (strcmp(lexicalAnalyzerContext->lexeme, "else") == 0)
		token = ELSE;

	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token WhileLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = WHILE;
	return WHILE;
}

Token GrowLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = GROW;
	return GROW;
}

Token IdLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->id = lexicalAnalyzerContext->lexeme;
	return ID;
}

Token CommaLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = COMMA;
	return COMMA;
}

Token GreaterEqualLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = GREATER_EQUAL;
	return GREATER_EQUAL;
}

Token LesserEqualLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->token = LESSER_EQUAL;
	return LESSER_EQUAL;
}

Token StringLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	lexicalAnalyzerContext->semanticValue->string = lexicalAnalyzerContext->lexeme;
	return STRING;
}