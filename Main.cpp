#include "Lexer.h"
#include "Parse_to_AST.h"

int main()
{
	std::string code =
		R"(
		MOV R2, R1
		MOV (R1),R2
		MOV (R1)+,R2
		MOV -(R1),R2
		MOV @-(R1),R2
		MOV @(R1)+,R2
		MOV 42(R1),R2
		MOV @42(R1),R2
		MOV #42,R2
		MOV @#1000,R2
		JMP LABEL
		CLR R4 	
		LABEL:
	    .WORD 42)";
	Lexer lexer(code);
	lexer.runLexAnalisis();
	for (const auto elements: lexer.getTokenList())
	{
		std::cout << elements.data <<" ";
	}
	std::cout << std::endl;
	/*Parse_to_AST parser(lexer.getTokenList());
	parser.parseCode();
	ASTRoot root = parser.getASTForest();
	int poslock = parser.currentAdress;
	std::cout << poslock << std::endl;*/
}