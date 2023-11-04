#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <fstream>

class Lexer
{
private:
	std::string TempLexeme;
	int lineCount = 0;
public:
	Lexer();
	std::string lexer(std::ifstream &file);
	std::string AssignToken(const std::string s);
	bool IsSeperator(const std::string s);
	bool IsOperator(const std::string s);
	bool IsKeyword(const std::string s);
	bool IsIdentifier(const std::string s);
	bool IsReal(const std::string s);
	bool IsInteger(const std::string s);
	std::string FormatSpacing(std::string s);
	void SetLexeme(std::string lexeme);
	std::string GetLexeme();
	std::string FormatNoSpace(const std::string s);
};
