#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <stdint.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#define EPSILON (UINT16_MAX)
#define DOT_CHAR (UINT16_MAX - 1)


enum NFA_OP {
    NFA_CONCAT,
    NFA_OR,
    NFA_STAR,
    NFA_PLUS,
    NFA_QMARK
};


class NFA
{
public:
    NFA(uint16_t c);
    NFA(uint16_t rangeStart, uint16_t rangeEnd);
    NFA(NFA* nfa, NFA_OP star_plus_qmark); // star 
    NFA(NFA* nfa1, NFA* nfa2, NFA_OP or_concat);
    NFA(const NFA& other);
    void PlusConstructor(NFA* nfa);
    void StarConstructor(NFA* nfa);
    void QmarkConstructor(NFA* nfa);
    void OrConstructor(NFA* nfa1, NFA* nfa2);
    void ConcatConstructor(NFA* nfa1, NFA* nfa2);

    void IncStates(int incAmount);
    bool Accept(const std::string& str);


private:
    
    int m_TotalStates;
    std::unordered_set<int> EpsilonClosure(std::unordered_set<int> states,  std::unordered_set<int>& visited);
    std::unordered_set<int> TransitionFunction(std::unordered_set<int> currentState, uint16_t character);
    bool ContainsAcceptState(std::unordered_set<int> states);


    void ConsumeTransitionTable(const std::unordered_map<int, std::unordered_map<uint16_t, std::unordered_set<int>>>& trans_table);


    /*
        We use UINT16_T as the input as the transition function so we can support all ascii characters
        and still use a special character for epsilon
    */
    std::unordered_map<int, std::unordered_map<uint16_t, std::unordered_set<int>>> m_Transitions;
    std::unordered_set<int> m_AcceptStates;
    std::unordered_set<int> m_StartState;

};


#endif 