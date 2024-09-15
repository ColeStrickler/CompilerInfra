#include "input.h"
#include "regex_compiler.h"
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



    
    for (auto& s: spec)
    {
        std::cout << s.first << "," << s.second << "\n";
        RegexCompiler reCompiler;
        NFA* regEx = reCompiler.CompileRegEx(s.first);
        if(regEx == nullptr)
            std::cout << reCompiler.GetErrorString();
        else
            printf("Compilation successful!\n");



        if (regEx->Accept("18541012412487098777"))
            printf("Test1 Accept!\n");

        if (regEx->Accept("1289aasa"))
            printf("Test2 Accept!\n");
    }
    

};
