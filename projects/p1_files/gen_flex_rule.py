

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


def getToken(s):
    if s == "eh?":
        return "EH"
    else:
        return s.upper()


for s in keywords:
    print(f"{getToken(s)} {s}")



for s in keywords:
    print(fr"""{getToken(s)} {{
            int tokenKind = TokenKind::{getToken(s)};
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum += strlen(yytext);
            return tokenKind;
        }}
    """)