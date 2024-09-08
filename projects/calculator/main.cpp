
#include <fstream>
#include <stdint.h>
#include <sstream>
#include "scanner.hpp"
#include "parser.hpp"


void interpret(std::istream* in)
{
    Scanner::Scanner* scanner = new Scanner::Scanner(in);
    bool scanning = true;

    while(scanning)
    {
        auto scan_err = scanner->produceToken();
        if (scan_err == TOKENTYPE::TOKEN_ERR)
        {
            auto last = scanner->GetLastToken();
            if (last == nullptr)
                std::cerr << "Scanner error. no tokens\n";
            else
                std::cerr << last->toString() << "\n";
            return;
        }
        if (scan_err == TOKENTYPE::TOKEN_EOF)
            scanning = false;

    };


    auto tokens = scanner->GetTokens();

    Parser* parser = new Parser(tokens);
    RootNode* rnode = parser->Parse();



    if (rnode == nullptr)
    {
        printf("Parser::Parse() unsuccessful.\n");
    }
    else {
        printf("Parse successful\n");
        printf("Eval: %.3f\n", rnode->translate());
    }
    //delete parser;
    //delete scanner;
}



int main(int argc, char** argv)
{
    std::stringstream is;
    std::string line;
    while(true)
    {
        std::cout << "\n>> ";
        while(std::getline(std::cin, line) && !line.empty())
        {
            is << line;
            interpret(&is);
            is.clear();
        }
    }

}