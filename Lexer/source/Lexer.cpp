#include "Lexer.h"


const std::string SPACE = "              ";

const std::vector<std::string> SEPERATORS{
	"{","}", "(", ")", "[", "]", ",", ";" 
}; 

const std::vector<std::string> OPERATORS{
	"+", "-", "*", "/", "<", ">", "=", "<=", ">="
};

const std::vector<std::string> KEYWORDS{
	"while", "if", "else", "endif", "get", "put", "ret", "integer", "bool"
};


Lexer::Lexer()
{

}

//converts input file to a lexeme output as a string
std::string Lexer::ConvertToLexeme(std::ifstream &file) const
{
	std::string retString = "OUTPUT\n";

	int index = 0;

	while(file)
	{
		char c;
		file.get(c);

		std::string lexeme = "" + c;
		
		// -------------------------Check for whitespace/operator/seperator ------------------
		//ignore whitespace
		if(lexeme == " ")
		{
			continue;
		}

		if(IsSeperator(lexeme))
		{
			retString.append("Seperator" + SPACE + c + "\n");
			continue;
		}
		else if(IsOperator(lexeme))
		{
			retString.append("Operator" + SPACE + c + "\n");
			continue;
		}

		// -------------------------Check for keyword/identifier/real -------------------
		//scan until whitespace 
		while(c != ' ')
		{		
			lexeme += c;
			file.get(c);			
		}

		if(IsKeyword(lexeme))
		{
			retString.append("Keyword" + SPACE + lexeme + "\n");
			continue;
		}


	}
	


	return retString;
}

bool Lexer::IsSeperator(const std::string s) const
{
	//Check if s is a seperator
	if(std::find(SEPERATORS.begin(), SEPERATORS.end(), s) != SEPERATORS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsOperator(const std::string s) const
{
	if(std::find(OPERATORS.begin(), OPERATORS.end(), s) != OPERATORS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsKeyword(const std::string s) const
{
	if(std::find(KEYWORDS.begin(), KEYWORDS.end(), s) != KEYWORDS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsIdentifier(const std::string s) const
{
	//use FSM
	return false;
}

bool Lexer::IsReal(const std::string s) const
{
	//use FSM
	return false;
}

bool Lexer::IsInteger(const std::string s) const
{
	return false;
}

int Lexer::AssignToken(const std::string s) const
{	
	for(int i=0; i < s.length(); i++)
	{
		//check for 
	}
}
