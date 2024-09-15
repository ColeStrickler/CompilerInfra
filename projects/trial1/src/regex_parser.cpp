#include "regex_parser.h"

ChildNode::ChildNode(uint16_t type, RegExprNode *node)  : m_Type(type), m_Node(node)
{
}

RegExprNode::RegExprNode() : m_DebugMode(false)
{
}

RegExprNode::~RegExprNode()
{
}

void RegExprNode::Print()
{
}

NFA *RegExprNode::Translate()
{
    PRINT_CALL();
    if (m_Children.size() >= 1)
    {
            NFA* baseNFA = m_Children[0].m_Node->Translate();
            NFA* curr = baseNFA;
            for (int i = 1; i < m_Children.size(); i++)
            {
                auto expr = m_Children[i].m_Node;
                auto nfa = expr->Translate();
                if (nfa == nullptr)
                    return nullptr;

                curr = new NFA(baseNFA, nfa, NFA_OP::NFA_CONCAT);
            }
            return curr;
    }

   std::cout << "RegExpr::Translate() error. m_Children.size() < 1\n";
   return nullptr;
}

void RegExprNode::AddChild(const ChildNode& node)
{
    m_Children.push_back(node);
}

OrExpr::OrExpr() : RegExprNode()
{
}

OrExpr::~OrExpr()
{
}

NFA *OrExpr::Translate()
{
    PRINT_CALL();
    if (m_Children.size() >= 1)
    {
            NFA* baseNFA = m_Children[0].m_Node->Translate();
            NFA* curr = baseNFA;
            for (int i = 1; i < m_Children.size(); i++)
            {
                auto expr = m_Children[i].m_Node;
                auto nfa = expr->Translate();
                if (nfa == nullptr)
                    return nullptr;
                curr = new NFA(baseNFA, nfa, NFA_OP::NFA_OR);
            }
            return curr;
    }

    std::cout << "OrExpr::Translate() error. m_Children.size() < 1\n";
    return nullptr;
}

void OrExpr::Print()
{
}

StarExpr::StarExpr() : RegExprNode()
{
}

StarExpr::~StarExpr()
{
}

NFA *StarExpr::Translate()
{
    PRINT_CALL();
    if (m_Children.size() == 1)
    {
        auto expr = m_Children[0].m_Node;
        auto type = m_Children[0].m_Type;

        NFA* nfa = expr->Translate();
        if (nfa == nullptr)
            return nullptr;

        switch(type)
        {
            case 0: return nfa;
            case 1: return new NFA(nfa, NFA_OP::NFA_STAR);
            case 2: return new NFA(nfa, NFA_OP::NFA_QMARK);
            case 3: return new NFA(nfa, NFA_OP::NFA_PLUS);
            default:
            {
                std::cout << "StarExpr::Translate() error. Got bad child operation type. Got: " + std::to_string(type) + "\n";
                return nullptr;
            }
        }
    }

    std::cout << "StarExpr::Translate() error. m_Children.size() != 1\n";
    return nullptr;
}


void StarExpr::Print()
{
}

InterMedExpr::InterMedExpr() : RegExprNode()
{
}

InterMedExpr::~InterMedExpr()
{
}

NFA *InterMedExpr::Translate()
{
    PRINT_CALL();
    if (m_Children.size() == 1)
    {
        return m_Children[0].m_Node->Translate();
    }

    std::cout << "InterMedExpr::Translate() error. m_Children.size() != 1\n";
    return nullptr;
}

void InterMedExpr::Print()
{

}

DashExpr::DashExpr() : RegExprNode()
{
}

DashExpr::~DashExpr()
{
}


NFA *DashExpr::Translate()
{
    PRINT_CALL();

    int numChildren = m_Children.size();
    if (numChildren == 0)
    {
        return nullptr;
    }
    if (numChildren == 1)
    {
        return m_Children[0].m_Node->Translate();
    }

    if (numChildren == 2)
    {
        auto clitExpr = m_Children[0].m_Node;
        CharExpr* cExpr = (CharExpr*)m_Children[1].m_Node;

        NFA* cLitNFA = clitExpr->Translate();
        NFA* cExprNFA = cExpr->TranslateBracket(); // apply the special transformation for inside the bracket context
        return new NFA(cLitNFA, cExprNFA, NFA_OP::NFA_OR);
    }

    if (numChildren == 3)
    {
        auto cLit1 = (CharLitExpr*)m_Children[0].m_Node;
        auto cLit2 = (CharLitExpr*)m_Children[1].m_Node;


        uint16_t litExpr1 = cLit1->GetChar();
        uint16_t litExpr2 = cLit2->GetChar();
        NFA* rangeNFA = new NFA(litExpr1, litExpr2);
        
        NFA* dashExprNFA = m_Children[2].m_Node->Translate();
        if (dashExprNFA == nullptr) // this is okay, because DashExpr ::= ...|EPSILON
            return rangeNFA;
        else
            return new NFA(rangeNFA, dashExprNFA, NFA_OP::NFA_OR);
    }

    std::cout << "DashExpr::Translate() error. Invalid number of children: " + std::to_string(numChildren) + "\n";
    return nullptr;
}


void DashExpr::Print()
{
}

ParenExpr::ParenExpr() : RegExprNode()
{
}

ParenExpr::~ParenExpr()
{
}

NFA *ParenExpr::Translate()
{
    PRINT_CALL();
    if (m_Children.size() == 1)
    {
        NFA* nfa = m_Children[0].m_Node->Translate();
        return nfa;
    }
    std::cout << "ParenExpr::Translate() error. m_Children.size() != 1\n";
}

void ParenExpr::Print()
{
}

CharExpr::CharExpr() : RegExprNode()
{
}

CharExpr::~CharExpr()
{
}

NFA *CharExpr::Translate()
{
    PRINT_CALL();
    if (m_Children.size() >= 1)
    {
        NFA* start = m_Children[0].m_Node->Translate();
        NFA* curr = start;
        
        for (int i = 1; i < m_Children.size(); i++)
        {
            NFA* next = m_Children[i].m_Node->Translate();
            if (next == nullptr)
                return nullptr;
            curr = new NFA(curr, next, NFA_OP::NFA_CONCAT);
        }
        return curr;
    }
    std::cout << "CharExpr::Translate() error. m_Children.size() < 1\n";
    return nullptr;
}

NFA *CharExpr::TranslateBracket()
{
    PRINT_CALL();
    if (m_Children.size() >= 1)
    {
        //
        NFA* start = m_Children[0].m_Node->Translate();
        NFA* curr = start;
        
        for (int i = 1; i < m_Children.size(); i++)
        {
            NFA* next = m_Children[i].m_Node->Translate();
            if (next == nullptr)
                return nullptr;
            curr = new NFA(curr, next, NFA_OP::NFA_OR);
        }
        return curr;
    }
    std::cout << "CharExpr::Translate() error. m_Children.size() < 1\n";
    return nullptr;
}

void CharExpr::Print()
{
}

CharLitExpr::CharLitExpr() : RegExprNode()
{
    
}

CharLitExpr::~CharLitExpr()
{
}

uint16_t CharLitExpr::GetChar()
{
    if (m_Children.size() == 1)
    {
        auto child = m_Children[0];
        return child.m_Type;
    }
    std::cout << "CharLitExpr::GetChar() error. m_Children.size() != 1. Got " << std::to_string(m_Children.size()) << "\n";
    return 0x0;
}

NFA *CharLitExpr::Translate()
{
    // should only be one child
    if (m_Children.size() == 1)
    {
        auto child = m_Children[0];
        return new NFA(child.m_Type);
    }
    std::cout << "CharLitExpr::Translate() error. m_Children.size() != 1\n";
    return nullptr;
}

void CharLitExpr::Print()
{
}


RegexParser::RegexParser() : m_Root(nullptr), m_Loc(0), m_DebugMode(false)
{
}

RegexParser::~RegexParser()
{
    Cleanup();
}

void RegexParser::AddTokens(std::vector<Token> tokens)
{
    m_Tokens.clear();
    m_Tokens = tokens;
}

RegExprNode *RegexParser::Parse()
{
    m_Root = ParseRegEx();
    if (m_Root == nullptr)
    {
        m_ErrorString = "RegexParser::Parse() failed to match token " + m_CurrentToken->toString() + "\n"; // get current token string
        return nullptr;
    }

    if (!AtEnd())
    {
        m_ErrorString = "RegexParser::Parse() --> did not successfully parse all tokens. Could not match token #" + std::to_string(m_Loc-1)\
          + " " + m_CurrentToken->toString() + "\n";
        return nullptr;
    }
    return m_Root;
}

RegExprNode *RegexParser::ParseRegEx()
{   
    PRINT_CALL();
    auto rNode = NEW_NODE(RegExprNode);
    RegExprNode* orNode = ParseOrExpr();
    if (orNode == nullptr) // got an error
    {
        return orNode;
    }


    rNode->AddChild(ChildNode(0, orNode));
    
    auto tokenType = PeekToken();
    
    while(tokenType == TokenType::CONCAT)
    {
        ConsumeToken();
        RegExprNode* reNode = ParseRegEx();
        if (reNode == nullptr)
        {
            return nullptr;
        }
        rNode->AddChild(ChildNode(1, reNode));
        
        tokenType = PeekToken();
    }


    return rNode;
}

RegExprNode *RegexParser::ParseOrExpr()
{
    PRINT_CALL();
    auto orNode = NEW_NODE(OrExpr);
    RegExprNode* starNode = ParseStarExpr();
    if (starNode == nullptr)
        return starNode;

    orNode->AddChild(ChildNode(0, starNode));

    auto tokenType = PeekToken();

    while(tokenType == TokenType::OR)
    {
        auto token = ConsumeToken();
        auto or_Node = ParseOrExpr();
        if (or_Node == nullptr)
            return nullptr;
        orNode->AddChild(ChildNode(1, or_Node));
        tokenType = PeekToken();
    }
    return orNode;
}

RegExprNode *RegexParser::ParseStarExpr()
{
    PRINT_CALL();
    auto starNode = NEW_NODE(StarExpr);
    RegExprNode* interMedNode = ParseIntermedExpr();
    if (interMedNode == nullptr)
        return interMedNode;

    
    auto tokenType = PeekToken();

    if (tokenType == TokenType::STAR)
    {
        ConsumeToken();
        starNode->AddChild(ChildNode(1, interMedNode)); // star operator
    }
    else if (tokenType == TokenType::QUESTION_MARK)
    {
        ConsumeToken();
        starNode->AddChild(ChildNode(2, interMedNode)); // question mark operator
    }
    else if (tokenType == TokenType::PLUS)
    {
        ConsumeToken();
        starNode->AddChild(ChildNode(3, interMedNode)); // plus operator
    }
    else
        starNode->AddChild(ChildNode(0, interMedNode)); // no operator
    return starNode;
}




RegExprNode *RegexParser::ParseIntermedExpr()
{
    PRINT_CALL();
    auto intermedNode = NEW_NODE(InterMedExpr);
    auto nextType = PeekToken();

    /*
        Predictive parse the next expression based on the next TokenType
    */

   switch(nextType)
   {
        case TokenType::BRACKET_LEFT:
        {
            RegExprNode* bracketExpr = ParseBracketExpr();
            if (bracketExpr == nullptr)
                return nullptr;
            intermedNode->AddChild(ChildNode(0, bracketExpr));
            break;
        } 
        case TokenType::PARENTHESES_LEFT:
        {
            RegExprNode* parenExpr = ParseParenExpr();
            if (parenExpr == nullptr)
                return nullptr;
            intermedNode->AddChild(ChildNode(1, parenExpr));
            break;
        }
        case TokenType::CHARACTER:
        {
            RegExprNode* charExpr = ParseCharExpr();
            if (charExpr == nullptr)
            {
                return nullptr;
            }
            intermedNode->AddChild(ChildNode(2, charExpr));
            break;
        }
        case TokenType::DOT:
        {
            RegExprNode* charExpr = ParseCharExpr();
            if (charExpr == nullptr)
            {
                return nullptr;
            }
            intermedNode->AddChild(ChildNode(2, charExpr));
            break;
        }
        default:
        {
            /*
                If we get here, none of the tokens matched the grammar, and none were consumed.
                So, to get error reporting on the proper token we consume the one that we errored on
                to set m_CurrentToken to the one we could not match.
            */
            ConsumeToken(); 
            return nullptr;
        }
   }

    return intermedNode;
}

RegExprNode *RegexParser::ParseBracketExpr()
{
    PRINT_CALL();
    auto bracketNode = NEW_NODE(BracketExpr);
    auto token = ConsumeToken();
    if (token->m_Type != TokenType::BRACKET_LEFT)
        return nullptr;
    bool isCarrotExpr = PeekToken() == TokenType::CARROT;
    if (isCarrotExpr)
    {
        ConsumeToken();
    }

    RegExprNode* dashExpr = ParseDashExpr();
    if (dashExpr == nullptr)
        return nullptr;

    bracketNode->AddChild(ChildNode(isCarrotExpr, dashExpr));
    token = ConsumeToken();
    if (token->m_Type != TokenType::BRACKET_RIGHT)
        return nullptr;
    return bracketNode;
}

NFA *BracketExpr::Translate()
{
    if (m_Children.size() == 1)
    {
        auto child = m_Children[0];
        auto node = (DashExpr*)child.m_Node;
        auto type = child.m_Type;
        auto chars = node->m_CharactersUsed;
        if (type == 1)
        {
            return new NFA(node->Translate(), NFA_OP::NFA_CARROT, chars);
        }
        else
        {
            return node->Translate();
        }


    }
    std::cout << "BracktExpr::Translate() error. m_Children.size() != 1\n";
    return m_Children[0].m_Node->Translate();
}


RegExprNode *RegexParser::ParseDashExpr()
{
    PRINT_CALL();
    auto dashNode = NEW_NODE(DashExpr);
    auto cExpr1 = (CharLitExpr*)ParseCharLitExpr();
    if (cExpr1 == nullptr)
    {
        return dashNode; // DashExpr ::= EPSILON
    }

    auto tokenType = PeekToken();
    dashNode->AddChild(ChildNode(tokenType == TokenType::DASH, cExpr1));
    
    
    if (tokenType == TokenType::DASH) // we force dashes to be at the beginning of BracketExpr
    {
        ConsumeToken();
        RegExprNode* cExpr2 = ParseCharLitExpr();
        if (cExpr2 == nullptr) // can not match nullptr here because we consumed DASH
            return nullptr;
        
        uint16_t beg = cExpr1->m_Children[0].m_Type;
        uint16_t end = cExpr2->m_Children[0].m_Type;
        
        uint16_t start = std::min(beg, end);
        uint16_t finish = std::min(beg, end);

        for (uint16_t i = start; i <= finish; i++)
            dashNode->m_CharactersUsed.insert(i);

        dashNode->AddChild(ChildNode(1, cExpr2));
        auto childDashNode = (DashExpr*)ParseDashExpr();
        if (childDashNode != nullptr) // should only receive nullptr on parsing failure
        {
            dashNode->AddChild(ChildNode(2, childDashNode));
            dashNode->m_CharactersUsed.insert(childDashNode->m_CharactersUsed.begin(), childDashNode->m_CharactersUsed.end());
        }
    }
    else
    {
        dashNode->m_CharactersUsed.insert(cExpr1->GetChar());

        auto type =PeekToken();
        if (type == TokenType::CONCAT)
            ConsumeToken();


        auto cExpr2 = (CharExpr*)ParseCharExpr();
        if (cExpr2 != nullptr)
        {
            dashNode->AddChild(ChildNode(0, cExpr2));
            dashNode->m_CharactersUsed.insert(cExpr2->m_CharactersUsed.begin(), cExpr2->m_CharactersUsed.end());
        }
    }


    return dashNode;
}

RegExprNode *RegexParser::ParseParenExpr()
{
    PRINT_CALL();
    auto parenNode = NEW_NODE(ParenExpr);
    auto token = ConsumeToken();
    if (token->m_Type != TokenType::PARENTHESES_LEFT)
        return nullptr;
    RegExprNode* regExpr = ParseRegEx();
    if (regExpr == nullptr)
        return nullptr;

    parenNode->AddChild(ChildNode(0, regExpr));
    token = ConsumeToken();
    if (token == nullptr)
        return nullptr;

    if (token->m_Type != TokenType::PARENTHESES_RIGHT)
        return nullptr;
    return parenNode;
}



RegExprNode *RegexParser::ParseCharExpr()
{
    PRINT_CALL();
    auto charNode = NEW_NODE(CharExpr);
    RegExprNode* charLitExpr = ParseCharLitExpr();
    if (charLitExpr == nullptr)
    {
        return nullptr;
    }

    charNode->m_CharactersUsed.insert(charLitExpr->m_Children[0].m_Type);
    charNode->AddChild(ChildNode(0, charLitExpr));
    auto tokenType = PeekToken();

    while(tokenType == TokenType::CONCAT && PeekNextToken() == TokenType::CHARACTER && !AtEnd())
    {
        ConsumeToken();
        RegExprNode* charLitExpr2 = ParseCharLitExpr();
        if (charLitExpr2 == nullptr)
            return nullptr;
        charNode->m_CharactersUsed.insert(charLitExpr2->m_Children[0].m_Type);
        charNode->AddChild(ChildNode(0, charLitExpr2));
        auto tokenType = PeekToken();
    }
    return charNode;
}

RegExprNode *RegexParser::ParseCharLitExpr()
{
    PRINT_CALL();
    auto charLitNode = NEW_NODE(CharLitExpr);

    auto tokenType = PeekToken();


    if (tokenType != TokenType::CHARACTER && tokenType != TokenType::DOT)
    {
        printf("no match charLit : %d\n", tokenType);
        return nullptr;
    }

    auto token = ConsumeToken();

    char c = token->m_Lexeme[0];
    auto type = token->m_Type;
    if (type == TokenType::DOT)
    {
        /*
            we let INT32_MAX be the any character match.
            We should add some special support to our NFAs (i.e. a special character meaning any)
        */
        charLitNode->AddChild(ChildNode(DOT_CHAR, nullptr)); 
    }
    else
    {
        // if character is not '.' we just add the raw ascii code
        charLitNode->AddChild(ChildNode(c, nullptr));
    }
    return charLitNode;
}

void RegexParser::SetDebugMode(bool on)
{
    m_DebugMode = on;
}

void* RegexParser::AddPointer(void *ptr)
{
    if (ptr != nullptr)
        m_PointerCleanup.push_back(ptr);
    return ptr;
}


void RegexParser::Cleanup()
{
    for (auto& p: m_PointerCleanup)
        delete p;
}

std::string RegexParser::GetCurrentTokenString()
{
    if (m_CurrentToken == nullptr)
        return "";
    return m_CurrentToken->toString();
}

Token *RegexParser::ConsumeToken()
{
    if (AtEnd())
        return nullptr;
    m_CurrentToken = &m_Tokens[m_Loc];
    return &m_Tokens[m_Loc++];
}

TokenType RegexParser::PeekToken()
{
    if (m_Loc >= m_Tokens.size())
        return TOKEN_INVALID;
    return m_Tokens[m_Loc].m_Type;
}

TokenType RegexParser::PeekNextToken()
{
    if (m_Loc + 1 >= m_Tokens.size())
        return TOKEN_INVALID;
    return m_Tokens[m_Loc + 1].m_Type;
}

bool RegexParser::AtEnd()
{
    return PeekToken() == TOKEN_INVALID;
}

BracketExpr::BracketExpr()
{
}

BracketExpr::~BracketExpr()
{
}


void BracketExpr::Print()
{
}

