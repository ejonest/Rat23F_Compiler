
#pragma once

class Lexer
{
	public:
		int CheckLexeme() const;
		bool IsSeperator() const;
		bool IsOperator() const;
		bool IsKeyword() const;
		bool IsIdentifier() const;
		bool IsReal() const;
		bool IsInteger() const;
};
