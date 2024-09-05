

symbols = [
"DIGIT",
"INTEGER",
"IDENTIFIER",
"STRING",
"COMMENT",
"WHITESPACE",
"ASSIGN",
"COLON",
"COMMA",
"CROSS",
"DASH",
"EQUALS",
"GREATER",
"GREATEREQ",
"LCURLY",
"LESS",
"LESSEQ",
"LPAREN",
"NOT",
"REF",
"NOTEQUALS",
"POSTDEC",
"POSTINC",
"RCURLY",
"RPAREN",
"SEMICOL",
"SLASH",
"STAR",
"ARROW",
]



for s in symbols:
    print(fr"""{s} {{
            int tokenKind = TokenKind::{s};
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }}
    """)