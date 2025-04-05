#include "Parse_to_AST.h"
#include "TokenStructure.h"

bool Parse_to_AST::isDirective()
{
	return peek().type == TokenTypes::DIRECTIVE;
}
bool Parse_to_AST::isLabel()
{
	return peek().type == TokenTypes::LABEL;
}
bool Parse_to_AST::isInstruction()
{
	return peek().type == TokenTypes::INSTRUCTION;
}
bool Parse_to_AST::isNewLine()
{
	return peek().type == TokenTypes::NEWLINE;
}
bool Parse_to_AST::match(TokenTypes type)
{
	return peek().type == type;
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
		return Token(TokenTypes::END,"",pos,line);
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
		line++;
		return nullptr;
	}
	
}

AST* Parse_to_AST::parseDirective() {
	DirectiveNode* node = new DirectiveNode();
	Token token = consume();
	node->name = token.data;

	while (pos < tokens.size() && !match(TokenTypes::NEWLINE)) {
		Token numToken = consume();
		if (numToken.type == TokenTypes::NUMSIGN) 
			token = consume();

		if (numToken.type == TokenTypes::NUMERIC) {
			node->data.push_back(stoi(numToken.data));
			node->size += 2;
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
	return new LabelNode(token.data.substr(0, token.data.size() - 1));
	
}

AST* Parse_to_AST::parseInstruction() {
	Token mnemonicToken = consume();
	InstructionNode* instr = new InstructionNode();
	instr->mnemonic = mnemonicToken.data;

	if (instr->mnemonic != "HALT" && instr->mnemonic != "NOP") {
		if (match(TokenTypes::AT))
			processATAdressing();
		if (match(TokenTypes::MINUS))
			processAutoDecrement();

		if (match(TokenTypes::NUMSIGN)) //дописать для отсальных типов памяти
			processImmediate();

		if (match(TokenTypes::NUMERIC))
			processIndex();
		if (match(TokenTypes::REGISTER) || match(TokenTypes::NUMERIC)) {
			instr->src = parseOperand(consume());
		}

		if (match(TokenTypes::COMMA)) {
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
	if (token.type == TokenTypes::REGISTER) {
		return new RegisterNode(std::stoi(token.data.substr(1)));		
	}
  
    else if (token.type == TokenTypes::NUMERIC) {
		return new NumberNode(std::stoi(token.data.substr(1)));
    }
    /*else if (token.type == TokenType::IMMEDIATE) {
        if (token.data[0] == '#') {
			return new ImmediateExpr(std::stoi(token.data.substr(1)));
        }
        else {
			return new RegisterIndirectExpr(std::stoi(token.data.substr(1)));
        }
    }
    else {
        throw std::runtime_error("Invalid operand: " + token.data);
    }*/

}