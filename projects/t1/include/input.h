#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>


enum INPUTACTION
{
    STORE_NO_METADATA,
    SKIP,
    ERROR,
    STORE_METADATA
};


struct TokenActionItem
{
    TokenActionItem() {}
    TokenActionItem(INPUTACTION action, const std::string& metadata) : m_Action(action), m_MetaData(metadata)
    {

    }

    INPUTACTION m_Action;
    std::string m_MetaData;
    std::string m_TokenName;
};



class ScannerInputSorter
{
public:
    ScannerInputSorter(const std::string& filename);
    ~ScannerInputSorter();


    std::vector<std::pair<std::string, TokenActionItem>> GetRegexes();
    bool ParseLines();
private:
    bool GetTokenAction(TokenActionItem* out, std::string actions);
    std::pair<std::string, std::string> splitAtFirstSpace(const std::string& line);



    int m_LineNo;
    std::stringstream m_FileStringStream;
    std::string m_FileContents;
    std::vector<std::pair<std::string, TokenActionItem>> m_Regexes; // {regex, action}
};

#endif