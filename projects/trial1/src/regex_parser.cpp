#include "regex_parser.h"

ChildNode::ChildNode(int type, RegExprNode *node)  : m_Type(type), m_Node(node)
{
}

RegExprNode::RegExprNode()
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
    return nullptr;
}

void RegExprNode::AddChild(const ChildNode& node)
{
    m_Children.push_back(node);
}

OrExpr::OrExpr()
{
}

OrExpr::~OrExpr()
{
}

NFA *OrExpr::Translate()
{
    return nullptr;
}

void OrExpr::Print()
{
}

StarExpr::StarExpr()
{
}

StarExpr::~StarExpr()
{
}

NFA *StarExpr::Translate()
{
    return nullptr;
}

void StarExpr::Print()
{
}

InterMedExpr::InterMedExpr()
{
}

InterMedExpr::~InterMedExpr()
{
}

NFA *InterMedExpr::Translate()
{
    return nullptr;
}

void InterMedExpr::Print()
{
}

DashExpr::DashExpr()
{
}

DashExpr::~DashExpr()
{
}

NFA *DashExpr::Translate()
{
    return nullptr;
}

void DashExpr::Print()
{
}

ParenExpr::ParenExpr()
{
}

ParenExpr::~ParenExpr()
{
}

NFA *ParenExpr::Translate()
{
    return nullptr;
}

void ParenExpr::Print()
{
}

CharExpr::CharExpr()
{
}

CharExpr::~CharExpr()
{
}

NFA *CharExpr::Translate()
{
    return nullptr;
}

void CharExpr::Print()
{
}

CharLitExpr::CharLitExpr()
{
}

CharLitExpr::~CharLitExpr()
{
}

NFA *CharLitExpr::Translate()
{
    return nullptr;
}

void CharLitExpr::Print()
{
}


RegexParser::RegexParser() : m_Root(nullptr), m_Loc(0)
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
        m_ErrorString = "RegexParser::Parse() --> did not successfully parse all tokens. Could not match" + m_CurrentToken->toString() + "\n";
        return nullptr;
    }
    return m_Root;
}

RegExprNode *RegexParser::ParseRegEx()
{   
    auto rNode = NEW_NODE(RegExprNode);
    RegExprNode* orNode = ParseOrExpr();
    if (orNode == nullptr) // got an error
        return orNode;


    rNode->AddChild(ChildNode(0, orNode));
    auto token = ConsumeToken(); 
    if (token == nullptr) // consumed all tokens
        return rNode;
    auto tokenType = token->m_Type;
    
    while(tokenType == TokenType::CONCAT)
    {
        RegExprNode* reNode = ParseRegEx();
        if (reNode == nullptr)
            return nullptr;
        rNode->AddChild(ChildNode(1, reNode));
        token = ConsumeToken();
        if (token == nullptr)
            break;
        tokenType = token->m_Type;
    }


    return rNode;
}

RegExprNode *RegexParser::ParseOrExpr()
{
    auto orNode = NEW_NODE(OrExpr);
    RegExprNode* starNode = ParseStarExpr();
    if (starNode == nullptr)
        return starNode;

    orNode->AddChild(ChildNode(0, starNode));

    auto token = ConsumeToken(); 
    if (token == nullptr) // consumed all tokens
        return orNode;
    auto tokenType = token->m_Type;

    while(tokenType == TokenType::OR)
    {
        auto or_Node = ParseOrExpr();
        if (or_Node == nullptr)
            return nullptr;
        orNode->AddChild(ChildNode(1, or_Node));
        token = ConsumeToken();
        if (token == nullptr)
            break;
        tokenType = token->m_Type;
    }
    return orNode;
}

RegExprNode *RegexParser::ParseStarExpr()
{
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
                return nullptr;
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
    auto bracketNode = NEW_NODE(BracketExpr);
    auto token = ConsumeToken();
    if (token->m_Type != TokenType::BRACKET_LEFT)
        return nullptr;
    bool isCarrotExpr = PeekToken() == TokenType::CARROT;
    if (isCarrotExpr)
        ConsumeToken();

    RegExprNode* dashExpr = ParseDashExpr();
    if (dashExpr == nullptr)
        return nullptr;

    bracketNode->AddChild(ChildNode(isCarrotExpr, dashExpr));
    token = ConsumeToken();
    if (token->m_Type != TokenType::BRACKET_RIGHT)
        return nullptr;
    return bracketNode;
}

RegExprNode *RegexParser::ParseDashExpr()
{
    auto dashNode = NEW_NODE(DashExpr);
    RegExprNode* cExpr1 = ParseCharExpr();
    if (cExpr1 == nullptr)
        return dashNode; // we can match epsilon

    dashNode->AddChild(ChildNode(0, cExpr1));
    
    auto tokenType = PeekToken();
    if (tokenType == TokenType::DASH)
    {
        ConsumeToken();
        RegExprNode* cExpr2 = ParseCharExpr();
        if (cExpr2 == nullptr) // can not match nullptr here
            return nullptr;
        dashNode->AddChild(ChildNode(0, cExpr2));
    }
    return dashNode;
}

RegExprNode *RegexParser::ParseParenExpr()
{
    auto parenNode = NEW_NODE(ParenExpr);
    auto token = ConsumeToken();
    if (token->m_Type != TokenType::PARENTHESES_LEFT)
        return nullptr;
    RegExprNode* regExpr = ParseRegEx();
    if (regExpr == nullptr)
        return nullptr;

    parenNode->AddChild(ChildNode(0, regExpr));

    token = ConsumeToken();
    if (token->m_Type != TokenType::PARENTHESES_RIGHT)
        return nullptr;
    return parenNode;
}



RegExprNode *RegexParser::ParseCharExpr()
{
    auto charNode = NEW_NODE(CharExpr);
    RegExprNode* charLitExpr = ParseCharLitExpr();
    if (charLitExpr == nullptr)
        return nullptr;
    charNode->AddChild(ChildNode(0, charLitExpr));
    auto tokenType = PeekToken();

    while(tokenType == TokenType::CONCAT)
    {
        ConsumeToken();
        RegExprNode* charLitExpr2 = ParseCharLitExpr();
        if (charLitExpr2 == nullptr)
            return nullptr;
        charNode->AddChild(ChildNode(0, charLitExpr2));
        auto tokenType = PeekToken();
    }
    return charNode;
}

RegExprNode *RegexParser::ParseCharLitExpr()
{
    auto charLitNode = NEW_NODE(CharLitExpr);

    auto tok = ConsumeToken();
    if (tok->m_Type != TokenType::CHARACTER)
        return nullptr;
    char c = tok->m_Lexeme[0];
    if (c == '.')
    {
        /*
            we let INT32_MAX be the any character match.
            We should add some special support to our NFAs (i.e. a special character meaning any)
        */
        charLitNode->AddChild(ChildNode(INT32_MAX, nullptr)); 
    }
    else
    {
        // if character is not '.' we just add the raw ascii code
        charLitNode->AddChild(ChildNode(c, nullptr));
    }
    return charLitNode;
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

NFA *BracketExpr::Translate()
{
    return nullptr;
}

void BracketExpr::Print()
{
}

