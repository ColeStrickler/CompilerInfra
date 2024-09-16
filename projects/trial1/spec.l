[0-9] DIGIT true
[0-9]+ INTEGER true
[a-zA-Z_]([a-zA-Z0-9_])* IDENTIFIER true
\"((\\(n|t|\\|\"\\))|[a-zA-Z])*\" STRING true
\"(\\[nt\\"\\]|[^"\\])*[^\"\\] (ERR) "unterm string"
\"(\\[^nt\\"\\]|[^"\\])*\" (ERR) "bad escape in string literal"
\"(\\[^nt\\"\\]|[^"\\])*[^\"\\] (ERR) "bad escape and unterm string literal"
#.* (SKIP)
\_ (SKIP)
\n (SKIP)
= ASSIGN true
: COLON true
, COMMA true
\+ CROSS true
\- DASH true
== EQUALS true
> GREATER true
>= GREATEREQ true
{ LBRACKET true
< LESS true
<= LESSEQ true
\( LPAREN true
! NOT true
& REF true
!= NOTEQUALS true
\-\- POSTDEC true
\+\+ POSINC true
} RBRACKET true
\) RPAREN true
; SEMICOLON true
/ SLASH true
\* STAR true
\-> ARROW true