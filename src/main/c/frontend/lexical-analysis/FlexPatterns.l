%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"->"                                {return ArrowLexemeAction(createLexicalAnalyzerContext());}

"="                                 {return EqualLexemeAction(createLexicalAnalyzerContext());}
"<"                                 {return LesserThanLexemeAction(createLexicalAnalyzerContext());}
"<="                                {return LesserEqualLexemeAction(createLexicalAnalyzerContext());}
">"                                 {return GreaterThanLexemeAction(createLexicalAnalyzerContext());}
">="                                {return GreaterEqualLexemeAction(createLexicalAnalyzerContext());}

","                                 {return CommaLexemeAction(reateLexicalAnalyzerContext());}
";"                                 {return SemicolonLexemeAction(createLexicalAnalyzerContext());}

"-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

"+="                                {return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD_EQ);}
"-="                                {return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB_EQ);}
"*="                                {return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL_EQ);}

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

"{"                                 {return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_CURLY_BRACE);}
"}"                                 {return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_CURLY_BRACE);}

"["                                 {return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE);}
"]"                                 {return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE);}

"for"                               {return ForLexemeAction(createLexicalAnalyzerContext());}
"in"                                {return InLexemeAction(createLexicalAnalyzerContext());}
"while"                             {return WhileLexemeAction(createLexicalAnalyzerContext());}
"with"                              {return WithLexemeAction(createLexicalAnalyzerContext());}
"if"                                {return ConditionalLexemeAction(createLexicalAnalyzerContext(), IF);}
"else"                              {return ConditionalLexemeAction(createLexicalAnalyzerContext(), ELSE);}

"world"                             {return WorldLexemeAction(createLexicalAnalyzerContext());}
"tree"                              {return TreeLexemeAction(createLexicalAnalyzerContext());}
"forest"                            {return ForestLexemeAction(createLexicalAnalyzerContext());}
"grow"                              {return GrowLexemeAction(createLexicalAnalyzerContext());}

"'"[[:alnum:]]+"'"                  {return StringLexemeAction(createLexicalAnalyzerContext());}

[[:alpha:][:alnum:]*]+              {return IdLexemeAction(createLexicalAnalyzerContext());}

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
