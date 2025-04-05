#pragma once
#include <string>
#include "TokenStructure.h"

class Token
{
public:
    TokenTypes type;
    std::string data; 
    int position;
    int line;

    Token(const TokenTypes type,std::string data, int pos,int line) : type(type), data(data), position(pos),line(line) {};
};

