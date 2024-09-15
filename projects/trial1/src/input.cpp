#include "input.h"

ScannerInputSorter::ScannerInputSorter(const std::string &filename) : m_LineNo(1)
{
    std::ifstream file(filename);
    if (!file) {
        std::cout << "could not open " << filename << "\n";
        return;
    }
    m_FileStringStream << file.rdbuf();
}

ScannerInputSorter::~ScannerInputSorter()
{
}

std::vector<std::pair<std::string, TokenActionItem>> ScannerInputSorter::GetRegexes()
{
    return m_Regexes;
}

bool ScannerInputSorter::ParseLines()
{
    std::vector<std::string> lines;
    std::string line;
    while(std::getline(m_FileStringStream, line))
    {
        /*
            Split each line into --> <regex> <action>
        */
        auto strings = splitAtFirstSpace(line);
        if (strings.second.size() == 0)
        {
            std::cout << "No space found in line number " << m_LineNo << ".\n";
            return false;
        }
        
        TokenActionItem action;
        action.m_MetaData = strings.first; // may be overwritten based on type
        if (!GetTokenAction(&action, strings.second))
        {
            std::cout << "ScannerInputSorter::ParseLines() --> could not parse line number " << std::to_string(m_LineNo) + "\n";
            return false;
        }
        std::pair<std::string, TokenActionItem> regexStrings = {strings.first, action};

        m_Regexes.push_back(regexStrings);
        m_LineNo++;
    }
    return true;
}

bool ScannerInputSorter::GetTokenAction(TokenActionItem *out, std::string actions)
{
    
    auto split = splitAtFirstSpace(actions);
    if (split.second.size() == 0)
    {
        if (actions == "(SKIP)")
        {
            out->m_Action = INPUTACTION::SKIP;
            out->m_MetaData = "";
            out->m_TokenName = "SKIP";
            return true;
        }
        return false;
    }

    if (split.first == "(ERR)")
    {
        out->m_Action = INPUTACTION::ERROR;
        out->m_MetaData = split.second.substr(1, split.second.size()-2);
        out->m_TokenName = "ERROR";
        return true;
    }
    else if (split.second == "true" || split.second == "false")
    {
        out->m_Action = split.second == "true" ? INPUTACTION::STORE_METADATA : INPUTACTION::STORE_NO_METADATA;
        out->m_TokenName = split.first;
        return true;
    }

    return false;
}

std::pair<std::string, std::string> ScannerInputSorter::splitAtFirstSpace(const std::string &line)
{
    std::string::size_type pos = line.find(' ');
    
    if (pos == std::string::npos) {
        // No space found; return the original string as the first part, and an empty second part
        //std::cout << "No space found in line number " << m_LineNo << ".\n";
        return {line, ""};
    }
    
    std::string first = line.substr(0, pos);
    std::string second = line.substr(pos + 1);
    
    return {first, second};
}
