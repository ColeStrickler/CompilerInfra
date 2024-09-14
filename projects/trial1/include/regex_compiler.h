#ifndef REGEX_COMPILER_H
#define REGEX_COMPILER_H
#include "regex_parser.h"


class RegexCompiler : public RegexScanner
{
public:
    RegexCompiler();
    ~RegexCompiler();
    
    std::vector<Token> TokenizeRegEx(const std::string& regex);
    NFA* CompileRegEx(const std::string& regex);
    RegExprNode* ParseRegEx(std::vector<Token> tokens);


    std::string GetErrorString();
    /*
        We will get this to return a DFA

        void Parse(std::vector<Token> tokens);
    */

private:
    
    RegexScanner m_Scanner;
    RegexParser m_Parser;
    REGEX_COMPILER_ERROR m_Error;

};




#endif