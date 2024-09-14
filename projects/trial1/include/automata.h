#ifndef AUTOMATA_H
#define AUTOMATA_H


enum NFA_OP {
    NFA_CONCAT,
    NFA_OR,
    NFA_STAR
};


class NFA
{
public:
    NFA(char c);
    NFA(NFA* nfa, NFA_OP);
    NFA(NFA* nfa1, NFA* nfa2, NFA_OP);
};





#endif 