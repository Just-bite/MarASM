#include "InstructionCoder.h"


InstructionCoder::InstructionCoder(const std::unordered_map<std::string, uint16_t>& labels)
    : label_table_(labels) {}

uint16_t InstructionCoder::encodeOperand(const Operand& op, uint16_t current_pc, bool is_src) {
    uint16_t encoded = 0;

    //(R0-R7)
    if (HasFlag(op.operand, OperandType::REG)) {
        encoded = (op.value & 7);
    }
    // (#42)
    else if (HasFlag(op.operand, OperandType::IMM)) {
        encoded = 0b111000;  
    }
    //  (labels)
    else if (HasFlag(op.operand, OperandType::REL)) {
        auto it = label_table_.find(op.label);
        if (it != label_table_.end()) {
            int offset = it->second - (current_pc + 2);
            encoded = 0b111000; 
        }
    }
    //  (MEM)
    else if (HasFlag(op.operand, OperandType::MEM)) {
        encoded = 0b010000 | (op.value & 7);  
    }

    return encoded;
}

uint16_t InstructionCoder::encodeInstruction(
    const Instruction& def,
    const ParsedInstruction& instr,
    uint16_t current_pc
) {
    uint16_t word = def.opcode_value;

    for (size_t i = 0; i < def.operands.size(); ++i) {
        uint16_t encoded = encodeOperand(instr.operands[i], current_pc, def.operands[i].is_src);
        if (def.operands[i].is_src) {
            word |= (encoded << 6);  // SRC: 6-11
        }
        else {
            word |= encoded;         // DST: 0-5
        }
    }

    return word;
}


std::vector<uint16_t> InstructionCoder::generateMachineCode(
    const std::vector<ParsedInstruction>& program,
    const std::unordered_map<std::string, uint16_t>& labels
) {
    std::vector<uint16_t> machine_code;
    uint16_t pc = 0;

    for (const auto& instr : program) {
        const auto& def = findInstruction(instr.mnemonic); 
        uint16_t encoded = encodeInstruction(def, instr, pc);
        machine_code.push_back(encoded);
        pc += 2;  
    }

    return machine_code;
}
