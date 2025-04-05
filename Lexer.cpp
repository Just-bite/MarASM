#include "Lexer.h"

std::vector<Token> Lexer::runLexAnalisis()
{
	while (this->isInAnalisys());
	return this->token_list;
}

std::vector<Token> Lexer::getTokenList()
{
	return this->token_list;
}

void Lexer::trackPosition()
{
	this->line++;
}

void Lexer::putMatchingTokenInTokenList(TokenStructure tokenType,std::smatch match)
{
	if (tokenType.type != TokenTypes::SPACE)
	{
		if (tokenType.type == TokenTypes::NEWLINE)
			trackPosition();

		Token token = Token(tokenType.type, match.str(), this->pos, this->line);
		this->token_list.push_back(token);
	}
	this->pos += match.str().length();
}

bool Lexer::isInAnalisys() 
{
		if (this->pos >= this->source_code.length())
			return false;
		
		return analiseTokenBasedOnPatterns();
}

bool Lexer::analiseTokenBasedOnPatterns()
{	
	try
	{
		int error_flag = 0;
		const auto token_list = token_patterns;
		for (const auto& tokenType : token_patterns)
		{
			std::smatch match;
			std::string remaining_code = this->source_code.substr(this->pos);
			if (std::regex_search(remaining_code, match, tokenType.regex)) 
			{
				if (!match.str().empty())
				{
					putMatchingTokenInTokenList(tokenType,match);
					return true;
				}
			}
		}
		throw std::invalid_argument("Lexical: error at line ");
	}
	catch (const std::invalid_argument& e) {
		std::cerr << e.what() << this->line <<"pos"<<this->pos << std::endl;
	}
}