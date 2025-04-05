#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include "Token.h"
#include <stdexcept>
#include <cctype>
#include <vector>
#include <string>
#include <map>
#include <hash_map>

enum class OperandType {
    REGISTER,     // R0-R7
    IMMEDIATE,    //#42
    ABSOLUTE,     // 1234
    RELATIVE,     // 
    INDEXED,      // 10(R2)
    INDIRECT,     // (R3)
    AUTOINC,      // (R4)+
    AUTODEC,      // -(R5)
    INDIRECT_AUTOINC  // @(R6)+
};

struct Operand {
    OperandType type;
    int value;          
    int index_reg;      
    std::string label;  
};

struct Instruction {
    std::string mnemonic;
    Operand src;         
    Operand dst;         
    int size;            
};


struct Directive {
    std::string name;      
    std::vector<int> data; 
    int size;              
};

struct Label {
    std::string name;
    int address;           
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    void parse();

    const std::vector<Instruction>& getInstructions() const;
    const std::vector<Directive>& getDirectives() const;
    const std::map<std::string, Label>& getLabels() const;

private:
    std::vector<Token> tokens;
    size_t current_pos = 0;


    std::vector<Instruction> instructions;
    std::vector<Directive> directives;
    std::map<std::string, Label> labels;
    int current_address = 0;

    Token peek() const;
    Token consume();
    bool match(TokenType type) const;


    void parseLine();
    void parseLabel();
    void parseInstruction();
    void parseDirective();

    Instruction parseSingleInstruction(const std::string& mnemonic);
    Operand parseOperand(const Token& token);
    Directive parseWordDirective();
    Directive parseByteDirective();


    bool isLabel() const;
    bool isDirective() const;
    bool isInstruction() const;
    bool isNewLine() const;


    int calculateInstructionSize(const Instruction& instr) const;
    int parseNumber(const std::string& s) const;
};


