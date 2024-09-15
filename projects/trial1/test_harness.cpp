#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "dragonlex.hpp"



std::string readFileToString(const std::string& filename) {
    // Open the file in binary mode
    std::ifstream file(filename, std::ios::binary);

    // Check if the file was opened successfully
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    // Read the file contents into a string stream
    std::ostringstream fileStream;
    fileStream << file.rdbuf();

    // Return the contents as a std::string
    return fileStream.str();
}
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "dragonlextest <file_to_scan>\n";
        return 0;
    }


    std::string fileContents = readFileToString(argv[1]);
    if (!fileContents.size())
        return 0;

    Dragonlex* lexer = new Dragonlex(fileContents);
    

    while(true)
    {
        auto token = lexer->ScanToken();

       // printf("token %x\n", token);
        if (token == nullptr)
            continue;
        if (token->m_Type == TokenType::TOKEN_EOF)
            break;
        
        std::cout << token->toString();
    }

    

}