#include "outfile_writer.h"



std::string escapeString(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        switch (c) {
            case '\n': escaped += "\\n"; break;
            case '\t': escaped += "\\t"; break;
            case '\r': escaped += "\\r"; break;
            case '\\': escaped += "\\\\"; break;
            case '\"': escaped += "\\\""; break;
            case '\'': escaped += "\\\'"; break;
            default: 
                // Append the character as-is if it does not need escaping
                escaped += c; 
                break;
        }
    }
    return escaped;
}



OutfileWriter::OutfileWriter(const std::string &outFileName, const std::vector<std::pair<NFA *, TokenActionItem>> regex_actions) : m_File(outFileName), m_RegExActions(regex_actions)
{
    WriteScannerClassFile();
}

void OutfileWriter::WriteScannerClassFile()
{
    std::remove(m_File.c_str()); // delete any old file since we are appending

    AppendFile("#ifndef DRAGONLEX_H");
    AppendFile("#define DRAGONLEX_H");

    AppendFile("#include <vector>");
    AppendFile("#include <string>");
    AppendFile("#include <iostream>");
    AppendFile("#include <set>");
    AppendFile("#include <unordered_set>");
    AppendFile("#include <unordered_map>");
    AppendFile("#define EPSILON (UINT16_MAX)");
    AppendFile("#define DOT_CHAR (UINT16_MAX - 1)");

    AppendFile("class Dragonlex;");

    /*
        TokenType enum
    */

    std::unordered_set<std::string> usedTypes;
    AppendFile("enum TokenType {");
    AppendFile("TOKEN_EOF,");
    for (auto& item: m_RegExActions)
    {
        auto type = item.second.m_TokenName;
        if (usedTypes.count(type) != 0)
            continue;
        AppendFile(type + ",");
        usedTypes.insert(type);
    }
    AppendFile("};\n");


    /* TokenTypeStrings */
    usedTypes.clear();
    AppendFile("static const char* TokenTypeStrings[] = {");
     AppendFile("\"TOKEN_EOF\",");
    for (auto& item: m_RegExActions)
    {
        auto type = item.second.m_TokenName;
        if (usedTypes.count(type) != 0)
            continue;
        AppendFile("\"" + type + "\",");
        usedTypes.insert(type);
    }
    AppendFile("};\n");

    /* Class and Struct declarations */
    AppendFile(inputActions);
    AppendFile("\n" + SimpleNFAClass + "\n");
    AppendFile("\n" + tokActionItem + "\n");
    AppendFile("\n" + posStruct + "\n");
    AppendFile("\n" + tokenStruct + "\n");




    




    AppendFile("class Dragonlex {");
    AppendFile("public:");
    AppendFile("\tDragonlex(const std::string& scanFileContents) : m_CurrentColumn(1), m_CurrentLine(1), m_RawLocation(0), m_FileContents(scanFileContents)\n{\n}");

    AppendFile(scanFunction);

    AppendFile(createTokenFunction);

    AppendFile("private:");
    AppendFile("\tstd::string m_FileContents;");
    AppendFile("\tint m_RawLocation;");
    AppendFile("\tint m_CurrentColumn;");
    AppendFile("\tint m_CurrentLine;");
    AppendFile("\tstatic std::vector<NFASimple> m_NFAs;");
    AppendFile("\tstatic std::vector<TokenActionItem> m_ActionItems;");
    AppendFile("};");



    AppendFile("std::vector<NFASimple> Dragonlex::m_NFAs = {");
    for (auto& item: m_RegExActions)
    {
        AppendFile(item.first->DumpTables());
    }
    AppendFile("};\n");



    AppendFile("std::vector<TokenActionItem> Dragonlex::m_ActionItems = {");
    for (auto& item: m_RegExActions)
    {
        AppendFile("TokenActionItem{");
        AppendFile(".m_Action = (INPUTACTION)" + std::to_string(item.second.m_Action) + ",\n");
        AppendFile(".m_MetaData = \"" + escapeString(item.second.m_MetaData) + "\",\n");
        AppendFile(".m_Type = TokenType::" + item.second.m_TokenName + ",\n");
        
        AppendFile("},");
    }
    AppendFile("};\n");


    AppendFile("#endif");
}

void OutfileWriter::AppendFile(const std::string &msg)
{
    std::ofstream file(m_File, std::ios::app); // Open file in append mode
    if (!file)
    {
        std::cout << "OutfileWriter::AppendFile() error opening file for appending: " << m_File << std::endl;
        return;
    }
    file << msg << std::endl;
}
