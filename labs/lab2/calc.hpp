#ifndef EECS_MANAGER_HPP
#define EECS_MANAGER_HPP

#include "scanner.hpp"
#include "grammar.hh"

namespace EECS{

class Manager {
public:
	void parse(const char * filepath);
	static int m_NodeCount;
private:
	
	EECS::Scanner * scanner = nullptr;
	EECS::Parser * parser = nullptr;
}; /* end class manager */

} /* end namespace EECS */

#endif
