#include "input.h"
#include "scanner.hpp"




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



    ;
    for (auto& s: spec)
    {
        std::cout << s.first << "," << s.second << "\n";
        RegexScanner reScanner(s.first);
        if (!reScanner.ScanRegex())
        {
            std::cout << reScanner.GetErrorString() << "\n";
            return -1;
        }
        auto tokens = reScanner.GetTokens();
        for (auto& tok: tokens)
        {
            std::cout << tok.toString() << "\n";
        }
        std::cout << "\n\n";
    }


};
