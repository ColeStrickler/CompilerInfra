#ifndef DRAGONLEX_H
#define DRAGONLEX_H
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#define EPSILON (UINT16_MAX)
#define DOT_CHAR (UINT16_MAX - 1)
class Dragonlex;
enum TokenType {
TOKEN_EOF,
NOUN,
VERB,
SKIP,
ERROR,
};

static const char* TokenTypeStrings[] = {
"TOKEN_EOF",
"NOUN",
"VERB",
"SKIP",
"ERROR",
};


enum INPUTACTION
{
    STORE_NO_METADATA,
    INPUT_SKIP,
    INPUT_ERROR,
    STORE_METADATA
};





struct NFASimple
{
public:
    void Reset()
    {
        m_CurrentStates.clear();
        m_CurrentStates = m_StartState;
    }

    bool ContainsAcceptState(std::unordered_set<int> states)
    {
        for (auto& state: states)
        {
            if (m_AcceptStates.count(state) != 0)
                return true;
        }
        return false;
    }

    bool inAcceptState()
    {
        return ContainsAcceptState(m_CurrentStates);
    }

    bool Advance(char c)
    {
        std::unordered_set<int> visited;
        //printf("currentState count %d\n", m_CurrentStates.size());
        std::unordered_set<int> current_states = EpsilonClosure(m_CurrentStates, visited);
        //printf("currentState count %d\n", current_states.size());
        m_PrevHadAcceptState = ContainsAcceptState(current_states);
        visited.clear();
        current_states = EpsilonClosure(TransitionFunction(current_states, c), visited);
        if (current_states.size() == 0)
            return false; // no valid states left, we can return false
        m_CurrentStates.clear();
        m_CurrentStates = current_states;
        return true;
    }

    std::unordered_set<int> TransitionFunction(std::unordered_set<int> currentState, uint16_t character)
    {
        //printf("e closure\n");
        std::unordered_set<int> newStates;
        
        for (auto& state: currentState)
        {
           // printf("transitions %d\n", m_Transitions[state].size());
            if (m_Transitions[state].count(DOT_CHAR) != 0) // we also look for the DOT wildcar character
            {
                std::unordered_set<int> transStates = m_Transitions[state][DOT_CHAR];
                for (auto& tState: transStates)
                    newStates.insert(tState);
            }



            if (m_Transitions[state].count(character) == 0) // if there is no transition from this state using character --> do nothing
                continue;

            std::unordered_set<int> transStates = m_Transitions[state][character];
            for (auto& tState: transStates)
                newStates.insert(tState);
        }
        return newStates;
    }



    std::unordered_set<int> EpsilonClosure(std::unordered_set<int> states,  std::unordered_set<int>& visited)
    {
        std::unordered_set<int> epsilonClosureStates = states;
        for (auto& state: states)
        {
            if (m_Transitions[state].count(EPSILON) == 0 || visited.count(state)) // if there is no transition from this state using EPSILON --> do nothing
                continue;
            visited.insert(state);
            // We must recurse here to capture n depth Epsilon transitions
            std::unordered_set<int> eClosureState = EpsilonClosure(m_Transitions[state][EPSILON], visited);
            for (auto& s : eClosureState)
            {
                epsilonClosureStates.insert(s);
            }
        }
        return epsilonClosureStates;
    }
    std::unordered_map<int, std::unordered_map<uint16_t, std::unordered_set<int>>> m_Transitions;
    std::unordered_set<int> m_AcceptStates;
    std::unordered_set<int> m_StartState;
    std::unordered_set<int> m_CurrentStates;
    bool m_PrevHadAcceptState;
};



struct TokenActionItem
{
    INPUTACTION m_Action;
    std::string m_MetaData;
    TokenType m_Type;
};




struct Position
{
    Position(int col, int line) : m_Col(col), m_Line(line)
    {
    }
    std::string toString() {return "[" + std::to_string(m_Line) + "," + std::to_string(m_Col) + "]";}
    int m_Col;
    int m_Line;
};




class Token
{
public:
    Token(TokenType type, const std::string& text, const Position& pos, bool useMetadata, std::string metadata) : m_Pos(pos)
    {
        m_Type = type;
        m_Lexeme = text;
        m_MetaData = metadata;
        m_bUseMetaData = useMetadata;
    }
    ~Token()
    {
    }
    std::string toString() { return std::string(TokenTypeStrings[m_Type]) + (m_bUseMetaData ? ":"+m_MetaData : "") +  + " " + m_Pos.toString() + "\n"; }
    std::string m_Lexeme;
    std::string m_MetaData;
    bool m_bUseMetaData;
    TokenType m_Type;
    Position m_Pos;
private:
};


class Dragonlex {
public:
	Dragonlex(const std::string& scanFileContents) : m_CurrentColumn(1), m_CurrentLine(1), m_RawLocation(0), m_FileContents(scanFileContents)
{
}

    Token* ScanToken()
    {
        if (m_RawLocation >= m_FileContents.size())
        {
            Position pos(m_CurrentColumn, m_CurrentLine);
            return new Token(TokenType::TOKEN_EOF, "EOF", pos, false, "");
        }

        Token* out = nullptr;
        int numNFAs = m_NFAs.size();
        //printf("nNFAs %d\n", numNFAs);
        std::vector<int> active;
        for (int i = 0; i < numNFAs; i++)
        {
            auto& nfa = m_NFAs[i];
            nfa.Reset();
            //printf("nfa start currentState count %d\n", nfa.m_CurrentStates.size());
            //for (auto& state: nfa.m_StartState)
            //    printf("nfa%d start state -> %d\n", i, state);


            active.push_back(i);
        }

        int colStart = m_CurrentColumn;
        int lineStart = m_CurrentLine;
        int rawStart = m_RawLocation;
        while(true)
        {
           // printf("total actives: %d\n", active.size());
            if (m_RawLocation >= m_FileContents.size())
            {
                if (active.size())
                {
                    int winner = active[0];
                    auto winNFA = m_NFAs[winner];
                    if (!winNFA.inAcceptState())
                    {
                        printf("DragonLex::ScanToken() error, did not match token at [%d,%d]\n", lineStart, colStart);
                        return nullptr;
                    }
                    return CreateToken(winner, colStart, lineStart, rawStart);
                }
                    
                return nullptr;
            }
            //printf("Raw location %d\n", m_RawLocation);
            std::vector<int> newActives;
            uint16_t c = m_FileContents[m_RawLocation];
            //printf("character %c\n", c);
            m_RawLocation += 1;
            
            m_CurrentColumn += 1;
            
            if (c == '\n')
            {
                m_CurrentColumn = 1;
                m_CurrentLine++;
            }

            for (int i = 0; i < active.size(); i++)
            {
                int index = active[i];
                auto& nfa = m_NFAs[index];
                if (nfa.Advance(c))
                    newActives.push_back(index);
            }

            if (newActives.size() == 0)
            {
                int winner = active[0];
                auto winNFA = m_NFAs[winner];
                
                m_RawLocation--; // go back
                m_CurrentColumn--;
                if (!winNFA.m_PrevHadAcceptState)
                {
                    printf("DragonLex::ScanToken() error, did not match token at [%d,%d]\n", lineStart, colStart);
                    return nullptr;
                }
                //printf("newActives.size() == 0 --> %c\n", c);
                return CreateToken(winner, colStart, lineStart, rawStart);
            }
            else if (newActives.size() == 1)
            {
                
                int winner = newActives[0];
                auto winNFA = m_NFAs[winner];
                if (!winNFA.m_PrevHadAcceptState)
                    continue;
                //printf("newActives.size() == 1 %c\n", c);
                return CreateToken(winner, colStart, lineStart, rawStart);
            }
            active = newActives;
        }

        return out;
    }


Token* CreateToken(int winningIndex, int colStart, int lineStart, int rawStart)
{
    //printf("WINNER %d\n", winningIndex);
    auto winningNFA = m_NFAs[winningIndex];

    Position pos(colStart, lineStart);
    auto action = m_ActionItems[winningIndex];
    if (action.m_Action == INPUTACTION::INPUT_ERROR)
    {
        //printf("column %d\n", m_CurrentColumn);
        std::cout << action.m_MetaData << "\n\n";
        return nullptr;
    }

    if (action.m_Action == INPUTACTION::INPUT_SKIP)
    {
        //printf("skip\n");
        return nullptr;
    }

    bool useMetaData = action.m_Action == INPUTACTION::STORE_METADATA;
    //printf("returning token \n");
    return new Token(action.m_Type, m_FileContents.substr(rawStart, m_RawLocation), pos, useMetaData, action.m_MetaData);
}

private:
	std::string m_FileContents;
	int m_RawLocation;
	int m_CurrentColumn;
	int m_CurrentLine;
	static std::vector<NFASimple> m_NFAs;
	static std::vector<TokenActionItem> m_ActionItems;
};
std::vector<NFASimple> Dragonlex::m_NFAs = {
NFASimple{
.m_Transitions = {{3,{{65535, {4,}},}},{4,{{103, {5,}},}},{0,{{100, {1,}},}},{2,{{111, {3,}},}},{1,{{65535, {2,}},}},},
.m_AcceptStates = {5,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{7,{{65535, {8,}},}},{8,{{115, {9,}},}},{3,{{65535, {4,}},}},{4,{{116, {5,}},}},{0,{{98, {1,}},}},{2,{{105, {3,}},}},{1,{{65535, {2,}},}},{6,{{101, {7,}},}},{5,{{65535, {6,}},}},},
.m_AcceptStates = {9,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {4,}},}},{2,{{97, {3,}},}},{3,{{65535, {5,2,}},}},{4,{{65535, {2,}},}},{5,{{110, {6,}},}},{0,{{109, {1,}},}},},
.m_AcceptStates = {6,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{32, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{5,{{65535, {6,}},}},{6,{{103, {7,}},}},{1,{{65535, {2,}},}},{2,{{119, {3,}},}},{0,{{115, {1,}},}},{4,{{97, {5,}},}},{3,{{65535, {4,}},}},},
.m_AcceptStates = {7,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{65534, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
};

std::vector<TokenActionItem> Dragonlex::m_ActionItems = {
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "dog",

.m_Type = TokenType::NOUN,

},
TokenActionItem{
.m_Action = (INPUTACTION)0,

.m_MetaData = "bites",

.m_Type = TokenType::VERB,

},
TokenActionItem{
.m_Action = (INPUTACTION)0,

.m_MetaData = "m(a)+n",

.m_Type = TokenType::NOUN,

},
TokenActionItem{
.m_Action = (INPUTACTION)1,

.m_MetaData = "",

.m_Type = TokenType::SKIP,

},
TokenActionItem{
.m_Action = (INPUTACTION)2,

.m_MetaData = "swaggin on em!",

.m_Type = TokenType::ERROR,

},
TokenActionItem{
.m_Action = (INPUTACTION)2,

.m_MetaData = "bad input",

.m_Type = TokenType::ERROR,

},
};

#endif
