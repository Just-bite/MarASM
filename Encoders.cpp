#include "Encoders.h"


uint8_t Encoder::getInstructionLength(const Token& token)
{
    if (0)
    {
        return 1;
    }
    return 2;
}
void Encoder::parse()
{
    firstPass();
    secondPass();
}

void Encoder::firstPass()
{
    for (const auto& token : tokens)
    {
        if (token.data_type == TokenType::LABEL)
        {
            symbolMap.emplace(token.data, PL);
        }
        PL += getInstructionLength(token);
    }
}

void Encoder::secondPass()
{
    PL = 0;
    std::vector<std::string> objCode;
    for (const auto& token : tokens)
    {
        symbolMap.emplace(token.data, getPL(token));
        PL += getInstructionLength(token);
    }
}

uint8_t Encoder::getPL(const Token&)
{

    return 1;
}