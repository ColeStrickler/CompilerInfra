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
DIGIT,
INTEGER,
IDENTIFIER,
STRING,
ERROR,
SKIP,
ASSIGN,
COLON,
COMMA,
CROSS,
DASH,
EQUALS,
GREATER,
GREATEREQ,
LBRACKET,
LESS,
LESSEQ,
LPAREN,
NOT,
REF,
NOTEQUALS,
POSTDEC,
POSINC,
RBRACKET,
RPAREN,
SEMICOLON,
SLASH,
STAR,
ARROW,
};

static const char* TokenTypeStrings[] = {
"TOKEN_EOF",
"DIGIT",
"INTEGER",
"IDENTIFIER",
"STRING",
"ERROR",
"SKIP",
"ASSIGN",
"COLON",
"COMMA",
"CROSS",
"DASH",
"EQUALS",
"GREATER",
"GREATEREQ",
"LBRACKET",
"LESS",
"LESSEQ",
"LPAREN",
"NOT",
"REF",
"NOTEQUALS",
"POSTDEC",
"POSINC",
"RBRACKET",
"RPAREN",
"SEMICOLON",
"SLASH",
"STAR",
"ARROW",
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
            int oldCol = m_CurrentColumn;
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
                int winner = 0;
                auto winNFA = m_NFAs[winner];
                for (int i = 0; i < active.size(); i++)
                {
                    winner = active[i];
                    winNFA = m_NFAs[winner];
                    if (winNFA.m_PrevHadAcceptState)
                        break;
                    
                }
                
                
                m_RawLocation--; // go back
                
                if (c == '\n')
                {
                    m_CurrentLine--;
                    m_CurrentColumn = oldCol;
                }
                else
                    m_CurrentColumn--;

                if (!winNFA.m_PrevHadAcceptState)
                {
                    printf("DragonLex::ScanToken() error, did not match token at [%d,%d] --> last char %c\n", lineStart, colStart, c);
                    return nullptr;
                }
                //printf("newActives.size() == 0 --> %c\n", c);
                return CreateToken(winner, colStart, lineStart, rawStart);
            }
            //else if (newActives.size() == 1)
            //{
            //    
            //    int winner = newActives[0];
            //    auto winNFA = m_NFAs[winner];
            //    if (!winNFA.inAcceptState())
            //        continue;
            //   // printf("newActives.size() == 1 %c\n", c);
            //    return CreateToken(winner, colStart, lineStart, rawStart);
            //}
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
.m_Transitions = {{0,{{57, {1,}},{56, {1,}},{55, {1,}},{54, {1,}},{53, {1,}},{52, {1,}},{51, {1,}},{50, {1,}},{49, {1,}},{48, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {0,}},}},{2,{{65535, {0,}},}},{0,{{48, {1,}},{49, {1,}},{50, {1,}},{51, {1,}},{52, {1,}},{53, {1,}},{54, {1,}},{55, {1,}},{56, {1,}},{57, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {2,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{9,{{65535, {24,}},}},{22,{{65535, {20,10,}},}},{21,{{65535, {23,}},}},{20,{{65535, {18,12,}},}},{19,{{65535, {21,}},}},{18,{{65535, {16,14,}},}},{17,{{65535, {19,}},}},{16,{{95, {17,}},}},{15,{{65535, {19,}},}},{14,{{57, {15,}},{56, {15,}},{55, {15,}},{54, {15,}},{53, {15,}},{52, {15,}},{51, {15,}},{50, {15,}},{49, {15,}},{48, {15,}},}},{13,{{65535, {21,}},}},{12,{{78, {13,}},{79, {13,}},{80, {13,}},{81, {13,}},{82, {13,}},{83, {13,}},{84, {13,}},{85, {13,}},{86, {13,}},{87, {13,}},{88, {13,}},{89, {13,}},{90, {13,}},{77, {13,}},{76, {13,}},{75, {13,}},{74, {13,}},{73, {13,}},{72, {13,}},{71, {13,}},{70, {13,}},{69, {13,}},{68, {13,}},{67, {13,}},{66, {13,}},{65, {13,}},}},{1,{{65535, {9,}},}},{7,{{65535, {9,}},}},{8,{{65535, {6,0,}},}},{2,{{90, {3,}},{89, {3,}},{88, {3,}},{87, {3,}},{86, {3,}},{85, {3,}},{84, {3,}},{83, {3,}},{82, {3,}},{81, {3,}},{80, {3,}},{79, {3,}},{78, {3,}},{65, {3,}},{66, {3,}},{67, {3,}},{68, {3,}},{69, {3,}},{70, {3,}},{71, {3,}},{72, {3,}},{73, {3,}},{74, {3,}},{75, {3,}},{76, {3,}},{77, {3,}},}},{3,{{65535, {7,}},}},{4,{{95, {5,}},}},{5,{{65535, {7,}},}},{6,{{65535, {4,2,}},}},{0,{{110, {1,}},{111, {1,}},{112, {1,}},{113, {1,}},{114, {1,}},{115, {1,}},{116, {1,}},{117, {1,}},{118, {1,}},{119, {1,}},{120, {1,}},{121, {1,}},{122, {1,}},{109, {1,}},{108, {1,}},{107, {1,}},{106, {1,}},{105, {1,}},{104, {1,}},{103, {1,}},{102, {1,}},{101, {1,}},{100, {1,}},{99, {1,}},{98, {1,}},{97, {1,}},}},{24,{{65535, {22,23,}},}},{11,{{65535, {23,}},}},{23,{{65535, {22,}},}},{10,{{122, {11,}},{121, {11,}},{120, {11,}},{119, {11,}},{118, {11,}},{117, {11,}},{116, {11,}},{115, {11,}},{114, {11,}},{113, {11,}},{112, {11,}},{111, {11,}},{110, {11,}},{97, {11,}},{98, {11,}},{99, {11,}},{100, {11,}},{101, {11,}},{102, {11,}},{103, {11,}},{104, {11,}},{105, {11,}},{106, {11,}},{107, {11,}},{108, {11,}},{109, {11,}},}},},
.m_AcceptStates = {23,},
.m_StartState = {8,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {28,}},}},{18,{{65535, {16,4,}},}},{19,{{65535, {27,}},}},{20,{{97, {21,}},{98, {21,}},{99, {21,}},{100, {21,}},{101, {21,}},{102, {21,}},{103, {21,}},{104, {21,}},{105, {21,}},{106, {21,}},{107, {21,}},{108, {21,}},{109, {21,}},{110, {21,}},{111, {21,}},{112, {21,}},{113, {21,}},{114, {21,}},{115, {21,}},{116, {21,}},{117, {21,}},{118, {21,}},{119, {21,}},{120, {21,}},{121, {21,}},{122, {21,}},}},{21,{{65535, {25,}},}},{22,{{90, {23,}},{89, {23,}},{88, {23,}},{87, {23,}},{86, {23,}},{85, {23,}},{84, {23,}},{83, {23,}},{82, {23,}},{81, {23,}},{80, {23,}},{79, {23,}},{78, {23,}},{65, {23,}},{66, {23,}},{67, {23,}},{68, {23,}},{69, {23,}},{70, {23,}},{71, {23,}},{72, {23,}},{73, {23,}},{74, {23,}},{75, {23,}},{76, {23,}},{77, {23,}},}},{23,{{65535, {25,}},}},{24,{{65535, {20,22,}},}},{25,{{65535, {27,}},}},{27,{{65535, {29,26,}},}},{28,{{65535, {27,26,}},}},{29,{{34, {30,}},}},{0,{{34, {1,}},}},{26,{{65535, {2,24,}},}},{17,{{65535, {19,}},}},{16,{{65535, {14,6,}},}},{15,{{65535, {17,}},}},{2,{{92, {3,}},}},{3,{{65535, {18,}},}},{4,{{110, {5,}},}},{5,{{65535, {19,}},}},{6,{{116, {7,}},}},{7,{{65535, {17,}},}},{8,{{92, {9,}},}},{9,{{65535, {15,}},}},{10,{{34, {11,}},}},{11,{{65535, {12,}},}},{12,{{92, {13,}},}},{13,{{65535, {15,}},}},{14,{{65535, {10,8,}},}},},
.m_AcceptStates = {30,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {26,}},}},{14,{{65535, {10,12,}},}},{13,{{65535, {15,}},}},{12,{{34, {13,}},}},{11,{{65535, {15,}},}},{10,{{65535, {8,6,}},}},{9,{{65535, {11,}},}},{8,{{92, {9,}},}},{7,{{65535, {11,}},}},{6,{{116, {7,}},}},{5,{{65535, {21,}},}},{4,{{110, {5,}},}},{0,{{34, {1,}},}},{26,{{65535, {25,2,}},}},{25,{{65535, {31,2,}},}},{24,{{65535, {20,26,}},}},{21,{{65535, {25,}},}},{20,{{65535, {18,4,}},}},{19,{{65535, {21,}},}},{18,{{65535, {16,14,}},}},{17,{{65535, {19,}},}},{16,{{92, {17,}},}},{3,{{65535, {24,}},}},{15,{{65535, {19,}},}},{2,{{92, {3,}},}},},
.m_AcceptStates = {32,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {10,}},}},{2,{{92, {3,}},}},{3,{{65535, {8,}},}},{8,{{65535, {20,10,}},}},{9,{{65535, {11,2,}},}},{10,{{65535, {9,2,}},}},{11,{{34, {12,}},}},{0,{{34, {1,}},}},},
.m_AcceptStates = {12,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {10,}},}},{2,{{92, {3,}},}},{3,{{65535, {8,}},}},{8,{{65535, {20,10,}},}},{9,{{65535, {15,2,}},}},{10,{{65535, {9,2,}},}},{0,{{34, {1,}},}},},
.m_AcceptStates = {16,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {4,}},}},{2,{{65534, {3,}},}},{3,{{65535, {2,}},}},{4,{{65535, {2,3,}},}},{0,{{35, {1,}},}},},
.m_AcceptStates = {3,},
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
.m_Transitions = {{0,{{10, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{61, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{58, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{44, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{43, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{45, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {2,}},}},{2,{{61, {3,}},}},{0,{{61, {1,}},}},},
.m_AcceptStates = {3,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{62, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {2,}},}},{2,{{61, {3,}},}},{0,{{62, {1,}},}},},
.m_AcceptStates = {3,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{123, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{60, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {2,}},}},{2,{{61, {3,}},}},{0,{{60, {1,}},}},},
.m_AcceptStates = {3,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{40, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{33, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{38, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {2,}},}},{2,{{61, {3,}},}},{0,{{33, {1,}},}},},
.m_AcceptStates = {3,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {2,}},}},{2,{{45, {3,}},}},{0,{{45, {1,}},}},},
.m_AcceptStates = {3,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {2,}},}},{2,{{43, {3,}},}},{0,{{43, {1,}},}},},
.m_AcceptStates = {3,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{125, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{41, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{59, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{47, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{0,{{42, {1,}},}},},
.m_AcceptStates = {1,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
NFASimple{
.m_Transitions = {{1,{{65535, {2,}},}},{2,{{62, {3,}},}},{0,{{45, {1,}},}},},
.m_AcceptStates = {3,},
.m_StartState = {0,},
.m_CurrentStates = {},
.m_PrevHadAcceptState = false
},
};

std::vector<TokenActionItem> Dragonlex::m_ActionItems = {
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "[0-9]",

.m_Type = TokenType::DIGIT,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "[0-9]+",

.m_Type = TokenType::INTEGER,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "[a-zA-Z_]([a-zA-Z0-9_])*",

.m_Type = TokenType::IDENTIFIER,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\\"((\\\\(n|t|\\\\|\\\"\\\\))|[a-zA-Z])*\\\"",

.m_Type = TokenType::STRING,

},
TokenActionItem{
.m_Action = (INPUTACTION)2,

.m_MetaData = "unterm string",

.m_Type = TokenType::ERROR,

},
TokenActionItem{
.m_Action = (INPUTACTION)2,

.m_MetaData = "bad escape in string literal",

.m_Type = TokenType::ERROR,

},
TokenActionItem{
.m_Action = (INPUTACTION)2,

.m_MetaData = "bad escape and unterm string literal",

.m_Type = TokenType::ERROR,

},
TokenActionItem{
.m_Action = (INPUTACTION)1,

.m_MetaData = "",

.m_Type = TokenType::SKIP,

},
TokenActionItem{
.m_Action = (INPUTACTION)1,

.m_MetaData = "",

.m_Type = TokenType::SKIP,

},
TokenActionItem{
.m_Action = (INPUTACTION)1,

.m_MetaData = "",

.m_Type = TokenType::SKIP,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "=",

.m_Type = TokenType::ASSIGN,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = ":",

.m_Type = TokenType::COLON,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = ",",

.m_Type = TokenType::COMMA,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\+",

.m_Type = TokenType::CROSS,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\-",

.m_Type = TokenType::DASH,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "==",

.m_Type = TokenType::EQUALS,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = ">",

.m_Type = TokenType::GREATER,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = ">=",

.m_Type = TokenType::GREATEREQ,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "{",

.m_Type = TokenType::LBRACKET,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "<",

.m_Type = TokenType::LESS,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "<=",

.m_Type = TokenType::LESSEQ,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\(",

.m_Type = TokenType::LPAREN,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "!",

.m_Type = TokenType::NOT,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "&",

.m_Type = TokenType::REF,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "!=",

.m_Type = TokenType::NOTEQUALS,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\-\\-",

.m_Type = TokenType::POSTDEC,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\+\\+",

.m_Type = TokenType::POSINC,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "}",

.m_Type = TokenType::RBRACKET,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\)",

.m_Type = TokenType::RPAREN,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = ";",

.m_Type = TokenType::SEMICOLON,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "/",

.m_Type = TokenType::SLASH,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\*",

.m_Type = TokenType::STAR,

},
TokenActionItem{
.m_Action = (INPUTACTION)3,

.m_MetaData = "\\->",

.m_Type = TokenType::ARROW,

},
};

#endif
