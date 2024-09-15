#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

class ScannerInputSorter
{
public:
    ScannerInputSorter(const std::string& filename);
    ~ScannerInputSorter();


    std::vector<std::pair<std::string, std::string>> GetRegexes();
    bool ParseLines();
private:
    
    std::pair<std::string, std::string> splitAtFirstSpace(const std::string& line);



    int m_LineNo;
    std::stringstream m_FileStringStream;
    std::string m_FileContents;
    std::vector<std::pair<std::string, std::string>> m_Regexes; // {regex, action}
};

#endif