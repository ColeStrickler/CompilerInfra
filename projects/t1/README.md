Please see "RegularExpressionGrammar" for legally composable regular expressions.

Bracket expressions must first include any negations, followed by any ranged expressions at the beginning. 

There is a bug in the grammar that I haven't had time to fix yet, and some times you have to force precedence with parentheses as is the case
in the STRING token example for the spec.l file.


Special characters include:
\t ==> tab
\" ==> double quote
\\ ==> backslash literal
\n ==> newline literal
\_ ==> space literal
\+ ==> plus literal
\- ==> minus literal
. ==> any character


Edit spec.l to change the regular expressions. Edit input.test to alter the input that will be scanned. 


# To build the scanner generator header file
make run

# To build the test harness and test on the input.test file
make test