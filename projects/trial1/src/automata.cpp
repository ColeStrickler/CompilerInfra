#include "automata.h"

NFA::NFA(uint16_t c) 
{
    m_StartState.insert(0);
    m_AcceptStates.insert(1);
    m_Transitions[0][c].insert(1);
    m_TotalStates = 2;
    //printf("totalStates %d\n", m_TotalStates);
}

NFA::NFA(uint16_t rangeStart, uint16_t rangeEnd)
{
    printf("NFA::RangeConstructor\n");
    m_StartState.insert(0);
    m_AcceptStates.insert(1);
    uint16_t beg = std::min(rangeStart, rangeEnd);
    uint16_t end = std::max(rangeStart, rangeEnd);

    printf("beg: %d, end: %d\n", beg, end);

    for (uint16_t i = beg; i <= end; i++)
    {
       // printf("RangeConstructor: i %d\n", i);
        m_Transitions[0][i].insert(1);
    }

    printf("Constructed ranged NFA\n");
    m_TotalStates = 2;
}

NFA::NFA(NFA *nfa, NFA_OP star_plus_qmark)
{
    switch(star_plus_qmark)
    {
        case NFA_OP::NFA_STAR: StarConstructor(nfa); break;
        case NFA_OP::NFA_PLUS: PlusConstructor(nfa); break;
        case NFA_OP::NFA_QMARK: QmarkConstructor(nfa); break;
        default:
        {
            std::cout << "NFA::NFA() expected NFA_OP == NFA_OP::NFA_STAR || NFA_OP == NFA_OP::NFA_PLUS\n";
            return;
        }
    }
}


void NFA::PlusConstructor(NFA* nfa)
{
     m_Transitions.clear();

    m_TotalStates = nfa->m_TotalStates; // we start with the old states
    m_StartState.insert(m_TotalStates); // add a new start state to the beginning

    // copy the old date we need
    m_AcceptStates = nfa->m_AcceptStates;
    ConsumeTransitionTable(nfa->m_Transitions);



    /*
        For + operator we do not automatically add an epsilon transition to accept because it is 1 or more
        m_Transitions[m_TotalStates][EPSILON] = m_AcceptStates; // because we can accept zero or more
    */
    m_Transitions[m_TotalStates][EPSILON].insert(nfa->m_StartState.begin(), nfa->m_StartState.end()); // newStart -> oldStart 
    for (auto& accept : m_AcceptStates)
    {
        m_Transitions[accept][EPSILON].insert(nfa->m_StartState.begin(), nfa->m_StartState.end()); // this the * 1 or more times
    }

    printf("Constructed + NFA\n");
    m_TotalStates += 1;
}

void NFA::StarConstructor(NFA* nfa)
{
    m_Transitions.clear();

    m_TotalStates = nfa->m_TotalStates; // we start with the old states
    m_StartState.insert(m_TotalStates); // add a new start state to the beginning

    // copy the old date we need
    m_AcceptStates = nfa->m_AcceptStates;
    ConsumeTransitionTable(nfa->m_Transitions);

    m_Transitions[m_TotalStates][EPSILON] = m_AcceptStates; // because we can accept zero or more
    m_Transitions[m_TotalStates][EPSILON].insert(nfa->m_StartState.begin(), nfa->m_StartState.end()); // newStart -> oldStart 
    for (auto& accept : m_AcceptStates)
    {
        m_Transitions[accept][EPSILON].insert(nfa->m_StartState.begin(), nfa->m_StartState.end()); // this the * 1 or more times
    }

    m_TotalStates += 1;
}

void NFA::QmarkConstructor(NFA *nfa)
{
    
    m_Transitions.clear();
    m_TotalStates = nfa->m_TotalStates;
    m_StartState.insert(m_TotalStates);
    m_AcceptStates.insert(m_TotalStates+1);

    ConsumeTransitionTable(nfa->m_Transitions);

    /*
        We need epsilon transitions from the new start state to the old ones
    */
    std::unordered_set<int> oldStart = nfa->m_StartState;
    for (auto& s: oldStart)
        m_Transitions[m_TotalStates][EPSILON].insert(s);


    /*
        We need an epsilon transition from the new start to the new accept
    */
    m_Transitions[m_TotalStates][EPSILON].insert(m_TotalStates+1);

    /*
     We need epsilon transitions from the old accept states to the new accept state
    */
    std::unordered_set<int> oldAccept = nfa->m_AcceptStates;
    for (auto& s: oldAccept)
        m_Transitions[s][EPSILON].insert(m_TotalStates+1);

    m_TotalStates += 2;
}

void NFA::OrConstructor(NFA * nfa1, NFA * nfa2)
{
    m_Transitions.clear();
    auto nfa1NumStates = nfa1->m_TotalStates;
    auto nfa2NumStates = nfa2->m_TotalStates;
    
    /*
        Because we nfa1 and nfa2 will have overlapping state names, we increment every state name
        in nfa2 to avoid state name reuse
    */
    nfa2->IncStates(nfa1NumStates);


    m_TotalStates = nfa1NumStates + nfa2NumStates; // we start with both the old states
    //printf("totalStates %d\n", m_TotalStates);
    m_StartState.insert(m_TotalStates); // add a new start state to the beginning
    m_AcceptStates.insert(m_TotalStates+1); // add a new accept state at the end


    /*
        Build the transition trable from both nfa1 and nfa2
    */
    ConsumeTransitionTable(nfa1->m_Transitions);
    ConsumeTransitionTable(nfa2->m_Transitions);



    // these are the states that the new start state will need an epsilon transition to
    
    std::unordered_set<int> startEpsilonTransitions = nfa1->m_StartState;
    startEpsilonTransitions.insert(nfa2->m_StartState.begin(), nfa2->m_StartState.end());
    m_Transitions[m_TotalStates][EPSILON].insert(startEpsilonTransitions.begin(), startEpsilonTransitions.end());

    
    // take the old accept states and have them both epsilon transition to a new accept state
    std::unordered_set<int> endEpsilonTransitions = nfa1->m_AcceptStates;
    endEpsilonTransitions.insert(nfa2->m_AcceptStates.begin(), nfa2->m_AcceptStates.end());
    for (auto& s: endEpsilonTransitions)
        m_Transitions[s][EPSILON].insert(m_TotalStates+1);


    m_TotalStates += 2;
}

void NFA::ConcatConstructor(NFA * nfa1, NFA * nfa2)
{
    m_Transitions.clear();
    auto nfa1NumStates = nfa1->m_TotalStates;
    auto nfa2NumStates = nfa2->m_TotalStates;
    
    /*
        Because we nfa1 and nfa2 will have overlapping state names, we increment every state name
        in nfa2 to avoid state name reuse
    */
    nfa2->IncStates(nfa1NumStates);


    m_TotalStates = nfa1NumStates + nfa2NumStates; // we start with both the old states
    //printf("totalStates %d\n", m_TotalStates);
    m_StartState.insert(nfa1->m_StartState.begin(), nfa1->m_StartState.end()); // we can re-use nfa1's start states
    m_AcceptStates.insert(nfa2->m_AcceptStates.begin(), nfa2->m_AcceptStates.end()); // we can re-use nfa2's accept states

    /*
        Build the transition trable from both nfa1 and nfa2
    */
    ConsumeTransitionTable(nfa1->m_Transitions);
    ConsumeTransitionTable(nfa2->m_Transitions);


    /*
        now we just need to add an epsilon transition from nfa1's accept states to the start states of nfa2
    */
    auto nfa1_accept = nfa1->m_AcceptStates;
    auto nfa2_start = nfa2->m_StartState;
    for (auto& state: nfa1_accept)
        m_Transitions[state][EPSILON].insert(nfa2_start.begin(), nfa2_start.end());

    /*
        there is no need to increment states here like in the other operators because we did not add any
    */

}


NFA::NFA(NFA *nfa1, NFA *nfa2, NFA_OP or_concat)
{
    switch(or_concat)
    {
        case NFA_OP::NFA_CONCAT: ConcatConstructor(nfa1, nfa2); break;
        case NFA_OP::NFA_OR: OrConstructor(nfa1, nfa2); break;
        default:
        {
            std::cout << "NFA::NFA() expected NFA_OP == NFA_OP::NFA_CONCAT || NFA_OP == NFA_OP::NFA_OR\n";
            return;
        }
    }
}


/*
    copy constructor
*/
NFA::NFA(const NFA &other)
{
    m_StartState.clear();
    m_AcceptStates.clear();
    m_Transitions.clear();


    m_StartState.insert(other.m_StartState.begin(), other.m_StartState.end());
    m_AcceptStates.insert(other.m_AcceptStates.begin(), other.m_AcceptStates.end());
    ConsumeTransitionTable(other.m_Transitions);
    m_TotalStates = other.m_TotalStates; 
}



void NFA::IncStates(int incAmount)
{
    // increment start states
    std::unordered_set<int> newStarts;
    for (auto& s : m_StartState)
    {
        newStarts.insert(s + incAmount);
    }
    m_StartState = newStarts;

    // increment accept states
    std::unordered_set<int> newAccepts;
    for (auto& s : m_AcceptStates)
    {
        newAccepts.insert(s + incAmount);
    }
    m_AcceptStates = newAccepts;
    
    // increment all transition functions
    std::unordered_map<int, std::unordered_map<uint16_t, std::unordered_set<int>>> newTransitions;
    for (int i = 0; i < m_TotalStates; i++)
    {
        for (auto& transition : m_Transitions[i])
        {
            auto targetSet = transition.second;
            for (auto &target: targetSet)
            {
                newTransitions[i + incAmount][transition.first].insert(target + incAmount);
            }
        }
    }
    m_Transitions = newTransitions;

}

bool NFA::Accept(const std::string &str)
{
    std::unordered_set<int> visited;
    std::unordered_set<int> current_states = EpsilonClosure(m_StartState, visited);
    //for (auto& s: current_states)
    //    printf("start state: %d\n", s);
    for (int i = 0; i < str.size(); i++)
    {
        visited.clear();
        current_states = EpsilonClosure(TransitionFunction(current_states, str[i]), visited);
        //for (auto& s: current_states)
        //    printf("second state: %d\n", s);
        if (current_states.size() == 0)
            return false; // did not accept
    }

    
    if (ContainsAcceptState(current_states))
        return true;
    return false;
}

std::unordered_set<int> NFA::EpsilonClosure(std::unordered_set<int> states, std::unordered_set<int>& visited)
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

std::unordered_set<int> NFA::TransitionFunction(std::unordered_set<int> currentState, uint16_t character)
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
    //printf("TransitionFunction() %d--> newStates.size() == %d\n", character, newStates.size());
    return newStates;
}

bool NFA::ContainsAcceptState(std::unordered_set<int> states)
{
    for (auto& state: states)
    {
        if (m_AcceptStates.count(state) != 0)
            return true;
    }
    return false;
}

void NFA::ConsumeTransitionTable(const std::unordered_map<int, std::unordered_map<uint16_t, std::unordered_set<int>>>& trans_table)
{
    for (auto& entry: trans_table)
    {
        for (auto& tfunc : entry.second)
        {
            m_Transitions[entry.first][tfunc.first].insert(tfunc.second.begin(), tfunc.second.end());
        }
    }
}
