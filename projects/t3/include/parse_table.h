#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H


#include "input.h"
#include <cassert>
/*
    std::pair<int, int>
*/
struct LR0_Item
{
    std::string m_LHS;
    int m_DotLoc;
    int m_ProdRuleIndex;

    bool operator==(const LR0_Item& other) const
    {
        return m_LHS == other.m_LHS && m_DotLoc == other.m_DotLoc && m_ProdRuleIndex == other.m_ProdRuleIndex;
    }
};

enum P_ACTION
{
    DUMMY,
    SHIFT,
    REDUCE,
    ACCEPT,
};

struct ParseAction
{
    ParseAction()
    {

    }

    ParseAction(int shiftstate) : action(P_ACTION::SHIFT), shiftState(shiftstate)
    {

    }

    ParseAction(int reducepopcount, std::string rpush): reducePopCount(reducepopcount), reducePush(rpush), action(P_ACTION::REDUCE)
    {

    }

    ParseAction(P_ACTION accept) : action(P_ACTION::ACCEPT)
    {

    }

    P_ACTION action;
    int shiftState;

    int reducePopCount;
    std::string reducePush;
};


struct PairHash {
    std::size_t operator()(const LR0_Item& item) const {
        auto a = std::to_string(item.m_DotLoc);
        auto b = std::to_string(item.m_ProdRuleIndex);
        return std::hash<std::string>{}(a+"-"+"-"+b+"-"+item.m_LHS);; // this should be sufficient for our purposes
    }
};


struct SetHash {
    template <typename T>
    std::size_t operator()(const std::unordered_set<T, PairHash>& set) const {
        std::size_t hashValue = 0;
        for (const auto& item : set) {
            hashValue ^= PairHash{}(item);
        }
        return hashValue;
    }
};

struct SetEqual {
    template <typename T>
    bool operator()(const std::unordered_set<T, PairHash>& lhs, const std::unordered_set<T, PairHash>& rhs) const {
        return lhs == rhs; // Uses the built-in equality
    }
};

struct DirectFollow
{
    std::string lhs;
    std::string follow;
    RHSType type;
};


struct Token
{
    Token(std::string type, std::string lex) : tokenType(type), lexeme(lex)
    {
    
    }


    std::string tokenType;
    std::string lexeme;
};

Token* ReadToken(std::stringstream& file);
std::vector<std::string> splitOnColon(const std::string& input);
std::string getFirstWord(const std::string& input);


#include <stack>
class ParseTable
{
public: 
    ParseTable(const std::vector<ProductionRule>& grammar);
    void ProduceItems();
    void ProduceClosureSets();

    bool ConstructActionTables();
    bool m_BadGrammar;
    bool Parse(std::vector<Token*> tokens);

    std::vector<int> m_StateStack;
    std::vector<std::string> m_SymbolStack;


    void BuildActionTable();
    bool isTerminal(std::string sym);
    void PrintItem(const LR0_Item& item);
    std::unordered_set<LR0_Item, PairHash>  Closure(const LR0_Item& item);
    //std::vector<ProductionRule> IndexToProdRules(std::pair<int, int> item, int index);
    void PrintItemSet(const std::unordered_set<LR0_Item, PairHash>& s);
    void ProduceStartItem(ProductionRule rule);
    bool CanBeEpsilon(std::string sym, std::unordered_set<std::string>& seen);
    void ProduceAcceptItem();
    void CanonicalSetConstruction();
    //void ConstructStates
    std::unordered_set<LR0_Item, PairHash> GoTo(const std::unordered_set<LR0_Item, PairHash>& set, const std::string& symbol, bool* reduce);
    //void PrintItemSet(const std::unordered_set<std::pair<int, int>, PairHash>& s);
private:

    std::unordered_map<std::string, std::unordered_set<std::string>> m_FirstSets;
    std::unordered_map<std::string, std::unordered_set<std::string>> m_FollowSets;
    std::unordered_set<std::string> ConstructFirstSets(std::string nonterminal, std::unordered_set<std::string>& seen);
    std::unordered_set<std::string> ConstructFollowSets(std::string nonterminal);

    std::unordered_map<std::string, std::vector<DirectFollow>> m_DirectlyFollowing;
    std::vector<ProductionRule> m_ProdRules;
    std::unordered_set<std::string> m_Symbols;
    std::unordered_map<std::string, std::vector<ProductionRule>> m_Grammar;
    std::unordered_set<std::string> m_Terminals;
    std::unordered_set<std::string> m_NonTerminals;
    LR0_Item m_StartItem;
    LR0_Item m_AcceptItem;
    std::unordered_map<int, std::unordered_map<std::string, int>> m_GoToTable;
    std::unordered_map<int, std::unordered_map<std::string, ParseAction>> m_ActionTable;
    std::unordered_set<LR0_Item, PairHash> m_InitialItems;

    std::unordered_map<int, std::unordered_set<LR0_Item, PairHash>> rC;
    std::unordered_map<std::unordered_set<LR0_Item, PairHash>, int, SetHash, SetEqual> C;

};

#endif