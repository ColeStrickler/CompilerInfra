#include "input.h"
#include "regex_compiler.h"
#include "automata.h"
#include "outfile_writer.h"

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



    std::vector<std::pair<NFA*, TokenActionItem>> outVector;
    for (auto& s: spec)
    {
        //std::cout << s.first << "\n";
        RegexCompiler reCompiler;
        
        NFA* regEx = reCompiler.CompileRegEx(s.first);
        if (regEx == nullptr)
        {
            std::cout << reCompiler.GetErrorString();
            return -1;
        }
        std::pair<NFA*, TokenActionItem> outItem = {regEx, s.second};
        outVector.push_back(outItem);

    }
    
    OutfileWriter fileWriter("./dragonlex.hpp", outVector);
   

    //fileWriter.WriteScannerClassFile();
    
};
