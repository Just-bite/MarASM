#pragma once
#include "Token.h"
#include <stdexcept>
#include <cctype>
#include <vector>
#include <string>
#include <map>

// Режимы адресации PDP-11
enum class OperandType {
    REGISTER,     // R0-R7
    IMMEDIATE,    //#42
    ABSOLUTE,     // 1234
    RELATIVE,     // метка (позже станет смещением)
    INDEXED,      // 10(R2)
    INDIRECT,     // (R3)
    AUTOINC,      // (R4)+
    AUTODEC,      // -(R5)
    INDIRECT_AUTOINC  // @(R6)+
};

// Операнд инструкции
struct Operand {
    OperandType type;
    int value;          // число или регистр (R0 → 0)
    int index_reg;      // для INDEXED (R2 в 10(R2))
    std::string label;  // для RELATIVE (если есть метка)
};

// Инструкция ассемблера
struct Instruction {
    std::string mnemonic;  // "MOV", "ADD"...
    Operand src;           // источник
    Operand dst;           // назначение
    int size;              // размер в байтах
};

// Директива ассемблера (например, .WORD 42)
struct Directive {
    std::string name;      // ".WORD", ".BYTE"...
    std::vector<int> data; // значения
    int size;              // размер в байтах
};

// Метка и её адрес
struct Label {
    std::string name;
    int address;           // заполнится при первом проходе
};

class Parser {
public:
    // Конструктор
    Parser(const std::vector<Token>& tokens);

    // Основной метод: запуск парсинга
    void parse();

    // Геттеры результатов
    const std::vector<Instruction>& getInstructions() const;
    const std::vector<Directive>& getDirectives() const;
    const std::map<std::string, Label>& getLabels() const;

private:
    // Входные данные
    std::vector<Token> tokens;
    size_t current_pos = 0;

    // Выходные данные
    std::vector<Instruction> instructions;
    std::vector<Directive> directives;
    std::map<std::string, Label> labels;
    int current_address = 0;  // Счётчик адресов

    // Вспомогательные методы
    Token peek() const;
    Token consume();
    bool match(TokenType type) const;

    // Методы разбора
    void parseLine();
    void parseLabel();
    void parseInstruction();
    void parseDirective();

    Instruction parseSingleInstruction(const std::string& mnemonic);
    Operand parseOperand(const Token& token);
    Directive parseWordDirective();
    Directive parseByteDirective();

    // Проверки
    bool isLabel() const;
    bool isDirective() const;
    bool isInstruction() const;
    bool isNewLine() const;

    // Утилиты
    int calculateInstructionSize(const Instruction& instr) const;
    int parseNumber(const std::string& s) const;
};
