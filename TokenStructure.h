#pragma once

#include <string>
#include <vector>
#include <regex>    
#include <utility>

enum class TokenTypes {
    INSTRUCTION,  // MOV, ADD, SUB
    BRANCH,       // BR,JMP
    REGISTER,     // R0-R7
    NUMERIC,       // 42, 0xAB, 0777  are allowed only in directives, so they must be inspected if adressing
    LABEL,        // LOOP:
    DIRECTIVE,    // .WORD, .END
    COMMA,        // ,
    LPAREN,       // (
    RPAREN,       // )
    PLUS,         // +
    MINUS,        // -
    AT,           // @
    NUMSIGN,      // #
    COMMENT,      // ; 
    NEWLINE,       // \n
    END,           // \0
    SPACE,        //  \t, \r
};

class TokenStructure
{
public:
    TokenTypes type;
    std::regex regex;
    std::string token_name;

    TokenStructure(TokenTypes type, const char* regex, const char* token_name) :
        type(type), regex(regex, std::regex::icase | std::regex::optimize), token_name(token_name) {}
};

const std::vector<TokenStructure> token_patterns = {

    TokenStructure(TokenTypes::INSTRUCTION, R"(^(MOV|ADD|SUB|MUL|DIV|ASH|ASHC|XOR|JSR|RTS|SOB|CLR|COM|INC|DEC|NEG|TST|ASR|ASL|ROR|ROL|SWAB|ADC|SBC|SXT|MARK|SPL|EMT|TRAP|BIC|BIS|BIT|HALT|WAIT|RTI|IOT|RESET|MFPS|MTPS)\b)", "INSTRUCTION"),
    TokenStructure(TokenTypes::BRANCH, R"(^(BR|BNE|BEQ|BPL|BMI|BVC|BVS|BCC|BCS|BGE|BLT|BGT|BLE|BHI|BLOS|BHIS|BLO|JMP)\b)", "BRANCH"),
    TokenStructure(TokenTypes::AT, R"(^@)", "AT"),
    TokenStructure(TokenTypes::NUMSIGN, R"(^\#)", "NUMSIGN"),
    TokenStructure(TokenTypes::MINUS, R"(^\-)", "MINUS"),
    TokenStructure(TokenTypes::LPAREN, R"(^\()", "LPAREN"),
    TokenStructure(TokenTypes::REGISTER, R"(^R[0-7]\b)", "REGISTER"),
    TokenStructure(TokenTypes::RPAREN, R"(^\))", "RPAREN"),
    TokenStructure(TokenTypes::PLUS, R"(^\+)", "PLUS"),
    TokenStructure(TokenTypes::NUMERIC,R"(^(0x[0-9A-Fa-f]+|0[0-7]*|[1-9]\d*))", "NUMERIC"),
    TokenStructure(TokenTypes::NEWLINE, R"(^\n\s*)", "NEWLINE"),
    TokenStructure(TokenTypes::COMMA, R"(^,)", "COMMA"),
    TokenStructure(TokenTypes::LABEL, R"(^[A-Za-z_]\w*:?)", "LABEL"),
    TokenStructure(TokenTypes::SPACE, R"(^[ \t\r]+)", "SPACE"),
    TokenStructure(TokenTypes::DIRECTIVE, R"(^\.[A-Za-z][A-Za-z0-9]*)", "DIRECTIVE"),
    TokenStructure(TokenTypes::COMMENT, R"(^;[^\n]*)", "COMMENT"),
    TokenStructure(TokenTypes::END, R"(^\0)", "END"),

};