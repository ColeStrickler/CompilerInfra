#include "regex_compiler.h"



RegexCompiler::RegexCompiler() : RegexScanner(), m_Error(REGEX_COMPILER_ERROR::NO_ERROR_COMPILER)
{
}

RegexCompiler::~RegexCompiler()
{
}

std::vector<Token> RegexCompiler::TokenizeRegEx(const std::string &regex)
{
    m_Scanner.ScannerReset();
    m_Scanner.AddRegExString(regex);
    if (!m_Scanner.ScanRegex())
    {
        m_Error = REGEX_COMPILER_ERROR::SCANNER_ERROR;
        return {};
    }
    return m_Scanner.GetTokens(); 
}

NFA *RegexCompiler::CompileRegEx(const std::string &regex)
{
    auto tokens = TokenizeRegEx(regex);
    if (tokens.size() == 0)
    {
        m_Error = REGEX_COMPILER_ERROR::SCANNER_ERROR;
        return nullptr;
    }

    auto parseTree = ParseRegEx(tokens);
    if (parseTree == nullptr)
    {
        m_Error = REGEX_COMPILER_ERROR::PARSER_ERROR;
        return nullptr;
    }

    printf("Parse successful!\n");
        

    return nullptr;
    /*
        Translate will use syntax directed translation to combine the smaller NFAs into one big NFA
    */
    //return parseTree->Translate();
}

RegExprNode *RegexCompiler::ParseRegEx(std::vector<Token> tokens)
{
    m_Parser.AddTokens(tokens);
    return m_Parser.Parse();
}

std::string RegexCompiler::GetErrorString()
{
    switch (m_Error)
    {
        case REGEX_COMPILER_ERROR::NO_ERROR_COMPILER: return "REGEX_COMPILER_ERROR::NO_ERROR_COMPILER\n";
        case REGEX_COMPILER_ERROR::SCANNER_ERROR: return "REGEX_COMPILER_ERROR::SCANNER_ERROR\n" + m_Scanner.GetErrorString();
        case REGEX_COMPILER_ERROR::PARSER_ERROR: return "REGEX_COMPILER_ERROR::PARSER_ERROR\n" + m_Parser.GetError(); // add actual error in future
        default:
        {
            return "Unknown error code.\n";
        }
    }
}