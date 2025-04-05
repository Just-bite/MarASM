#pragma once
#include <string>
#include <vector>

enum class AdressTypes {
    REGISTER,                   // Rn
    REGISTER_INDIRECT,          // (Rn)
    AUTOINCREMENT,              // (Rn)+
    AUTOINCREMENT_INDIRECT,     // @(Rn)+
    AUTODECREMENT,              // -(Rn)
    AUTODECREMENT_IDIRECT,      // @-(Rn)
    INDEXED,                    // X(Rn)
    INDEXED_INDIRECT,           // @X(Rn)
    IMMEDIATE,                  // #N
    ABSOLUTE,                   // @#A
    RELATIVE,                   // label
};



class AST { public: virtual ~AST() {} };

class ASTRoot : public AST {
public:
    std::vector<AST*> instructions; 
    void addInstruction(AST* instr) {
        instructions.push_back(instr);
    }
};

class RegisterNode : public AST
{ 
public:
    int register_number;
    AdressTypes adress_type;
    bool is_sp = false; // R6
    bool is_pc = false; // R7
    RegisterNode(int register_number) : register_number(register_number) {} 
};

class NumberNode : public AST
{
public:
    int number;
    AdressTypes adress_type;
    NumberNode(int number) : number(number) {}
};



class LabelNode : public AST 
{
public: 
    int adress;
    std::string name; 
    AdressTypes adress_type = AdressTypes::RELATIVE;
    LabelNode(std::string name) : name(name) {}
};

class BranchNode : public AST
{
public:
    std::string target; 
    AdressTypes adress_type;
    BranchNode(std::string t) : target(t) {}
};


class InstructionNode : public AST 
{
public: 
    int size;
    std::string mnemonic;
    uint16_t opcode;
    uint16_t op_mask;
    AST* src;  
    AST* dest;
    InstructionNode(){};
    InstructionNode(int size,std::string op, AST* s, AST* d) : size(size), opcode(op), src(s), dest(d) {}
};


class DirectiveNode : public AST {
public:
    std::string name;  
    std::vector<int> data;
    int size = 0;
    DirectiveNode() {};
    DirectiveNode(std::string name, std::vector<int> data) : name(name), data(data) {}
};