#pragma once //absolutely dislike this file will rework it soon
#include <string>
#include <vector>
#include <cstdint>
#include <type_traits>

enum class OperandType : uint8_t {
    NONE = 0,    
    REG = 1 << 0, //  (R0-R7)
    MEM = 1 << 1, //  ((R1))
    IMM = 1 << 2, //  (#42)
    REL = 1 << 3  //  (label)
};

inline OperandType operator|(OperandType a, OperandType b) {
    return static_cast<OperandType>(
        static_cast<typename std::underlying_type<OperandType>::type>(a) |
        static_cast<typename std::underlying_type<OperandType>::type>(b)
        );
}

inline OperandType operator&(OperandType a, OperandType b) {
    return static_cast<OperandType>(
        static_cast<typename std::underlying_type<OperandType>::type>(a) &
        static_cast<typename std::underlying_type<OperandType>::type>(b)
        );
}

inline bool HasFlag(OperandType flags, OperandType flag) {
    return (flags & flag) == flag;
}

struct Operand
{
    OperandType operand;
    bool is_src;
};

struct Instruction
{
    std::string mnemonc; //example: INC
    uint16_t opcode_mask;
    uint16_t opcode_value;

    std::vector<Operand> operands; //src,dest
    bool is_byte_op;
};

class InstructionSet
{
public:
    InstructionSet()
    {
        
    };

    Instruction findInstruction(std::string mnemonic);

private:
    const std::vector<Instruction> instruction_set;
};


const std::vector<Instruction> instruction_set =
{
    //UNAR OPERAND INSTRUCTIONS
    {
        "CLR",
        0xFFC0,
        0x0A00,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "COM",
        0xFFC0,
        0x0A40,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "INC",
        0xFFC0,
        0x0A80,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "DEC",
        0xFFC0,
        0x0AC0,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "NEG",
        0xFFC0,
        0x0B00,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "TST",
        0xFFC0,
        0x0BC0,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "ASR",
        0xFFC0,
        0x0C80,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "ASL",
        0xFFC0,
        0x0CC0,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "ROR",
        0xFFC0,
        0x0C00,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "ROL",
        0xFFC0,
        0x0C40,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "SWAB",
        0xFFC0,
        0x00C0,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "ADC",
        0xFFC0,
        0x0B40,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "SBC",
        0xFFC0,
        0x0B80,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "SXT",
        0xFFC0,
        0x0DC0,
        {{OperandType::REG | OperandType::MEM,false}},
        false
    },
    //DOUBLE OPERAND INSTRUCTIONS
    {
        "MOV",
        0xF000,
        0x1000,
        {{OperandType::REG | OperandType::IMM | OperandType::MEM,true},{OperandType::REG | OperandType::MEM,false}},
        false //MOVB
    },
    {
        "CMP",
        0xF000,
        0x2000,
        {{OperandType::REG | OperandType::IMM | OperandType::MEM,true},{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "ADD",
        0xF000,
        0x6000,
        {{OperandType::REG | OperandType::IMM | OperandType::MEM,true},{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "SUB",
        0xF000,
        0xD000,
        {{OperandType::REG | OperandType::IMM | OperandType::MEM,true},{OperandType::REG | OperandType::MEM,false}},
        false
    },
    //LOGIC INSTRUTIONS    

    {
        "BIT",
        0xF000,
        0x3000,
        {{OperandType::REG | OperandType::IMM | OperandType::MEM,false},{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "BIC",
        0xF000,
        0x4000,
        {{OperandType::REG | OperandType::IMM | OperandType::MEM,false},{OperandType::REG | OperandType::MEM,false}},
        false
    },
    {
        "BIS",
        0xF000,
        0x5000,
        {{OperandType::REG | OperandType::IMM | OperandType::MEM,false},{OperandType::REG | OperandType::MEM,false}},
        false
    },

    {
        "MUL",
        0xFD00,
        0x7000,
        {{OperandType::REG,false},{OperandType::REG,true}},
        false
    },
    {
        "DIV",
        0xFD00,
        0x7200,
        {{OperandType::REG ,false},{OperandType::REG,true}},
        false
    },
    {
        "ASH",
        0xFD00,
        0x7400,
        {{OperandType::REG ,false},{OperandType::REG,true}},
        false
    },
    {
        "ASHC",
        0xFD00,
        0x7600,
        {{OperandType::REG ,false},{OperandType::REG,true}},
        false
    },
    {
        "XOR",
        0xFD00,
        0x7800,
        {{OperandType::REG ,false},{OperandType::REG,true}},
        false
    },
    //PROGRAMM CONTROL INSTRUCTIONS
    {
        "BR",
        0xFF00,
        0x0100,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BNE",
        0xFF00,
        0x0200,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BEQ",
        0xFF00,
        0x0300,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BPL",
        0xFF00,
        0x8000,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BMI",
        0xFF00,
        0x8100,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BVC",
        0xFF00,
        0x8400,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BVS",
        0xFF00,
        0x8500,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BCC",
        0xFF00,
        0x8600,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BCS",
        0xFF00,
        0x8700,
        {{},{OperandType::REL,false}},
        false
    },
    //SIGNED CONDITION BRANCHES
    {
        "BGE",
        0xFF00,
        0x0400,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BLT",
        0xFF00,
        0x0500,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BGT",
        0xFF00,
        0x0600,
        {{},{OperandType::REL,false}},
        false
    },
    {
        "BLT",
        0xFF00,
        0x0700,
        {{},{OperandType::REL,false}},
        false
    },
    //uNSIGNED BRANCH
    {
        "BHI",
        0xFF00,
        0x8200,
        {{OperandType::REL,false}},
        false
    }, {
        "BLOS",
        0xFF00,
        0x8300,
        {{OperandType::REL,false}},
        false
    }, {
        "BHIS",
        0xFF00,
        0x8600,
        {{OperandType::REL,false}},
        false
    }, {
        "BLO",
        0xFF00,
        0x8700,
        {{OperandType::REL,false}},
        false
    }, {
        "JMP",
        0xFF80,
        0x0040,
        {{OperandType::MEM,false}},
        false
    },

};