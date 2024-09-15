Please see "RegularExpressionGrammar" for legally composable regular expressions.

Bracket expressions must firsy include any negations, followed by any ranged expressions at the beginning. 

Special characters include:
\t ==> tab
\" ==> double quote
\\ ==> backslash literal
\n ==> newline literal
\_ ==> space literal
. ==> any character


Edit spec.l to change the regular expressions. Edit input.test to alter the input that will be scanned.


# To build the scanner generator header file
make run

# To build the test harness and test on the input.test file
make test