#include "parse_table.h"



ParseTable::ParseTable(const std::vector<ProductionRule>& prodRules, std::unordered_map<std::string,std::string>& actions, std::string init, std::unordered_map<std::string, RHSType> mapping) : m_Actions(actions), m_InitSection(init), m_ProdMapping(mapping)
{
    for (auto& rule: prodRules)
    {
        m_ProdRules[rule.m_LHS].push_back(rule);


        /*
            These will be used for producing follow sets
        */

        for (int i = 0; i < rule.m_RHS.size(); i++)
        {
            int start_i = i;
            while(i+1 < rule.m_RHS.size() && rule.m_RHS[i+1].type == RHSType::ACTION) // skip actions
                i++;
            if (i+1 < rule.m_RHS.size())
                m_DirectlyFollowing[rule.m_RHS[start_i].obj].push_back(DirectFollow{rule.m_LHS, rule.m_RHS[i+1].obj, rule.m_RHS[i+1].type});
            else
                m_DirectlyFollowing[rule.m_RHS[start_i].obj].push_back(DirectFollow{rule.m_LHS, "", RHSType::EPSILON}); // this allows us to still process the lhs
        }
    }
        
    //printf("1\n");
    /*
        Construct all first sets
    */
    for (int i = 0; i < prodRules.size(); i++)
    {
        if (m_FirstSets.count(prodRules[i].m_LHS) == 0) // keep us from processing ones we will get the first time through
        {
            auto first_set = ConstructFirstSets(prodRules[i].m_LHS, 0);
            m_FirstSets[prodRules[i].m_LHS].insert(first_set.begin(), first_set.end());
        }
    }
   // printf("2\n");


    // must keep start symbol
    m_StartSymbol = prodRules[0].m_LHS;
    /*
        Construct all follow sets
    */
  
    //printf("first %s %d\n", prodRules[0].m_LHS.c_str(), prodRules.size());
    m_FollowSets[prodRules[0].m_LHS].insert("EOF"); // the only item in follow set for the start symbol is EOF
    for (int i = 0; i < prodRules.size(); i++)
    {
         //printf("touch %s %d\n", prodRules[i].m_LHS.c_str(), m_ProdRules.size());
        if (m_FollowSets.count(prodRules[i].m_LHS) == 0) // keep us from processing ones we will get the first time through
        {
            auto follow_set = ConstructFollowSets(prodRules[i].m_LHS);

            m_FollowSets[prodRules[i].m_LHS].insert(follow_set.begin(), follow_set.end());
           // m_FirstSets[prodRules[i].m_LHS].insert(first_set.begin(), first_set.end());
        }

           
    }

   // printf("3\n");
    
}

std::unordered_set<std::string> ParseTable::ConstructFollowSets(std::string nonterminal)
{  
   // printf("START followset %s\n", nonterminal.c_str());
    if (m_ProdRules.count(nonterminal) == 0)
    {
       // printf("followset is term%s\n", nonterminal.c_str());
        if (nonterminal == "epsilon")
            return {};
        else
            return {nonterminal};
    }


    std::unordered_set<std::string> ret;
    for (auto& follow: m_DirectlyFollowing[nonterminal])
    {
        auto& follower = follow.follow;
        //printf("non term %s  ---> follower: %s\n", nonterminal.c_str(), follower.c_str());
        if (follow.type == RHSType::TERMINAL)
        {
            //printf("followset term %s\n", nonterminal.c_str());
            ret.insert(follower);
        }
        else if (m_FirstSets.find(follower) != m_FirstSets.end() && m_FirstSets[follower].size())
        {
           // printf("followset follower first%s --> FOLLOWED BY %s\n", nonterminal.c_str(), follower.c_str());
            ret.insert(m_FirstSets[follower].begin(), m_FirstSets[follower].end());
            ret.erase("epsilon");
        }
        else
        {
           // printf("here lhs %s follow rhs %s\n", follow.lhs.c_str(), nonterminal.c_str());

            if (follow.lhs == nonterminal)
            {
                ret.insert( m_FirstSets[follow.lhs].begin(), m_FirstSets[follow.lhs].end());
                continue;
            }
            auto lhs_follow_set = ConstructFollowSets(follow.lhs);
            if (m_FollowSets.count(follow.lhs) != 0) // corner case for EOF
            {
                if (m_FollowSets[follow.lhs].find("EOF") != m_FollowSets[follow.lhs].end())
                    lhs_follow_set.insert("EOF");
            }

            ret.insert(lhs_follow_set.begin(), lhs_follow_set.end());
        }
    }



    return ret;
}

void ParseTable::PrintFirst(std::string nonterm)
{
    std::cout << "First " << nonterm << ": ";
    for (auto& f: m_FirstSets[nonterm])
        std::cout<< f << ",";

    std::cout << "\n";
}

void ParseTable::PrintFollow(std::string nonterm)
{
    std::cout << "Follow " << nonterm << ": ";
    for (auto& f: m_FollowSets[nonterm])
        std::cout<< f << ",";

    std::cout << "\n";
}


std::unordered_set<std::string> ParseTable::ConstructFirstSets(std::string nonterminal, int i)
{
    if (m_ProdRules.count(nonterminal) == 0)
        return {nonterminal}; // this should get epsilon and terminals
    

    std::unordered_set<std::string> ret;
    bool nullable = true;
    bool rhs_contains_epsilon = false;
    for (auto& prod: m_ProdRules[nonterminal])
    {
        for (auto& rhs : prod.m_RHS)
        {
            if (rhs.type == RHSType::ACTION)
                continue;
            if (rhs.obj == "epsilon")
                rhs_contains_epsilon = true;

            auto firstAn = ConstructFirstSets(rhs.obj, i+1);
            for (auto& s : firstAn)
            {

                 m_ProdRuleFirstSets[nonterminal][s] = prod;
            }

            if (firstAn.count("epsilon") == 0)
            {
                ret.insert(firstAn.begin(), firstAn.end());
                nullable = false;
                break;
            }
            else
            {
                firstAn.erase("epsilon");
                ret.insert(firstAn.begin(), firstAn.end());
            } 
        }
    }

    if (nullable || rhs_contains_epsilon)
        ret.insert("epsilon");

    return ret;

}


bool ParseTable::ConstructParseTable()
{

    for (auto& p : m_ProdRules)
    {
        for (auto& first: m_FirstSets[p.first])
        {
            if (first == "epsilon")
            {
                for (auto& follow: m_FollowSets[p.first])
                {
                    //if (m_ParseTable[p.first].count(follow) != 0) 
                    //{
                    //    std::cout << "ParseTable::ConstructParseTable() conflicting first sets found _1.\n";
                    //    std::cout << p.first << "," << follow << "\n";
                    //    return false;
                    //}
                    
                    if(m_ParseTable[p.first][follow].AddIfNeeded(RHSObject(m_ProdMapping[follow], follow)))
                    {
                         //std::cout << "Inserting ParseTable1: "  << p.first << "," << follow << " "  << m_ProdRuleFirstSets[p.first][follow].toString() << "\n";
                    }
                }
            }
            else
            {
                //if (m_ParseTable[p.first].count(first) != 0) 
                //{
                //    std::cout << "ParseTable::ConstructParseTable() conflicting first sets found _2.\n";
                //    return false;
                //}
                //std::cout << "Inserting ParseTable2: "  << p.first << "," << first << " "  << m_ProdRuleFirstSets[p.first][first].toString() << "\n";
                m_ParseTable[p.first][first] = m_ProdRuleFirstSets[p.first][first];
            }
        }
    }
    return true;
}

std::string ParseTable::BuildActionTable()
{
    std::string table;

    table += "void runAction(std::string action, Token* t){\n";
    table += "switch(atoi(action.c_str())){\n";
    for (auto& action : m_Actions)
    {
        std::string num = action.first.substr(1);

        table += "case " + num + ":\n";
        table += "{\n" + action.second + "\nbreak;\n}\n";
    }
    table += "}";
    table += "}\n";
    return table;
}

std::string ParseTable::BuildParseTable()
{
    std::string parse_table;
    parse_table += "{";
    for (auto& r : m_ParseTable)
    {
        parse_table += "{";
        parse_table +=  "\"" + r.first + "\"" + ", ";
        parse_table += "{";
        for (auto& e : r.second)
        {
            parse_table += "{";
            parse_table += "\"" + e.first + "\"" + "," + e.second.toString();
            parse_table += "},";
        }
        parse_table += "}";
        parse_table += "},\n";
    }

    parse_table += "};";

    return parse_table;
}

void ParseTable::DumpFile(const std::string &file)
{
    m_File = file;
    std::remove(m_File.c_str()); // delete any old file since we are appending
    SetTabs(0);
    AppendFile("#include <sstream>\n#include <fstream>\n#include <iostream>\n#include <string>\n#include <vector>\n#include <stack>\n#include <unordered_map>\n");
    AppendFile(m_InitSection);
    AppendFile("int lineNo = 1;\n");
    
    
    AppendFile(ParseTableDefinition);
    AppendFile(BuildParseTable());
    AppendFile(TokenStruct);
    AppendFile(BuildActionTable());
    AppendFile(SplitOnColonFunc);
    AppendFile(GetFirstWordFunc);
    AppendFile(ReadTokenFromInputFileFunc);
    AppendFile("std::stack<RHSObject> parseStack;\n");


    AppendFile(HandleTerminalFunc);
    AppendFile(HandleNonTerminalFunc);
    AppendFile(HandleEpsilonFunc);

    AppendFile("int main(int argc, char** argv) {\n");
    SetTabs(1);
    AppendFile("if (argc < 1){std::cout << \"Usage: dragonsdt <token stream>\"; return 0;}\n");
    AppendFile(OpenInputFile);
    AppendFile("Token* t = nullptr;\n");
    AppendFile("RHSObject startSymbol = RHSObject(RHSType::NONTERMINAL, \"" + m_StartSymbol +"\");");

    AppendFile("parseStack.push(startSymbol);\n");
    AppendFile("while(!parseStack.empty())\n\t{\n");
    AppendFile("t = ReadToken(fs);\n");
    AppendFile("//std::cout << t->tokenType << \",\" << t->lexeme << \"\\n\";\n");
    AppendFile(MainLoopBody);
    AppendFile("}\n");

    AppendFile("if (parseStack.size()){std::cout << \"parse unsuccessful as \" << lineNo << \"\\n\"; return -1;}else {printf(\"parse successful!\\n\");}");
    SetTabs(0);
    AppendFile("}\n");


}

void ParseTable::AppendFile(std::string msg)
{
    std::ofstream file(m_File, std::ios::app); // Open file in append mode
    if (!file)
    {
        std::cout << "OutfileWriter::AppendFile() error opening file for appending: " << m_File << std::endl;
        return;
    }
    for (int t = 0; t < numTabs; t++)
        file << "\t";

    file << msg << std::endl;
}
