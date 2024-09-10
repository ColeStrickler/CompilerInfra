#include <fstream>
#include <stdint.h>
#include "mylexer.hpp"

int main(int argc, char *argv[]){
	std::ifstream * infile = new std::ifstream(argv[1]);
	MyLexer * lex  = new MyLexer(infile);
	bool loop = true;

	uint64_t nouns = 0;
	uint64_t verbs = 0;
	uint64_t adjectives = 0;

	while (loop){
		Token * tok;
		auto type = lex->produceToken(&tok);
		switch(type)
		{
			case TokenType::ERROR:
			{
				return -1;
			}
			case TokenType::NOUN:
			{
				nouns++;
				break;
			}
			case TokenType::VERB:
			{
				verbs++;
				break;
			}
			case TokenType::ADJECTIVE:
			{
				adjectives++;
				break;
			}
			case TokenType::END_OF_FILE:
			{
				loop = false;
				break;
			}
			default:
			{
				std::cout << "Encountered unexpected token type. Exiting\n";
				return -2;
			}
		}
	}

	printf("nouns: %lu, verbs: %lu, adjectives: %lu\n", nouns, verbs, adjectives);
}
