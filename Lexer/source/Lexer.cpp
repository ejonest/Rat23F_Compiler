#include "Lexer.h"


const std::string SPACE = "              ";

const std::vector<std::string> SEPERATORS{
	"{","}", "(", ")", "[", "]", ",", ";" 
}; 

const std::vector<std::string> OPERATORS{
	"+", "-", "*", "/", "<", ">", "=", "<=", ">=, ==, !"
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

		retString += AssignToken(lexeme);

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
	/*  
		- starting state: 1
		- accepting state: 2
		- failing states: 1, 3, 4
		
		n = number
		l = letter
		o = other

		ex. hi99p -> 1, 2, 2, 3, 3, 2
		ex. p#p -> 1, 2, 4, 4
		ex. 99p -> 1, 4, 4, 4
		ex. h9p9i -> 1, 2, 3, 2, 3, 2 
		
		0	n	l	o
		1	4	2	4
		2	3	2	4	
		3	3	2	4
		4	4	4	4
	*/
	int state = 1;

	//while

	int fsm[5][4] = {
						0, 'n','l','o', //provide clarity and make fsm[state=1] actually fsm[1] and not fsm[2]
						1,	4,	2,	4,
						2,	3,	2,	4,
						3,	3,	3,	4,
						4,	4,	4,	4 		
					};

	for (int i=0; i < s.length(); i++)
	{
		char c = s[i];

		if(std::isdigit(c))
		{
			state = fsm[state][1];
		}
		else if(std::isalpha(c))
		{
			state = fsm[state][2];
		}
		else
		{
			state = fsm[state][3];
		}
	}

	if(state == 2)
		return true;

	return false;
}

bool Lexer::IsReal(const std::string s) const
{
	//use FSM

		/*  
		- starting state: 1
		- accepting state: 2
		- failing states: 1, 3, 4, 5
		
		n = number
		. = dot
		o = other

		ex. 123 -> 1, 3, 3, 3 fail 
		ex. 1.22 -> 1, 3, 4, 2, 2 pass 
		ex. 1.1.1 -> 1, 3, 4, 5 fail
		ex. .1 -> 1, 5, 5 fail
		ex. ..1. -> 5, 5, 5	fail
		
		0	n	.	o
		1	3	5	5 
		2	2	5	5	
		3	3	4	5
		4	2	5	5
		5	5	5	5
	*/
	int state = 1;

		//while

	int fsm[6][4] = {
						0, 'n','.','o', //provide clarity and make fsm[state=1] actually fsm[1] and not fsm[2]
						1,	3,	5,	5,
						2,	2,	5,	5,
						3,	3,	4,	5,
						4,	2,	5,	5,
						5,	5,	5,	5	
					};

	for (int i=0; i < s.length(); i++)
	{
		char c = s[i];

		if(std::isdigit(c))
		{
			state = fsm[state][1];
		}
		else if(c == '.')
		{
			state = fsm[state][2];
		}
		else
		{
			state = fsm[state][3];
		}
	}

	if(state == 2)
		return true;

	return false;
}

bool Lexer::IsInteger(const std::string s) const
{
		//use FSM

		/*  
		- starting state: 1
		- accepting state: 2
		- failing states: 1, 3
		
		n = number
		o = other
	*/
	int state = 1;

		//while

	int fsm[4][3] = {
						0, 'n','o', //provide clarity and make fsm[state=1] actually fsm[1] and not fsm[2]
						1,	2,	3,
						2,	2,	3,
						3,  3,  3 
					};

	for (int i=0; i < s.length(); i++)
	{
		char c = s[i];

		if(std::isdigit(c))
		{
			state = fsm[state][1];
		}
		else
		{
			state = fsm[state][2];
		}
	}

	if(state == 2)
		return true;

	return false;
}

std::string Lexer::AssignToken(const std::string lexeme) const
{		
	std::string token = "";

	if(IsKeyword(lexeme))
	{
		token.append("Keyword" + SPACE + lexeme + "\n");
	}

	else if(IsIdentifier(lexeme))
	{
		token.append("Identifier" + SPACE + lexeme + "\n");
	}

	else if(IsReal(lexeme))
	{
		token.append("Real" + SPACE + lexeme + "\n");
	}

	else if(IsInteger(lexeme))
	{
		token.append("Integer" + SPACE + lexeme + "\n");
	}

	return token;
}
