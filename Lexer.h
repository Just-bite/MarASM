#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include "Token.h"

class Lexer
{
public:
	std::string code;
	int pos = 0;
	std::vector<Token> token_list;

	Lexer(const char* code) : code(code) {};

	std::vector<Token> lexAnalisis()
	{
		while (this->nextToken());
		return this->token_list;
	}

	bool nextToken()
	{
		try
		{
			if (this->pos >= this->code.length())
			{
				return false;
			}
			const auto token_list = Token_patterns;
			for (const auto& tokenType : Token_patterns)
			{
				std::smatch match;
				std::string remaining_code = this->code.substr(this->pos);
				if (std::regex_search(remaining_code, match, tokenType.regex)) {
					if (!match.str().empty())
					{
						if (tokenType.token_name != "SPACE")
						{
							Token token = Token(tokenType.type, match.str(), this->pos);
							this->token_list.push_back(token);
						}
						this->pos += match.str().length();
						return true;
					}
				}
			}
			throw std::invalid_argument("error at pos ");
		}
		catch (const std::invalid_argument& e) {
			std::cerr << "Lexical: " << e.what() <<this->pos << std::endl;
		}
	}
};

