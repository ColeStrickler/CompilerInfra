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

    std::string DumpTables();
    std::unordered_map<int, std::unordered_map<uint16_t, std::unordered_set<int>>> m_Transitions;
    std::unordered_set<int> m_AcceptStates;
    std::unordered_set<int> m_StartState;
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
        std::unordered_set<int> current_states = EpsilonClosure(m_CurrentStates, visited);
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
        std::unordered_set<int> newStates;

        for (auto& state: currentState)
        {

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




#endif 