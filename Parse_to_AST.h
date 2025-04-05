#pragma once

#include <vector>
#include <string>
#include "AST.h"
#include "Token.h"

class Parse_to_AST
{
public:
	int pos = 0;
	int currentAdress = 0;
	Parse_to_AST(std::vector<Token> tokens) : tokens(tokens) {};
	ASTRoot getASTForest();
	Token peek();
	Token consume();
	void parseCode();
	AST* parseLine();

	bool isDirective();
	bool isLabel();
	bool isInstruction();
	bool isNewLine();
	bool match(TokenType);

	AST* parseDirective();
	AST* parseLabel();
	AST* parseInstruction();
	AST* parseOperand(const Token& token);

private:
	std::vector<Token> tokens;
	ASTRoot astForest;

};

