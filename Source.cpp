#include "Lexer.h"
#include "Parser.h"

int main()
{
	const char* code =
		"MOV R2, R1 \nLABEL:\n.WORD 42";
	Lexer lexer(code);
	lexer.lexAnalisis();
	for (int i = 0; i < lexer.token_list.size(); i++)
	{
		std::cout << lexer.token_list[i].data << std::endl;
	}
	Parser parser(lexer.token_list);
	parser.parse();
	for (const auto& instr : parser.getInstructions()) {
		std::cout << "Instruction: " << instr.mnemonic << std::endl;
	}
	for (const auto& instr : parser.getDirectives()) {
		std::cout << "Directives: " << instr.name << std::endl;
	}
	for (const auto& instr : parser.getLabels()) {
		std::cout << "Labels: " << instr.first << std::endl;
	}
}