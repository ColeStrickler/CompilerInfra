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

std::vector<std::pair<std::string, std::string>> ScannerInputSorter::GetRegexes()
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
            return false;
        }
        m_Regexes.push_back(strings);
        m_LineNo++;
    }
    return true;
}

std::pair<std::string, std::string> ScannerInputSorter::splitAtFirstSpace(const std::string &line)
{
    std::string::size_type pos = line.find(' ');
    
    if (pos == std::string::npos) {
        // No space found; return the original string as the first part, and an empty second part
        std::cout << "No space found in line number " << m_LineNo << ".\n";
        return {line, ""};
    }
    
    std::string first = line.substr(0, pos);
    std::string second = line.substr(pos + 1);
    
    return {first, second};
}
