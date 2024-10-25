#include "symbol_table.hpp"
namespace a_lang {

	ScopeTable::ScopeTable()
	{

	}

    void ScopeTable::AddSymbol(SemSymbol *symbol)
    {
		symbols[symbol->m_SymbolName] = symbol;
    }

    bool ScopeTable::SymbolExists(const std::string &symbolName)
    {
		if (symbols.find(symbolName) != symbols.end())
			return true;
		return false;
	}


    SemSymbol *ScopeTable::GetSymbol(const std::string &symbolName)
    {
        if (symbols.find(symbolName) != symbols.end())
			return symbols[symbolName];

		printf("ERROR. SYMBOL NOT FOUND\n");
		return nullptr;
    }


    SymbolTable::SymbolTable(){
		//TODO: implement the list of hashtables approach
		// to building a symbol table:
		// Upon entry to a scope a new scope table will be 
		// entered into the front of the chain and upon exit the 
		// latest scope table will be removed from the front of 
		// the chain.
		scopeTableChain = std::vector<ScopeTable *>();
	}

    bool SymbolTable::AddSymbolCurrentScope(SemSymbol* sym)
    {
		auto& currentScope = scopeTableChain.back();
		
		/*
			There is already a symbol with this name
		*/
		if(currentScope->SymbolExists(sym->m_SymbolName))
		{
			

			// throw an errror?
		}

		
		currentScope->AddSymbol(sym);
		return true;
    }

    void SymbolTable::NewScope()
    {
		
		auto newScope = new ScopeTable();
		scopeTableChain.push_back(newScope);
	}

	void SymbolTable::EndScope()
	{
		auto scope = scopeTableChain.back();
		scopeTableChain.pop_back();
		delete scope;
	}

    SemSymbol *SymbolTable::GetSymbol(const std::string &symbolName)
    {
        for (auto it = scopeTableChain.rbegin(); it != scopeTableChain.rend(); it++)
		{
			if ((*it)->SymbolExists(symbolName))
			{
				return (*it)->GetSymbol(symbolName);
			}
		}
	
		return nullptr;
    }

    bool SymbolTable::SymbolExists(const std::string& symbolName)
    {
        for (auto it = scopeTableChain.rbegin(); it != scopeTableChain.rend(); it++)
		{

			if ((*it)->SymbolExists(symbolName))
				return true;
		}
		return false;
    }

	bool SymbolTable::SymbolExistsInCurrentScope(const std::string& symbolName)
    {
        if(scopeTableChain[scopeTableChain.size() - 1]->SymbolExists(symbolName))
			return true;
		return false;
    }


    SemSymbol::~SemSymbol()
    {
    }
    void SemSymbol::AddFunctionParameters(const std::list<FormalDeclNode *>*  formals)
    {
		for (auto it = formals->begin(); it != formals->end(); it++)
			m_FunctionParamFormals.push_back((*it)->getTypeNode()->GetTypeString());
    }
    void SemSymbol::AddFunctionReturnType(TypeNode *retType)
    {
		m_xType = retType->GetTypeString();
    }

    void SemSymbol::AddVariableDeclType(TypeNode *varType)
    {
		m_xType = varType->GetTypeString();
    }

    std::string SemSymbol::toString()
    {
        switch (m_Type)
		{
			case SYMBOLTYPE::VARIABLE:
			{
				return "{" + m_xType + "}";
			}
			case SYMBOLTYPE::FUNCTION:
			{
				std::string ret = "{(";
				if (m_FunctionParamFormals.size())
				{
					for (unsigned int i = 0; i < m_FunctionParamFormals.size() - 1 ; i++)
					{
						ret += m_FunctionParamFormals[i] + ", ";
					}
					
					ret += m_FunctionParamFormals[m_FunctionParamFormals.size() - 1];
				}
				
				ret += ")->" + m_xType + "}";
				return ret;
			}
			case SYMBOLTYPE::CLASS:
			{
				std::string ret;
				ret += "{" + m_SymbolName  + "}";
				return ret;
			}
			default:
				break;
		}
		return "err\n";
    }
}
