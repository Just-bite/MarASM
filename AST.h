#pragma once
#include <string>
#include <vector>

class AST { public: virtual ~AST() {} };

class ASTRoot : public AST {
public:
    std::vector<AST*> instructions; 
    void addInstruction(AST* instr) {
        instructions.push_back(instr);
    }
};

class RegisterExpr : public AST
{ 
public:
    int reg;
    bool is_sp = false; //R6
    bool is_pc = false; //R7
    RegisterExpr(int reg_num) : reg(reg_num) {} 
};

class RegisterIndirectExpr : public AST
{
public:
    int reg;
    RegisterIndirectExpr(int r) : reg(r) {}
};

class AutoIncrementExpr : public AST 
{
public:
    int reg;
    AutoIncrementExpr(int r) : reg(r) {}
};

class ImmediateExpr : public AST
{
public:
    int value;
    bool is_char = false;
    ImmediateExpr(int v) : value(v) {}
};

class NumberExpr : public AST
{
public:
    int number;
    NumberExpr(int number) : number(number) {}
};



class LabelExpr : public AST 
{
public: 
    std::string name; 
    LabelExpr(std::string n) : name(n) {}
};

class BranchExpr : public AST
{
public:
    std::string target; 
    BranchExpr(std::string t) : target(t) {}
};


class InstructionExpr : public AST 
{
public: 
    std::string opcode;
    int size;
    AST* src;  
    AST* dest;
    InstructionExpr(){};
    InstructionExpr(int size,std::string op, AST* s, AST* d) : size(size), opcode(op), src(s), dest(d) {}
};


class DirectiveExpr : public AST {
public:
    std::string name;  
    std::vector<int> data;
    int size;
    DirectiveExpr() {};
    DirectiveExpr(std::string name, std::vector<int> data) : name(name), data(data) {}
};