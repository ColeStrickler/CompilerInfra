#include "input.h"
#include "scanner.hpp"


#include "automata.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: dragonlex <input file>\n";
        return 0;
    }


    ScannerInputSorter input(argv[1]);
    if (!input.ParseLines())
    {
        std::cout << "Invalid spec file.\n";
        return -1;
    }
    auto spec = input.GetRegexes();



    
    //for (auto& s: spec)
    //{
    //    std::cout << s.first << "," << s.second << "\n";
    //    RegexScanner reScanner(s.first);
    //    if (!reScanner.ScanRegex())
    //    {
    //        std::cout << reScanner.GetErrorString() << "\n";
    //        return -1;
    //    }
    //    auto tokens = reScanner.GetTokens();
    //    for (auto& tok: tokens)
    //    {
    //        std::cout << tok.toString() << "\n";
    //    }
    //    std::cout << "\n\n";
    //}
    NFA nfa_x('x');
    NFA nfa_a('a');
    NFA nfa_x2('x');
    NFA nfa_a2('a');

    NFA starNFAx(&nfa_x, NFA_OP::NFA_STAR);
    NFA starNFAa(&nfa_a, NFA_OP::NFA_STAR);

    NFA starNFAx2(&nfa_x2, NFA_OP::NFA_STAR);
    NFA starNFAa2(&nfa_a2, NFA_OP::NFA_STAR);
//
    //if (starNFAx.Accept("xxxxxxxxx"))
    //    printf("ACCEPTED star x\n");
    //else
    //    printf("DENIED\n");
    //if (starNFAa.Accept("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"))
    //    printf("ACCEPTED star a\n");
    //else
    //    printf("DENIED\n");
//
//
    NFA orNFAxa(&starNFAa, &starNFAx, NFA_OP::NFA_OR);
    NFA orNFAxa2(&starNFAa2, &starNFAx2, NFA_OP::NFA_OR);

    if (orNFAxa.Accept("aaaaaa"))
        printf("OR NFA Accepted As\n");
    else
        printf("OR NFA denied As\n");

    if (orNFAxa.Accept("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"))
        printf("OR NFA Accepted Xs\n");
    else
        printf("OR NFA denied Xs\n");

    NFA testNFA(&orNFAxa, &orNFAxa2, NFA_OP::NFA_CONCAT);
    NFA testNFA2 = testNFA;
    printf("here\n");

    if (testNFA.Accept("aaaaaaaaaaaaaaaaaaaaaaaaxxxx"))
        printf("testNFA accepted!\n");
    else
        printf("test NFA denied :(\n");


    NFA finalNFA(&testNFA, &testNFA2, NFA_OP::NFA_CONCAT);
    NFA fNFA('z');
    NFA final2NFA(&fNFA, NFA_OP::NFA_PLUS);
    if (finalNFA.Accept("xxxxxaaaaaaaaaxxxx"))
        printf("Final accept1\n");
    if (finalNFA.Accept("aaaaxxxxaaaaxxxx"))
        printf("Final accept2\n");
    if (finalNFA.Accept("xxxxaaaaxxxxaaaa"))
        printf("Final accept3\n");
    if (finalNFA.Accept("aaaaxxxxxxxx"))
        printf("Final accept4\n");


    if (final2NFA.Accept(""))
        printf("Final2 accept2\n");
};
