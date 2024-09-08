#include "parser.hpp"



Parser::Parser() : m_Loc(0), m_CurrentToken(nullptr)
{
}

Parser::Parser(std::vector<Token*> tokens) : m_Tokens(tokens), m_Loc(0), m_CurrentToken(nullptr)
{
    
}

Token* Parser::ConsumeToken()
{
    if (m_Loc >= m_Tokens.size())
        return nullptr;
    

    m_CurrentToken = m_Tokens[m_Loc];
    m_Loc++;
    return m_CurrentToken;
}

TOKENTYPE Parser::PeekToken()
{
    if (m_Loc >= m_Tokens.size())
        return (TOKENTYPE)-1;
    return m_Tokens[m_Loc]->GetType();
}

TOKENTYPE Parser::PeekNextToken()
{
    if (AtEnd())
        return (TOKENTYPE)0;
    return m_Tokens[m_Loc + 1]->GetType();;
}

bool Parser::AtEnd()
{
    return PeekToken() == (TOKENTYPE)-1;
}

void Parser::ParseError()
{
    std::cerr << "Error parsing token: " + m_CurrentToken->toString() << "\n";
}

void Parser::ParseError(const std::string &msg)
{
    std::cerr << msg << "\n";
}

/*
    Consumes token such that it makes a complete tree
*/
RootNode* Parser::Parse()
{
    auto root = new RootNode();
    auto expr = ParseExpr();
    if (expr == nullptr)
    {
        printf("null expr\n");
        delete root;
        return nullptr;
    }
    root->m_Children.push_back(Child(0, expr));
    return root;
}

Node* Parser::ParseExpr()
{
    printf("Parser::ParseExpr()\n");
    Node* root;
    Node* currExpr;
    auto term = ParseTerm();
    if (term == nullptr)
        return term;
    TOKENTYPE rootType = PeekToken();
    

    root = new RawExpr();
    root->AddChild(Child(0, term));

    
    currExpr = root;
    while(PeekToken() == TOKENTYPE::CROSS || PeekToken() == TOKENTYPE::DASH)
    {
        auto type = ConsumeToken()->GetType();
        auto expr = ParseTerm();
        currExpr->AddChild(Child(type == TOKENTYPE::CROSS ? 0 : 1, expr));
    }
    return root;
}

Node* Parser::ParseTerm()
{
    printf("Parser::ParseTerm()\n");
    Node* root;
    Node* currTerm;
    auto factor = ParseFactor();
    if (factor == nullptr)
        return factor;
    TOKENTYPE rootType = PeekToken();

    root = new RawTerm();
    root->AddChild(Child(0, factor));

    
    currTerm = root;

    while(PeekToken() == TOKENTYPE::STAR || PeekToken() == TOKENTYPE::SLASH)
    {
        auto type = ConsumeToken()->GetType();
        auto term = ParseFactor();
        currTerm->AddChild(Child(type == TOKENTYPE::STAR ? 0 : 1, term));
    }
    return root;
}

Node* Parser::ParseFactor()
{
    printf("Parser::ParseFactor()\n");
    Token* token = ConsumeToken();
    TOKENTYPE type = token->GetType();

    if (type == TOKENTYPE::INT)
    {
        printf("RawFactor()\n");
        auto rawFactor = new RawFactor(token->GetLexeme());
        return rawFactor;
    }
    else if (type == TOKENTYPE::LPAREN)
    {
        auto expr = ParseExpr();
        token = ConsumeToken();
        type = token->GetType();
        if (type != TOKENTYPE::RPAREN)
        {
            ParseError();
            return nullptr;
        }
        printf("ParenFactor()\n");
        auto factor = new ParenFactor(expr);
        return factor;
    }
    else
    {
        ParseError("Parse error. Reached bottom of parse tree with no token matches.\n");
        return nullptr;
    }

}

PlusExpr::PlusExpr()
{
}


PlusExpr::~PlusExpr()
{
}

float PlusExpr::translate()
{
    float ret = 0.0f;
    for (auto& child: m_Children)
    {
        if (child.op == 0)
            ret += child.node->translate();
        else
            ret -= child.node->translate();
    }
}
DashExpr::DashExpr()
{
}


DashExpr::~DashExpr()
{
}

float DashExpr::translate()
{
    float ret = 0.0f;
    for (auto& child: m_Children)
    {
        if (child.op == 0)
            ret += child.node->translate();
        else
            ret -= child.node->translate();
    }
    return ret;
}

RawExpr::RawExpr()
{
}

RawExpr::RawExpr(Node *term)
{
    m_Children.push_back(Child(0, term));
}

RawExpr::~RawExpr()
{
}

float RawExpr::translate()
{
    float ret = 0.0f;
    for (auto& child: m_Children)
    {
        if (child.op == 0)
            ret += child.node->translate();
        else
            ret -= child.node->translate();
        printf("ret expr: %.3f\n", ret);
    }
    return ret;
}



StarTerm::StarTerm()
{
}

StarTerm::~StarTerm()
{
}

float StarTerm::translate()
{
    if (m_Children.size() != 2)
        printf("StarTerm::translate() Eval error. Did not have 2 children for eval.\n");
    // /auto lval = m_Children[0]->translate();
    // /auto rval = m_Children[1]->translate();

   // printf("StarTerm value %.2f\n", lval * rval);
    return 0.0f;
}


SlashTerm::SlashTerm()
{
}

float SlashTerm::translate()
{
    return 0.0f;
}

RawTerm::RawTerm()
{
}

RawTerm::RawTerm(Node *factor)
{
    m_Children.push_back(Child(0, factor));
}

RawTerm::~RawTerm()
{
}

float RawTerm::translate()
{
    float ret = 1.0f;
    for (auto& child: m_Children)
    {
        if (child.op == 0)
            ret *= child.node->translate();
        else
            ret /= child.node->translate();
        printf("ret term: %.3f\n", ret);
    }
    return ret;
}

RawFactor::RawFactor(const std::string &literal) : m_Literal(literal)
{

}

RawFactor::RawFactor()
{
}

RawFactor::~RawFactor()
{
}

float RawFactor::translate()
{
    printf("Raw factor value: %.2f\n", std::stof(m_Literal));
    return std::stof(m_Literal);
}

ParenFactor::ParenFactor()
{
}

ParenFactor::ParenFactor(Node *expr)
{
    m_Children.push_back(Child(0, expr));
}

float ParenFactor::translate()
{
    return m_Children[0].node->translate();
}

ParenFactor::~ParenFactor()
{
}

RootNode::RootNode()
{
}

RootNode::~RootNode()
{
}

float RootNode::translate()
{
    float sum = 0.0f;
    for (auto& child: m_Children)
    {
        sum += child.node->translate();
    }
    return sum;
}

float Node::translate()
{
    return 0.0f;
}

void Node::AddChild(Child child)
{
    m_Children.push_back(child);
}

void Node::AddChildren(Child child1, Child child2)
{
    m_Children.push_back(child1);
    m_Children.push_back(child2);
}
