#include "input.h"

#include "parse_table.h"



int main(int argc, char** argv)
{
    InputSorter sorter(argv[1]);
    if(!sorter.ParseLines())
    {
        std::cout << "sorter.ParseLines() --> Parse Error.\n";
        return -1;
    }
    std::cout << "Init Section:\n" << sorter.GetInitSection() << "\n";
    std::cout << "Productions:\n";
    auto rules = sorter.GetProdRules();
    for (auto& rule: rules)
        rule.Print();
    std::cout << "Actions:\n";
    sorter.PrintActions();

    auto initSection = sorter.GetInitSection();
    auto prodRules = sorter.GetProdRules();
    auto actions = sorter.GetActions();

    ParseTable table(prodRules, actions, initSection, sorter.m_ProdMapping);
    if(!table.ConstructParseTable())
        return -1;
    //std::string nonTerminals[] = {
    //    "EXPR",
    //    "EXPRTAIL",
    //    "TERM",
    //    "TERMTAIL",
    //    "FACTOR"
    //};

    // Print First and Follow sets for each non-terminal
    //for (const std::string& name : nonTerminals) {
    //    table.PrintFirst(name);
    //    table.PrintFollow(name);
    //}

    table.DumpFile("./dragonsdt.cpp");
}