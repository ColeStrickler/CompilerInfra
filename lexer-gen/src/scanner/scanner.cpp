#include "scanner.hpp"

RegexScanner::RegexScanner(const std::string &regex) : m_Regex(regex), m_CurrIndex(0), m_PrevCharacter(0x0)
{
}

void RegexScanner::ScanRegex()
{
    while (!AtEnd())
    {
        char c = ConsumeChar();

        if (c >= '0' && c <= '9')
        {

        }
        else if (c >= 'a' && c <= 'z')
        {

        }
        else if (c >= 'A' && c <= 'Z')
        {

        }
        else if (c == '|')
        {

        }
        else if (c == '[' || c == ']')
        {

        }
        else if (c == '(' || c == ')')
        {

        }
        else if (c == '+' || c == '*' || c == '*')
        {

        }
        else if (c == '^' || c == '\\')
        {
            
        }





    }
}





bool RegexScanner::AtEnd()
{
    return m_CurrIndex >= m_Regex.size();
}

char RegexScanner::ConsumeChar()
{
    if (AtEnd())
    {
        m_ErrorCode = ERROR::CONSUME_AT_END;
        return 0x0;
    }

    return m_Regex[m_CurrIndex++];
}

