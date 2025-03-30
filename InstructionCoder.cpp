#include "InstructionCoder.h"


InstructionCoder::InstructionCoder(const std::unordered_map<std::string, uint16_t>& labels)
    : label_table_(labels) {}

// Кодирование одного операнда
uint16_t InstructionCoder::encodeOperand(const Operand& op, uint16_t current_pc, bool is_src) {
    uint16_t encoded = 0;

    // Регистровый режим (R0-R7)
    if (HasFlag(op.operand, OperandType::REG)) {
        encoded = (op.value & 7);
    }
    // Непосредственное значение (#42)
    else if (HasFlag(op.operand, OperandType::IMM)) {
        encoded = 0b111000;  // Режим 7 (PC)+
        // Для immediate-значений потребуется дополнительное слово
    }
    // Относительная адресация (метки)
    else if (HasFlag(op.operand, OperandType::REL)) {
        auto it = label_table_.find(op.label);
        if (it != label_table_.end()) {
            int offset = it->second - (current_pc + 2);
            encoded = 0b111000;  // Режим 7 (PC)+
        }
    }
    // Косвенная адресация (MEM)
    else if (HasFlag(op.operand, OperandType::MEM)) {
        encoded = 0b010000 | (op.value & 7);  // Режим 2 (Rn)
    }

    return encoded;
}

// Кодирование полной инструкции
uint16_t InstructionCoder::encodeInstruction(
    const Instruction& def,
    const ParsedInstruction& instr,
    uint16_t current_pc
) {
    uint16_t word = def.opcode_value;

    for (size_t i = 0; i < def.operands.size(); ++i) {
        uint16_t encoded = encodeOperand(instr.operands[i], current_pc, def.operands[i].is_src);
        if (def.operands[i].is_src) {
            word |= (encoded << 6);  // SRC: биты 6-11
        }
        else {
            word |= encoded;         // DST: биты 0-5
        }
    }

    return word;
}

// Генерация машинного кода для всей программы
std::vector<uint16_t> InstructionCoder::generateMachineCode(
    const std::vector<ParsedInstruction>& program,
    const std::unordered_map<std::string, uint16_t>& labels
) {
    std::vector<uint16_t> machine_code;
    uint16_t pc = 0;

    for (const auto& instr : program) {
        const auto& def = findInstruction(instr.mnemonic);  // Из вашего InstructionSet
        uint16_t encoded = encodeInstruction(def, instr, pc);
        machine_code.push_back(encoded);
        pc += 2;  // Большинство команд PDP-11 занимают 1 слово (2 байта)
    }

    return machine_code;
}
