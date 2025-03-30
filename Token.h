#pragma once
#include <string>
#include "TokenStructure.h"

class Token
{
public:
    TokenType data_type;//token type
    std::string data; //current token data
    int pos;

    Token(const TokenType data_type,std::string data, int pos) : data_type(data_type), data(data), pos(pos) {};
};

