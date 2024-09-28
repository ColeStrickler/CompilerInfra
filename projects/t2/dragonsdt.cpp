#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>


int sum = 0;

std::stack<int> s;



int lineNo = 1;



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

{{"EXPR", {{"NUMBER",ProductionRule("EXPR",{RHSObject((RHSType)2,"TERM"),RHSObject((RHSType)2,"EXPRTAIL"),RHSObject((RHSType)0,"#6"),})},{"(",ProductionRule("EXPR",{RHSObject((RHSType)2,"TERM"),RHSObject((RHSType)2,"EXPRTAIL"),RHSObject((RHSType)0,"#6"),})},}},
{"EXPRTAIL", {{"epsilon",ProductionRule("",{RHSObject((RHSType)3,"epsilon"),})},{")",ProductionRule("",{RHSObject((RHSType)1,")"),})},{"EOF",ProductionRule("",{RHSObject((RHSType)0,"EOF"),})},{"-",ProductionRule("EXPRTAIL",{RHSObject((RHSType)1,"-"),RHSObject((RHSType)2,"TERM"),RHSObject((RHSType)0,"#4"),RHSObject((RHSType)2,"EXPRTAIL"),})},{"+",ProductionRule("EXPRTAIL",{RHSObject((RHSType)1,"+"),RHSObject((RHSType)2,"TERM"),RHSObject((RHSType)0,"#5"),RHSObject((RHSType)2,"EXPRTAIL"),})},}},
{"TERMTAIL", {{"epsilon",ProductionRule("",{RHSObject((RHSType)3,"epsilon"),})},{"-",ProductionRule("",{RHSObject((RHSType)1,"-"),})},{"/",ProductionRule("TERMTAIL",{RHSObject((RHSType)1,"/"),RHSObject((RHSType)2,"FACTOR"),RHSObject((RHSType)0,"#2"),RHSObject((RHSType)2,"TERMTAIL"),})},{"+",ProductionRule("",{RHSObject((RHSType)1,"+"),})},{"*",ProductionRule("TERMTAIL",{RHSObject((RHSType)1,"*"),RHSObject((RHSType)2,"FACTOR"),RHSObject((RHSType)0,"#3"),RHSObject((RHSType)2,"TERMTAIL"),})},}},
{"TERM", {{"(",ProductionRule("TERM",{RHSObject((RHSType)2,"FACTOR"),RHSObject((RHSType)2,"TERMTAIL"),})},{"NUMBER",ProductionRule("TERM",{RHSObject((RHSType)2,"FACTOR"),RHSObject((RHSType)2,"TERMTAIL"),})},}},
{"FACTOR", {{"NUMBER",ProductionRule("FACTOR",{RHSObject((RHSType)0,"#1"),RHSObject((RHSType)1,"NUMBER"),})},{"(",ProductionRule("FACTOR",{RHSObject((RHSType)1,"("),RHSObject((RHSType)2,"EXPR"),RHSObject((RHSType)1,")"),})},}},
};

struct Token
{
    Token(std::string type, std::string lex) : tokenType(type), lexeme(lex)
    {
    
    }


    std::string tokenType;
    std::string lexeme;
};


void runAction(std::string action, Token* t){
switch(atoi(action.c_str())){
case 6:
{
 
       // printf("case 6 %d\n", s.size());
        auto a = s.top();
        if (s.size() == 1)
            std::cout << "Result: " << a << "\n";
    
break;
}
case 5:
{
 
    //printf("case 5 %d\n", s.size());
        if (s.size() < 2)
            break;
        auto a = s.top();
        s.pop();
        auto b = s.top();
        s.pop();
        s.push(a+b);
    
break;
}
case 4:
{
 
  //  printf("case 4\n");

        auto a = s.top();
        if (s.size() == 1)
            s.push(0-a);
        s.pop();
        if (!s.size())
            break;
        auto b = s.top();
        s.pop();
        s.push(a-b);
    
break;
}
case 3:
{
 
       // printf("case 3\n");
        if (s.size() < 2)
            break;
        auto a = s.top();
        s.pop();
        auto b = s.top();
        s.pop();
        s.push(a*b);
    
break;
}
case 2:
{
 
     //printf("case 2\n");
        if (s.size() < 2)
            break;
        auto a = s.top();
        s.pop();
        auto b = s.top();
        s.pop();
        s.push(a/b);
    
break;
}
case 1:
{
 
   // printf("case 1 adding %s\n", t->lexeme.c_str());
    s.push(atoi(t->lexeme.c_str())); 
    
    
break;
}
}}


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


std::string getFirstWord(const std::string& input) {
    std::istringstream iss(input);
    std::string firstWord;
    
    // Extract the first word
    iss >> firstWord;
    return firstWord;
}


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

std::stack<RHSObject> parseStack;


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


void handleEpsilon(Token* t)
{
    parseStack.pop();
   // std::cout << "handling epsilon\n"; 
}

int main(int argc, char** argv) {

	if (argc < 1){std::cout << "Usage: dragonsdt <token stream>"; return 0;}

	
    std::ifstream file(argv[1]);
    if (!file) {
        std::cout << "could not open " << argv[1] << "\n";
        return -1;
    }
    std::stringstream fs;
    fs << file.rdbuf();


	Token* t = nullptr;

	RHSObject startSymbol = RHSObject(RHSType::NONTERMINAL, "EXPR");
	parseStack.push(startSymbol);

	while(!parseStack.empty())
	{

	t = ReadToken(fs);

	//std::cout << t->tokenType << "," << t->lexeme << "\n";

	

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
   

	}

	if (parseStack.size()){std::cout << "parse unsuccessful as " << lineNo << "\n"; return -1;}else {printf("parse successful!\n");}
}

