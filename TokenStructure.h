#pragma once

#include <string>
#include <vector>
#include <regex>    
#include <utility>

enum class TokenType {
    INSTRUCTION,  // MOV, ADD, SUB
    REGISTER,     // R0-R7
    IMMEDIATE,    // #42, #0xAB, #0777
    NUMBER,       // 42, 0xAB, 0777 (без #)
    LABEL,        // LOOP:
    DIRECTIVE,    // .WORD, .END
    COMMA,        // ,
    LPAREN,       // (
    RPAREN,       // )
    PLUS,         // +
    MINUS,        // -
    ASSIGN,       // =
    AT,           //@
    COMMENT,      // ; 
    NEWLINE,       // \n
    END,           // \0
    SPACE,        //  \t, \r
    UNKNOWN       // 
};

class TokenStructure
{
public:
    TokenType type;
    std::regex regex;
    std::string token_name;

    TokenStructure(TokenType type, const char* regex, const char* token_name) :
        type(type), regex(regex, std::regex::icase | std::regex::optimize), token_name(token_name) {}
};

const std::vector<TokenStructure> Token_patterns = {

    TokenStructure(TokenType::INSTRUCTION, R"(^(MOV|ADD|SUB|MUL|DIV|ASH|ASHC|XOR|BR|BNE|BEQ|BPL|BMI|BVC|BVS|BCC|BCS|BGE|BLT|BGT|BLE|BHI|BLOS|BHIS|BLO|JMP|JSR|RTS|SOB|CLR|COM|INC|DEC|NEG|TST|ASR|ASL|ROR|ROL|SWAB|ADC|SBC|SXT|MARK|SPL|EMT|TRAP|BIC|BIS|BIT|HALT|WAIT|RTI|IOT|RESET|MFPS|MTPS)\b)", "INSTRUCTION"),
    TokenStructure(TokenType::REGISTER, R"(^R[0-7]\b)", "REGISTER"),
    TokenStructure(TokenType::IMMEDIATE, R"(^#(0x[0-9A-Fa-f]+|0[0-7]*|[1-9]\d*))", "IMMEDIATE"),
    TokenStructure(TokenType::NUMBER, R"(^(0x[0-9A-Fa-f]+|0[0-7]*|[1-9]\d*))", "NUMBER"),
    TokenStructure(TokenType::NEWLINE, R"(^\n\s*)", "NEWLINE"),
    TokenStructure(TokenType::COMMA, R"(^,)", "COMMA"),
    TokenStructure(TokenType::SPACE, R"(^[ \t\r]+)", "SPACE"),
    TokenStructure(TokenType::LABEL, R"(^[A-Za-z_]\w*:)", "LABEL"),
    TokenStructure(TokenType::DIRECTIVE, R"(^\.[A-Za-z][A-Za-z0-9]*)", "DIRECTIVE"),
    
    TokenStructure(TokenType::LPAREN, R"(^\()", "LPAREN"),
    TokenStructure(TokenType::RPAREN, R"(^\))", "RPAREN"),
    TokenStructure(TokenType::PLUS, R"(^\+)", "PLUS"),
    TokenStructure(TokenType::MINUS, R"(^\-)", "MINUS"),
    TokenStructure(TokenType::ASSIGN, R"(^=)", "ASSIGN"),
    TokenStructure(TokenType::AT, R"(^@)", "AT"),
    TokenStructure(TokenType::COMMENT, R"(^;[^\n]*)", "COMMENT"),
    TokenStructure(TokenType::END, R"(^\0)", "END"),

};