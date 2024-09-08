
#include <fstream>
#include <stdint.h>
#include "scanner.hpp"
#include "parser.hpp"

int main(int argc, char** argv)
{
    std::ifstream * infile = new std::ifstream(argv[1]);
    Scanner::Scanner* scanner = new Scanner::Scanner(infile);
    bool scanning = true;

    while(scanning)
    {
        auto scan_err = scanner->produceToken();
        if (scan_err == TOKENTYPE::TOKEN_ERR)
        {
            auto last = scanner->GetLastToken();
            std::cerr << last->toString();
            exit(-1);
        }
        if (scan_err == TOKENTYPE::TOKEN_EOF)
            scanning = false;

    };


    auto tokens = scanner->GetTokens();

    Parser* parser = new Parser(tokens);
    RootNode* rnode = parser->Parse();

    printf("Parse successful\n");

    if (rnode == nullptr)
    {
        printf("Parser::Parse() unsuccessful.\n");
    }
    else {
        printf("Eval: %.3f\n", rnode->translate());
    }


}