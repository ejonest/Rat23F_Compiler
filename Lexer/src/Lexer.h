#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Lexer
{
public:
	Lexer();
	std::string lexer(std::ifstream &file) const;
	std::string AssignToken(const std::string s) const;
	bool IsSeperator(const std::string s) const;
	bool IsOperator(const std::string s) const;
	bool IsKeyword(const std::string s) const;
	bool IsIdentifier(const std::string s) const;
	bool IsReal(const std::string s) const;
	bool IsInteger(const std::string s) const;
	bool IsComment(const std::string s) const;
	std::string FormatSpacing(std::string s) const;
};
