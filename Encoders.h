#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include "Token.h"
#include <cctype>
#include <vector>
#include <string>
#include <hash_map>

class Encoder{
public:
    Encoder(std::vector<Token> tokens) : tokens(tokens) {};
    void parse();
private:
    uint8_t PL = 0;
    std::vector<Token> tokens;
    std::hash_map<std::string, uint8_t > symbolMap;
    uint8_t getInstructionLength(const Token&);
    void firstPass();
    void secondPass();
    uint8_t getPL(const Token&);
};

