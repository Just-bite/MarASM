#include "Parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

void Parser::parse() {
    while (current_pos < tokens.size()) {
        parseLine();
    }
}

void Parser::parseLine() {
    if (isLabel()) {
        parseLabel();
    }
    else if (isDirective()) {
        parseDirective();
    }
    else if (isInstruction()) {
        parseInstruction();
    }
    else if (isNewLine()) {
        current_pos++;
    }
    else {
        throw std::runtime_error("Unknown line format");
    }
}

void Parser::parseLabel() {
    Token labelToken = consume();
    std::string labelName = labelToken.data.substr(0, labelToken.data.size() - 1); // Убираем ':'

    labels[labelName] = Label{ labelName, current_address };
}

void Parser::parseInstruction() {
    Token mnemonicToken = consume();
    Instruction instr = parseSingleInstruction(mnemonicToken.data);

    if (instr.mnemonic != "HALT" && instr.mnemonic != "NOP") {
        if (match(TokenType::REGISTER) || match(TokenType::NUMBER)) {
            instr.src = parseOperand(consume());
        }

        if (match(TokenType::COMMA)) {
            consume();
            instr.dst = parseOperand(consume());
        }
    }

    instr.size = calculateInstructionSize(instr);
    instructions.push_back(instr);
    current_address += instr.size;
}

Instruction Parser::parseSingleInstruction(const std::string& mnemonic) {
    Instruction instr;
    instr.mnemonic = mnemonic;
    return instr;
}

Operand Parser::parseOperand(const Token& token) {
    Operand op;

    if (token.data_type == TokenType::REGISTER) {
        op.type = OperandType::REGISTER;
        op.value = std::stoi(token.data.substr(1)); // R0 → 0
    }
    else if (token.data_type == TokenType::NUMBER) {
        op.type = OperandType::IMMEDIATE;
        op.value = parseNumber(token.data);
    }
    else if (token.data_type == TokenType::IMMEDIATE) {
        if (token.data[0] == '#') {
            op.type = OperandType::IMMEDIATE;
            op.value = parseNumber(token.data.substr(1));
        }
        else {
            op.type = OperandType::RELATIVE;
            op.label = token.data;
        }
    }
    else {
        throw std::runtime_error("Invalid operand: " + token.data);
    }

    return op;
}

void Parser::parseDirective() {
    Token dirToken = consume();
    Directive dir;

    if (dirToken.data == ".WORD") {
        dir = parseWordDirective();
    }
    else if (dirToken.data == ".BYTE") {
        dir = parseByteDirective();
    }
    else {
        throw std::runtime_error("Unknown directive: " + dirToken.data);
    }

    directives.push_back(dir);
    current_address += dir.size;
}

Directive Parser::parseWordDirective() {
    Directive dir;
    dir.name = ".WORD";

    while (current_pos < tokens.size() && !match(TokenType::NEWLINE)) {
        Token numToken = consume();
        if (numToken.data_type == TokenType::NUMBER || numToken.data_type == TokenType::IMMEDIATE) {
            dir.data.push_back(parseNumber(numToken.data));
        }
        else {
            throw std::runtime_error("Expected number in .WORD directive");
        }
    }

    dir.size = 2 * dir.data.size(); 
    return dir;
}

Directive Parser::parseByteDirective() {
    Directive dir;
    dir.name = ".BYTE";

    while (current_pos < tokens.size() && !match(TokenType::NEWLINE)) {
        Token numToken = consume();
        if (numToken.data_type == TokenType::NUMBER || numToken.data_type == TokenType::IMMEDIATE) {
            dir.data.push_back(parseNumber(numToken.data) & 0xFF);
        }
        else {
            throw std::runtime_error("Expected number in .BYTE directive");
        }
    }

    dir.size = dir.data.size(); // Each .BYTE is 1 byte :)
    return dir;
}

bool Parser::isLabel() const {
    return peek().data_type == TokenType::LABEL;
}

bool Parser::isDirective() const {
    return peek().data_type == TokenType::DIRECTIVE;
}

bool Parser::isInstruction() const {
    return peek().data_type == TokenType::INSTRUCTION;
}
bool Parser::isNewLine() const {
    return peek().data_type == TokenType::NEWLINE;
}
Token Parser::peek() const {
    if (current_pos >= tokens.size()) {
        return Token(TokenType::END, "", current_pos);
    }
    return tokens[current_pos];
}

Token Parser::consume() {
    if (current_pos >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return tokens[current_pos++];
}

bool Parser::match(TokenType type) const {
    return peek().data_type == type;
}

int Parser::calculateInstructionSize(const Instruction& instr) const {
    // SIMPLIFIED NEEDS FIX
    if (instr.mnemonic == "HALT" || instr.mnemonic == "NOP") {
        return 2;
    }
    return 2; // COMMON
}

int Parser::parseNumber(const std::string& s) const {
    if (s.empty()) return 0;

    if (s.size() > 1 && (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) || s[0] == '#') {
        return std::stoi(s.substr(s[0] == '#' ? 1 : 2), nullptr, 16);
    }

    return std::stoi(s);
}

const std::vector<Instruction>& Parser::getInstructions() const { return instructions; }
const std::vector<Directive>& Parser::getDirectives() const { return directives; }
const std::map<std::string, Label>& Parser::getLabels() const { return labels; }

