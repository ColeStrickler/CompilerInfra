#include "parse_table.h"

std::vector<std::string> splitOnColon(const std::string& input) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end;

    // Find the position of each colon
    while ((end = input.find(':', start)) != std::string::npos) {
        // Extract the substring from start to end
        result.push_back(input.substr(start, end - start));
        start = end + 1; // Move past the colon
    }

    // Add the last substring after the last colon (if any)
    result.push_back(getFirstWord(input.substr(start)));

    return result;
}


std::string getFirstWord(const std::string& input) {
    std::istringstream iss(input);
    std::string firstWord;
    
    // Extract the first word
    iss >> firstWord;
    return firstWord;
}

Token* ReadToken(std::stringstream& file)
{
    std::string line;
    if (std::getline(file, line))
    {
        if (line.find(':') != std::string::npos) // case 1
        {
            auto v = splitOnColon(line);
            return new Token(v[0], v[1]);
        }
        else // case 2
        {
            auto t = getFirstWord(line);
            return new Token(t, t);
        }      
    }
    return new Token("EOF", "EOF");
}




ParseTable::ParseTable(const std::vector<ProductionRule> &grammar) : m_ProdRules(grammar), m_StateStack({}), m_SymbolStack({})
{
    std::remove("./parser.dot"); // delete any old file since we are appending

    for (auto& rule: m_ProdRules)
    {
        m_Grammar[rule.m_LHS].push_back(rule);
        m_Symbols.insert(rule.m_LHS);
        m_NonTerminals.insert(rule.m_LHS);
        for (auto& rhs: rule.m_RHS)
            m_Symbols.insert(rhs.obj);
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

    for (auto& rule: m_ProdRules)
    {
        for (auto& rhs: rule.m_RHS)
        {
            if (m_NonTerminals.count(rhs.obj) == 0)
            {
                m_Terminals.insert(rhs.obj);   
            }
        }
    }
    for (auto& nt: m_NonTerminals)
    {
        std::unordered_set<std::string> seen;
        m_FirstSets[nt] = ConstructFirstSets(nt, seen);
        //std::cout << "First set: " << nt << " ";
        //for (auto& x: m_FirstSets[nt])
        //    std::cout << " ," << x; 
        //std::cout << "\n\n\n";
    }


    for (auto& nt: m_NonTerminals)
    {
        m_FollowSets[nt] = ConstructFollowSets(nt);
    }

        


    ProduceStartItem(m_ProdRules[0]);
    ProduceAcceptItem();
    CanonicalSetConstruction();

    
    //ProduceItems();
    //ProduceClosureSets();
}


void ParseTable::ProduceStartItem(ProductionRule rule)
{
    //std::cout << "Start rule: " << rule.toString() << "\n";
    m_StartItem = LR0_Item{rule.m_LHS, 0, 0};
}

bool ParseTable::CanBeEpsilon(std::string sym, std::unordered_set<std::string>& seen)
{
    //printf("true %s,%d\n", sym.c_str(), sym.size());
    
    if (sym == ".")
    {
        return true;
    }
    if (isTerminal(sym))
        return false;
    if (seen.count(sym))
        return false;
    

    for (auto& p : m_Grammar[sym])
    {
       // seen.insert(sym);
        for (auto& r : p.m_RHS)
        {
            if (r.obj == sym)
                break;
            if (CanBeEpsilon(r.obj, seen))
                return true;
            else
                break;
        }
       // seen.erase(sym);
    }

    //seen.insert(sym);
    return false;
}

void ParseTable::ProduceAcceptItem()
{
    auto rule = m_Grammar[m_StartItem.m_LHS][m_StartItem.m_ProdRuleIndex];

    m_AcceptItem = LR0_Item{m_StartItem.m_LHS, (int)rule.m_RHS.size(), m_StartItem.m_ProdRuleIndex}; // S' -> S.
}

void ParseTable::CanonicalSetConstruction()
{
    auto x = Closure(m_StartItem);
    C.insert({x, 0});
    rC.insert({0, x});
    std::unordered_set<std::unordered_set<LR0_Item, PairHash>, SetHash, SetEqual> set;
    set.insert(x);
    std::vector<std::unordered_set<LR0_Item, PairHash>> states;
    states.push_back(x);

    while(true)
    {
        int added = 0;
        auto size = states.size();
        for (int i = 0; i < size; i++)
        {
            //printf("here\n");
            auto itemset = states[i];
           // printf("itemSet %d\n", itemset.size());
            for (auto& symbol: m_Symbols)
            {   
                // PrintItemSet(itemset);
                // printf("here\n\n\n");
                bool reduce = false;
                auto goTo_X = GoTo(itemset, symbol, &reduce);
                //printf("gotoX size: %d\n", goTo_X.size());
                if (goTo_X.size() && set.count(goTo_X) == 0)
                {
                    set.insert(goTo_X);
                  // printf("here\n");
                    states.push_back(goTo_X);
                    added++;
                    C[goTo_X] = states.size();
                    rC[states.size()] = goTo_X;
                }
            }
        }


        if (added == 0)
            break;
    }
   // printf("STATES SIZE: %d\n")
    if(ConstructActionTables())
        m_BadGrammar = true;
    else
        m_BadGrammar = false;
   // m_C = C;
}


bool ParseTable::ConstructActionTables()
{
    for (auto& i: rC)
    {
        //printf("RC\n");

        m_ActionTable[0]["EOF"] = ParseAction(P_ACTION::ACCEPT);

        for (auto& entry: i.second)
        {
            auto rule = m_Grammar[entry.m_LHS][entry.m_ProdRuleIndex];
            if (entry.m_DotLoc == rule.m_RHS.size())
            {
                auto follow = m_FollowSets[rule.m_LHS];
                for (auto& f: follow)
                {
                    if (m_ActionTable[i.first].find(f) != m_ActionTable[i.first].end())
                        return false; //conflict

                    m_ActionTable[i.first][f] = ParseAction(rule.m_RHS.size(), rule.m_LHS);
                    //printf("adding reduce %i-->%s\n", i.first, f.c_str());
                }
                
                m_ActionTable[i.first]["EOF"] = ParseAction(rule.m_RHS.size(), rule.m_LHS);
            }
            else
            {
                auto sym = rule.m_RHS[entry.m_DotLoc].obj;
                bool reduce = false;
                if (!isTerminal(sym))
                {
                    m_GoToTable[i.first][sym] =  C[GoTo(i.second, sym, &reduce)];
                   // printf("goto %d|%s --> %d\n", i.first, sym.c_str(),  m_GoToTable[i.first][sym]);
                    continue;
                }
                auto shiftState = C[GoTo(i.second, sym, &reduce)];
                //printf("shift [%d][%s] --> %d\n", i.first, sym.c_str(), shiftState);
                if (m_ActionTable[i.first].find(sym) != m_ActionTable[i.first].end())
                        return false; //conflict
                m_ActionTable[i.first][sym] = ParseAction(shiftState);
            }
        }
    }
}


std::unordered_set<LR0_Item, PairHash> ParseTable::GoTo(const std::unordered_set<LR0_Item, PairHash> &set, const std::string &symbol, bool* reduce)
{
    std::unordered_set<LR0_Item, PairHash> ret;
   // printf("start %d\n", set.size());
    for (auto& item: set)
    {
       // PrintItem(item);
        // printf("here1 %d\n", item.m_LHS.size());
        auto entry = m_Grammar[item.m_LHS];
        //printf("here %d\n", entry.size());
        auto rule = entry[item.m_ProdRuleIndex];
         
        if (item.m_DotLoc >= rule.m_RHS.size())
            continue;
        if (rule.m_RHS[item.m_DotLoc].obj != symbol)
            continue;
        if (item.m_DotLoc+1 == rule.m_RHS.size())
            *reduce = true;
        
        LR0_Item new_item = {item.m_LHS, item.m_DotLoc+1, item.m_ProdRuleIndex};
        auto closure_new_item = Closure(new_item);
        for (auto& x: closure_new_item)
            ret.insert(x);
       // printf("done\n");
    }
    //printf("ret\n");
    return ret;
}

std::unordered_set<std::string> ParseTable::ConstructFirstSets(std::string nonterminal, std::unordered_set<std::string>& seen)
{
    if (seen.count(nonterminal))
        return {};
   // std::cout << nonterminal << "\n";   
     if (m_NonTerminals.count(nonterminal) == 0)
        return {nonterminal}; // this should get epsilon and terminals
    //std::cout << nonterminal << "\n";   
    std::unordered_set<std::string> ret;
    bool nullable = true;
    bool rhs_contains_epsilon = false;
    for (auto& prod: m_Grammar[nonterminal])
    {

        

        for (auto& rhs : prod.m_RHS)
        {
            if (rhs.type == RHSType::ACTION)
                continue;
            if (rhs.obj == ".")
                rhs_contains_epsilon = true;
           // printf("here\n");
            std::unordered_set<std::string> check;
            bool canBeEpsilon = CanBeEpsilon(rhs.obj, check);
            if (nonterminal == rhs.obj)
            {
                if (canBeEpsilon)
                    continue;
                else
                    break;
            }
            //seen.insert(rhs.obj);
            auto firstAn = ConstructFirstSets(rhs.obj, seen);
            if (canBeEpsilon)
                firstAn.insert(".");
            
            //for (auto& s : firstAn)
            //{
//
            //     m_ProdRuleFirstSets[nonterminal][s] = prod;
            //}

            if (firstAn.count(".") == 0)
            {
                ret.insert(firstAn.begin(), firstAn.end());
                nullable = false;
                break;
            }
            else
            {
                firstAn.erase(".");
                ret.insert(firstAn.begin(), firstAn.end());
               // seen.insert(rhs.obj);
            } 
        }
    }

    if (nullable || rhs_contains_epsilon)
        ret.insert(".");

    return ret;
}

std::unordered_set<std::string> ParseTable::ConstructFollowSets(std::string nonterminal)
{
    // printf("START followset %s\n", nonterminal.c_str());
    if (isTerminal(nonterminal))
    {
       // printf("followset is term%s\n", nonterminal.c_str());
        if (nonterminal == ".")
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

void ParseTable::ProduceItems()
{
    for (auto& rule: m_Grammar)
    {
        std::unordered_set<LR0_Item, PairHash> ruleSetI;
        for (int i = 0; i < rule.second.size(); i++)
        {
            auto currentRule = rule.second[i];
            //std::cout << currentRule.toString() << "\n";
            for (int j = 0; j <= currentRule.m_RHS.size(); j++)
            {
               
                LR0_Item item = {currentRule.m_LHS, j, i};
                ruleSetI.insert(item);
                //PrintItem(item);
                m_InitialItems.insert(item);
            }
        }
        
    }
}

void ParseTable::ProduceClosureSets()
{
    for (auto& item: m_InitialItems)
    {
        auto closureSet = Closure(item);
        
        //std::cout << "\n";
        //PrintItemSet(closureSet);
    
    
    }
}


bool ParseTable::Parse(std::vector<Token *> tokens)
{
    
    int token_num = 0;
    
    m_StateStack.push_back(0);
    // printf("here %ld %ld\n", m_StateStack.size(), m_SymbolStack.size());
    while(true)
    {
       
        auto s = m_StateStack.back(); 
        auto symbol = tokens[token_num];
        // printf("here %ld %ld current state: %d --> %s,%s\n", m_StateStack.size(), m_SymbolStack.size(), s, symbol->tokenType.c_str(), symbol->lexeme.c_str());

        if (m_ActionTable[s][symbol->tokenType].action == P_ACTION::ACCEPT)
        {
            return true;
        }

        if (m_ActionTable[s].find(symbol->tokenType) != m_ActionTable[s].end())
        {
            //printf("not end. type: %d %d\n", s, m_ActionTable[s][symbol->tokenType].action);
            if (m_ActionTable[s][symbol->tokenType].action == P_ACTION::SHIFT)
            {
                
                auto sState = m_ActionTable[s][symbol->tokenType].shiftState;
             //  printf("shift %d:%s\n", sState, symbol->tokenType.c_str());
                m_StateStack.push_back(sState);
                m_SymbolStack.push_back(symbol->tokenType);
                token_num++;
            }
            else if (m_ActionTable[s][symbol->tokenType].action == P_ACTION::REDUCE)
            {
                // printf("reduce %s\n", m_ActionTable[s][symbol->tokenType].reducePush.c_str());
                for (int i = 0; i < m_ActionTable[s][symbol->tokenType].reducePopCount; i++)
                {
                  //  printf("reduced: %s\n", m_SymbolStack.back().c_str());
                    m_SymbolStack.pop_back();
                     m_StateStack.pop_back();

                }
                
                m_SymbolStack.push_back(m_ActionTable[s][symbol->tokenType].reducePush);
               
                if (m_GoToTable[m_StateStack.back()].find(m_SymbolStack.back()) != m_GoToTable[m_StateStack.back()].end())
                {
                    m_StateStack.push_back(m_GoToTable[m_StateStack.back()][m_SymbolStack.back()]);
                  //  printf("pushing state: %d\n", m_GoToTable[m_StateStack.back()][m_SymbolStack.back()]);
                }
                //else
                  //  printf("no goto\n");
                
            }
            else
            {
               printf("ERROR token #%d, %s\n", token_num, symbol->tokenType.c_str());
                return false;
            }
        }
        else
        {
            printf("error\n");
            printf("not found m_ActionTable[%d][%s]: %s!\n", s, symbol->tokenType.c_str(), symbol->lexeme.c_str());
            for (auto& e: m_ActionTable[s])
            {
                std::cout << "{" << e.first << "," << e.second.action << "}\n";
            }
            return false;
        }
    }
}



bool ParseTable::isTerminal(std::string sym)
{
    return m_Terminals.count(sym) != 0;
}

void ParseTable::PrintItem(const LR0_Item &item)
{
    auto& entry = m_Grammar[item.m_LHS];
    auto rule = entry[item.m_ProdRuleIndex];
    

    std::cout << rule.m_LHS << " ::= ";
    for (int i = 0; i <= rule.m_RHS.size(); i++)
    {
        if (i == item.m_DotLoc)
            std::cout << " . ";
        if (i >= rule.m_RHS.size())
            break;

        std::cout <<  " " << rule.m_RHS[i].obj << " ";
        
    }
    std::cout << "\n";
}

std::unordered_set<LR0_Item, PairHash>  ParseTable::Closure(const LR0_Item& item)
{
   // auto rule = m_ProdRules[item.second];
    std::unordered_set<LR0_Item, PairHash> retSet;
    retSet.insert(item);
    while(true)
    {
        
        int count = 0;
        for (auto& it: retSet)
        {
            // first = dot location
            // second = prod rule index
            auto& rule = m_Grammar[it.m_LHS][it.m_ProdRuleIndex];
            if (it.m_DotLoc >= rule.m_RHS.size())
                continue;
            auto rhsObj = rule.m_RHS[it.m_DotLoc];
            auto rhsEntry = m_Grammar[rhsObj.obj];
            
            int i = 0;
            for (auto& currentRule: rhsEntry)
            {
                 LR0_Item checkItem = {rhsObj.obj, 0, i};
                if (retSet.count(checkItem) != 0)
                    continue;
                count++;
                i++;
                retSet.insert(checkItem);
            }
            
        }
        if (count == 0)
            break;
    }

    
    return retSet;
}

void ParseTable::PrintItemSet(const std::unordered_set<LR0_Item, PairHash>& s)
{
    if (!s.size())
        return;
    for (auto& p: s)
    {
        std::cout << p.m_LHS << " ::= ";

        auto rule = m_Grammar[p.m_LHS][p.m_ProdRuleIndex];
        for (int i = 0; i <= rule.m_RHS.size(); i++)
        {
            if (i == p.m_DotLoc)
                std::cout << " . ";
            if (i >= rule.m_RHS.size())
                break;

            std::cout << rule.m_RHS[i].obj << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

void ParseTable::AppendFile(const std::string& msg)
{
    std::ofstream file("./parser.dot", std::ios::app); // Open file in append mode
    if (!file)
    {
        std::cout << "OutfileWriter::AppendFile() error opening file for appending: ./parser.dot" << std::endl;
        return;
    }
    file << msg << std::endl;
}


void ParseTable::DumpAutomaton()
{
    AppendFile("digraph G {\n");

    for (auto& s : C)
    {
        std::string state = "I" + std::to_string(s.second);
        std::string label = " [label=\"";
        label += state + "\\n\\n";
        for (auto& item: s.first)
        {
            auto& entry = m_Grammar[item.m_LHS];
            auto rule = entry[item.m_ProdRuleIndex];
            
            label += rule.rawString(item.m_DotLoc) + "\\n";
        }



        
        


        label +="\"];\n";
        AppendFile("\t" + state + " " + label);
    }
    for (auto& x: m_GoToTable)
    {
        
        for (auto& to: m_GoToTable[x.first])
        {
            AppendFile("\tI" + std::to_string(x.first) + " -> I" + std::to_string(to.second));
            AppendFile(" [label=\"" + to.first + "\"];\n");
        }
    }

    for (auto& x: m_ActionTable)
    {
        for (auto& action: m_ActionTable[x.first])
        {
            if (action.second.action == P_ACTION::SHIFT)
            {
                AppendFile("\tI" + std::to_string(x.first) + " -> I" + std::to_string(action.second.shiftState));
                AppendFile(" [label=\"" + action.first + "\"];\n");
            }
        }
    }




    AppendFile("}");


}
