#include "input.h"



#include "parse_table.h"




int main(int argc, char** argv)
{
    InputSorter input(argv[1]);


    if(!input.ParseLines())
    {
        std::cout << "Unable to parse lines from " + std::string(argv[1]) + ".\n";
        return -1; 
    }

    auto rules = input.GetProdRules();

    //for (auto& rule: rules)
    //    std::cout << rule.toString() << "\n";

    ParseTable pTable(rules);
    if (pTable.m_BadGrammar)
    {
        std::cout << "Bad grammar\n";
        return -2;
    }

     std::ifstream file(argv[2]);
    if (!file) {
        std::cout << "could not open " << argv[2] << "\n";
        return -1;
    }
    std::stringstream fs;
    fs << file.rdbuf();

    std::vector<Token*> tokens;
    while(true)
    {
        Token* t = ReadToken(fs);
        tokens.push_back(t);
        if (t->lexeme == "EOF")
            break;
    }
    //for (auto& token: tokens)
    //    printf("%s\n", token->tokenType.c_str());


    if (pTable.Parse(tokens))
        printf("accepted\n");
    else
    {
        printf("Parse error.\n");
        return 1;
    }


    return 0;
}