#include "errors.hpp"

void a_lang::Report::fatal(const Position *pos, const char *msg)
{
	std::cerr << "FATAL " 
	<< pos->span()
	<< ": " 
	<< msg  << std::endl;
}