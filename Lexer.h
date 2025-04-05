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

	Lexer(std::string code) : code(code) {};

	std::vector<Token> lexAnalisis();

	bool nextToken();
};

