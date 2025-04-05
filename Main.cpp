#include "Lexer.h"
#include "Parser.h"
#include "Parse_to_AST.h"

int main()
{
	std::string code =
		R"(
		MOV R2, R1
		ADD R1, R3
		CLR R4 	
		LABEL:
	    .WORD 42)";
	Lexer lexer(code);
	lexer.lexAnalisis();
	for (int i = 0; i < lexer.token_list.size(); i++)
	{
		std::cout << lexer.token_list[i].data << std::endl;
	}
	Parse_to_AST parser(lexer.token_list);
	parser.parseCode();
	ASTRoot root = parser.getASTForest();
	int poslock = parser.currentAdress;
	std::cout << poslock << std::endl;
}