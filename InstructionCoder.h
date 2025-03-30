#pragma once
#include "InstructionSet.h"  
#include <cstdint>
#include <vector>
#include <unordered_map>

class InstructionCoder {
public:
    InstructionCoder(const std::unordered_map<std::string, uint16_t>& labels = {});

  
    uint16_t encodeInstruction(const Instruction& def, const ParsedInstruction& instr, uint16_t current_pc);

    
    uint16_t encodeOperand(const Operand& op, uint16_t current_pc, bool is_src);

    std::vector<uint16_t> generateMachineCode(
        const std::vector<ParsedInstruction>& program,
        const std::unordered_map<std::string, uint16_t>& labels
    );

private:
    const std::unordered_map<std::string, uint16_t>& label_table_;  // —сылка на таблицу меток
};