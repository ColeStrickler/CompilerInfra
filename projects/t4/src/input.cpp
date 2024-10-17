#include "input.h"



InputSorter::InputSorter(const std::string &filename) : m_LineNo(1)
{
    std::ifstream file(filename);
    if (!file) {
        std::cout << "could not open " << filename << "\n";
        return;
    }
    m_FileStringStream << file.rdbuf();
}

InputSorter::~InputSorter()
{
}

bool InputSorter::ParseLines()
{

    HandleGrammarSection();
    /*
        We do two traversals over the productions to properly change nonterminals to terminals
    */
    std::unordered_set<std::string> nonterminals;
    for (auto& p : m_ProductionRules)
        nonterminals.insert(p.m_LHS);
    
    for (auto& p : m_ProductionRules)
    {
        for (auto& r: p.m_RHS)
        {
            if ( r.type != RHSType::ACTION && r.type != RHSType::EPSILON && nonterminals.count(r.obj) == 0)
            {
                r.type = RHSType::TERMINAL;
                
            }
            m_ProdMapping[r.obj] = r.type;
        }
        m_ProdMapping[p.m_LHS] = RHSType::NONTERMINAL;
    }


    return true;
}

bool InputSorter::HandleGrammarSection()
{
    while(std::getline(m_FileStringStream, m_CurrentLine) && m_CurrentLine != "%%")
    {
        auto components = SplitGrammarString(m_CurrentLine);
        if (components.size() < 2)
        {
            std::cout << "InputSorter::HandleGrammarSection() --> invalid grammar rule at line number " << m_LineNo << ".\n"; 
            return false;
        }


        std::vector<std::string> actions;
        ProductionRule prod;
        prod.m_LHS = components[0];
        for (int i = 1; i < components.size(); i++)
        {  
            auto component = components[i];
            if (component == ".")
                prod.m_RHS.push_back(RHSObject(RHSType::EPSILON, component));
            else if (component[0] == '#')
                prod.m_RHS.push_back(RHSObject(RHSType::ACTION, component));
            else
                prod.m_RHS.push_back(RHSObject(RHSType::NONTERMINAL, component)); // need to figure out where to set this properly
        }
        m_ProductionRules.push_back(prod);
        m_LineNo++;
    }


    return true;

}




std::vector<std::string> InputSorter::SplitGrammarString(const std::string &input)
 {
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string word;

    bool found = false;
    // Use the stream to extract words separated by whitespace
    while (iss >> word) {
        if (word == "::=") // dont need this
        {
            found = true;
            continue;
        }
        result.push_back(word);
    }

    if (!found)
    {
        std::cout << "InputSorter::SplitGrammarString() --> did not find '::=' in line number " << m_LineNo << "\n";
        return {};
    }

    return result;
}


//std::string InputSorter::ExtractBetweenBrackets(std::string input)
//{
//    size_t start = input.find('{');
//    size_t end = input.find('}', start);
//    int line_add = 0;
//    while (end == std::string::npos && std::getline(m_FileStringStream, m_CurrentLine))
//    {
//        input += "\n" + m_CurrentLine;
//        end = input.find('}');
//        line_add++;
//             
//    }
//    m_LineNo += line_add;
//
//    // Check if both brackets were found
//    if (start != std::string::npos && end != std::string::npos) {
//        // Extract the substring between the brackets
//        return input.substr(start + 1, end - start - 1);
//    }
//
//    // Return an empty string if no valid brackets were found
//    return "";
//}

std::string InputSorter::GetFirstWord(std::string input)
{
    std::istringstream iss(input);
    std::string firstWord;
    
    // Extract the first word
    iss >> firstWord;
    return firstWord;
}
