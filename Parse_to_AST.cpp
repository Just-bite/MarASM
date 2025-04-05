#include "Parse_to_AST.h"

bool Parse_to_AST::isDirective()
{
	return peek().data_type == TokenType::DIRECTIVE;
}
bool Parse_to_AST::isLabel()
{
	return peek().data_type == TokenType::LABEL;
}
bool Parse_to_AST::isInstruction()
{
	return peek().data_type == TokenType::INSTRUCTION;
}
bool Parse_to_AST::isNewLine()
{
	return peek().data_type == TokenType::NEWLINE;
}
bool Parse_to_AST::match(TokenType type)
{
	return peek().data_type == type;
}

ASTRoot Parse_to_AST::getASTForest()
{
	return astForest;
}

Token Parse_to_AST::peek()
{
	return tokens[pos];
}

Token Parse_to_AST::consume()
{
	if (pos < tokens.size())
		return tokens[pos++];
	else
		return Token(TokenType::END,"",pos);
}

void Parse_to_AST::parseCode()
{
	while (pos < tokens.size()) {
		AST* node = parseLine();
		if(node)
			astForest.addInstruction(node);
	}
}

AST* Parse_to_AST::parseLine()
{
	if (isDirective()) 
		return  parseDirective();
	
	if(isLabel())
		return parseLabel();

	if (isInstruction())
		return parseInstruction();

	if (isNewLine()) {
		pos++;
		return nullptr;
	}
	
}

AST* Parse_to_AST::parseDirective() {
	DirectiveExpr* node = new DirectiveExpr();
	Token token = consume();
	node->name = token.data;

	while (pos < tokens.size() && !match(TokenType::NEWLINE)) {
		Token numToken = consume();
		if (numToken.data_type == TokenType::NUMBER || numToken.data_type == TokenType::IMMEDIATE) {
			node->data.push_back(stoi(numToken.data));
		}
		else {
			delete node; 
			throw std::runtime_error("Expected number in directive");
		}
	}

	return node;  
}

AST* Parse_to_AST::parseLabel()
{
	Token token = consume();
	return new LabelExpr(token.data.substr(0, token.data.size() - 1));
	
}

AST* Parse_to_AST::parseInstruction() {
	Token mnemonicToken = consume();
	InstructionExpr* instr = new InstructionExpr();
	instr->opcode = mnemonicToken.data;

	if (instr->opcode != "HALT" && instr->opcode != "NOP") {
		if (match(TokenType::REGISTER) || match(TokenType::NUMBER)) {
			instr->src = parseOperand(consume());
		}

		if (match(TokenType::COMMA)) {
			consume();
			instr->dest = parseOperand(consume());
		}
	}

	instr->size = 2;
	currentAdress += instr->size;
	return instr;
}

AST* Parse_to_AST::parseOperand(const Token& token) {

	AST operand;
	if (token.data_type == TokenType::REGISTER) {
		return new RegisterExpr(std::stoi(token.data.substr(1)));		
	}
  
    else if (token.data_type == TokenType::NUMBER) {
		return new NumberExpr(std::stoi(token.data.substr(1)));
    }
    else if (token.data_type == TokenType::IMMEDIATE) {
        if (token.data[0] == '#') {
			return new ImmediateExpr(std::stoi(token.data.substr(1)));
        }
        else {
			return new RegisterIndirectExpr(std::stoi(token.data.substr(1)));
        }
    }
    else {
        throw std::runtime_error("Invalid operand: " + token.data);
    }

}