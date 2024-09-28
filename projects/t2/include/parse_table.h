#ifndef PARSETABLE_H
#define PARSETABLE_H
#include "input.h"




struct DirectFollow
{
    std::string lhs;
    std::string follow;
    RHSType type;
};


struct ProductionFirstSet
{
    ProductionRule rule;
    std::unordered_set<std::string> first_set;
};



const std::string TokenStruct = R"(
struct Token
{
    Token(std::string type, std::string lex) : tokenType(type), lexeme(lex)
    {
    
    }


    std::string tokenType;
    std::string lexeme;
};

)";


const std::string SplitOnColonFunc = R"(
std::vector<std::string> splitOnColon(const std::string& input) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end;

    // Find the position of each colon
    while ((end = input.find(':', start)) != std::string::npos) {
        // Extract the substring from start to end
        result.push_back(input.substr(start, end - start));
        start = end + 1; // Move past the colon
    }

    // Add the last substring after the last colon (if any)
    result.push_back(input.substr(start));

    return result;
}
)";


const std::string GetFirstWordFunc = R"(
std::string getFirstWord(const std::string& input) {
    std::istringstream iss(input);
    std::string firstWord;
    
    // Extract the first word
    iss >> firstWord;
    return firstWord;
}
)";


const std::string OpenInputFile = R"(
    std::ifstream file(argv[1]);
    if (!file) {
        std::cout << "could not open " << argv[1] << "\n";
        return -1;
    }
    std::stringstream fs;
    fs << file.rdbuf();

)";


const std::string ReadTokenFromInputFileFunc = R"(
Token* ReadToken(std::stringstream& file)
{
    std::string line;
    if (std::getline(file, line))
    {
        lineNo++;
        if (line.find(':') != std::string::npos) // case 1
        {
            auto v = splitOnColon(line);
            return new Token(v[0], v[1]);
        }
        else // case 2
        {
            auto t = getFirstWord(line);
            return new Token(t, t);
        }      
    }
    return new Token("EOF", "EOF");
}
)";


const std::string ParseTableDefinition = R"(

enum RHSType
{
    ACTION,
    TERMINAL,
    NONTERMINAL,
    EPSILON
};

struct RHSObject
{
    RHSObject(){}
    RHSObject(RHSType t, std::string o) : type(t), obj(o)
    {

    }
    RHSObject(const RHSObject& other)
    {
        type = other.type;
        obj = other.obj;
    }

    RHSType type;
    std::string obj;
};


struct ProductionRule
{
    std::string m_LHS;
    std::vector<RHSObject> m_RHS;

    ProductionRule()
    {
    }

    ProductionRule(std::string lhs, const std::vector<RHSObject>& rhs) : m_LHS(lhs), m_RHS(rhs)
    {
    }

    ProductionRule(const ProductionRule& other)
    {
        m_LHS = other.m_LHS;
        m_RHS = other.m_RHS;
    }

    void Print()
    {
        std:: cout << m_LHS << " ::= ";
        for (auto& t: m_RHS)
            std::cout << t.obj << "(" << (t.type == RHSType::ACTION ? "action" : (t.type == RHSType::NONTERMINAL ? "NONTERMINAL" : "TERMINAL")) << ") ";
        std::cout << "\n";
    }
};
std::unordered_map<std::string, std::unordered_map<std::string, ProductionRule>> ParseTable =
)";


const std::string HandleNonTerminalFunc = R"(
bool handleNonTerminal(Token* t)
{
    
    auto top = parseStack.top();
    parseStack.pop();
    //std::cout << "handleNonTerminal: " << top.obj << "\n";
    if (ParseTable[top.obj].find(t->tokenType) != ParseTable[top.obj].end())
    {
        auto& prodRule = ParseTable[top.obj][t->tokenType];
        auto& rhs = prodRule.m_RHS;
        if (prodRule.m_LHS.size() == 0)
        {
            return true;
        }
       // std::cout << "pushing: ";
        for (int i = rhs.size() - 1; i >= 0; i--)
        {
           // std::cout << rhs[i].obj << ",";
            parseStack.push(rhs[i]);
        }
       // std::cout << "\n";
        return true;
    }
    else
    {
        //std::cout << "no parse entry for " << top.obj << "," << t->tokenType << "\n;";
        if (ParseTable[top.obj].find("epsilon") != ParseTable[top.obj].end())
        {
            //std::cout << "matched epsilon for " << top.obj << "\n";
            return true;
        }

       return false;
    }
    return false;
};
)";

const std::string HandleEpsilonFunc = R"(
void handleEpsilon(Token* t)
{
    parseStack.pop();
   // std::cout << "handling epsilon\n"; 
}
)";

const std::string HandleTerminalFunc = R"(
bool handleTerminal(Token* t)
{
    //std::cout << "handleTerminal\n";
    auto top = parseStack.top();
    if (t->tokenType == top.obj)
    {
        //std::cout << "popping " << top.obj << ":" << t->lexeme << "\n";
        parseStack.pop();
    }
    else
    {
        std::cout << "handleTerminal() --> did not match expected terminal at line " << lineNo << "\n";
        return false; 
    }
    return true;
}
)";


const std::string MainLoopBody = R"(

    bool getNextToken = false;
    while(!getNextToken && parseStack.size())
    {
       // printf("\n\n\nhere %d\n", parseStack.size());
       // printf("\n\n\n");
        auto top = parseStack.top();
        if (top.obj == "EOF")
        {
            printf("here!\n");
            parseStack.pop();
            break;
        }
       // std::cout << "top: " <<  top.obj << ", token: " << t->tokenType << ":" << t->lexeme << "\n";
         switch (top.type)
        {
            case RHSType::NONTERMINAL:
            {
                handleNonTerminal(t);
                break;
            }
            case RHSType::TERMINAL:
            {
                if(!handleTerminal(t))
                    return -1;
                getNextToken = true;
                break;
            }
            case RHSType::EPSILON:
            {
                handleEpsilon(t);
                break;
            }
            case RHSType::ACTION:
            {
                runAction(top.obj.substr(1), t);
                parseStack.pop();
                break;
            }
            default:
            {
                std::cout << "Received bad RHSType " << top.type << "\n";
                return -1;
            }
        }
    }
   
)";



class ParseTable
{
public:
    ParseTable(const std::vector<ProductionRule>& prodRules, std::unordered_map<std::string, std::string>& actions, std::string init, std::unordered_map<std::string, RHSType> mapping);
    std::unordered_set<std::string> ConstructFirstSets(std::string nonterminal, int i);
    std::unordered_set<std::string> ConstructFollowSets(std::string nonterminal);



    void PrintFirst(std::string nonterm);
    void PrintFollow(std::string nonterm);
    bool ConstructParseTable();

    std::string BuildActionTable();

    std::string BuildParseTable();

    void DumpFile(const std::string& file);
    void AppendFile(std::string msg);
    void SetTabs(int t) {numTabs = t;}
private:
    int numTabs;
    std::string m_StartSymbol;
    std::unordered_map<std::string, std::unordered_set<std::string>> m_FirstSets;
    std::unordered_map<std::string, ProductionFirstSet> m_FirstSetRules;
    std::unordered_map<std::string, std::unordered_set<std::string>> m_FollowSets;
    std::unordered_map<std::string, std::vector<DirectFollow>> m_DirectlyFollowing;

    std::unordered_map<std::string, std::unordered_map<std::string, ProductionRule>> m_ProdRuleFirstSets;
    std::unordered_map<std::string, std::unordered_map<std::string, ProductionRule>> m_ParseTable;

    std::unordered_map<std::string, RHSType> m_ProdMapping;
    std::unordered_map<std::string, std::vector<ProductionRule>> m_ProdRules;
    std::unordered_map<std::string, std::string> m_Actions;
    std::string m_InitSection;
    std::string m_File;
};


#endif