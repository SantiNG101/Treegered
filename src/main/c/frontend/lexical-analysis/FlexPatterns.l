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

"="                                 {return EqualLexemeAction(createLexicalAnalyzerContext());}

","                                 {return CommaLexemeAction(createLexicalAnalyzerContext());}
";"                                 {return SemicolonLexemeAction(createLexicalAnalyzerContext());}

"("									{return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
"{"                                 {return CurlyBraceLexemeAction(createLexicalAnalyzerContext(), OPEN_CURLY_BRACE);}
"}"                                 {return CurlyBraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_CURLY_BRACE);}

"with"                              {return WithLexemeAction(createLexicalAnalyzerContext());}

"world"                             {return WorldLexemeAction(createLexicalAnalyzerContext());}
"tree"                              {return TreeLexemeAction(createLexicalAnalyzerContext());}

"'"[[:alnum:]]+"'"                  {return StringLexemeAction(createLexicalAnalyzerContext());}

"true"                              {return BooleanLexemeAction(createLexicalAnalyzerContext(), TRUE);}
"TRUE"                              {return BooleanLexemeAction(createLexicalAnalyzerContext(), TRUE);}
"false"                             {return BooleanLexemeAction(createLexicalAnalyzerContext(), FALSE);}
"FALSE"                             {return BooleanLexemeAction(createLexicalAnalyzerContext(), FALSE);}

"#"([0-9A-Fa-f]{6})                 {return HexcolorLexemeAction(createLexicalAnalyzerContext());}

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:alpha:][:alnum:]*]+              {return IdLexemeAction(createLexicalAnalyzerContext());}

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }


%%

#include "FlexExport.h"
