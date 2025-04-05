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
	std::string source_code;

	Lexer(std::string source_code) : source_code(source_code) {};

	std::vector<Token> runLexAnalisis();
	std::vector<Token> getTokenList();

	bool isInAnalisys();
	bool analiseTokenBasedOnPatterns();

	void trackPosition();
	void putMatchingTokenInTokenList(TokenStructure, std::smatch);
private:
	int pos = 0;
	int line = 0;
	std::vector<Token> token_list;
};

