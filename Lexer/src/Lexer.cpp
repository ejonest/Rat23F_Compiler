#include "Lexer.h"

const std::string SPACE = "              ";

const int MAX_LETTERS = 12;

const std::vector<std::string> SEPERATORS{
	"{", "}", "(", ")", ",", ";"};

const std::vector<std::string> OPERATORS{
	"+", "-", "*", "/", "<", ">", "=", "<=", ">=, ==, !=, -=, +=, *=, /="};

const std::vector<std::string> KEYWORDS{
	"while", "if", "else", "endif", "get", "put", "ret", "integer", "bool", "function"};

Lexer::Lexer()
{
}

// converts input file to a lexeme output as a string
std::string Lexer::lexer(std::ifstream &file) const
{
	std::cout << "Started" << std::endl;

	std::string retString = "OUTPUT\n";
	char c;

	while (!file.eof())
	{
		file.get(c);

		// -------------------------Check for whitespace ------------------
		// ignore whitespace and next line character
		if (c == ' ' || c == '\n')
		{
			continue;
		}

		//ignore all characters between [* *]
		if(c == '['){
			std::string ignored = "[";
			while(c != ']'){
				file.get(c);
				ignored += c;
			}
			std::cout << "Comment: " << ignored << std::endl;
			continue;
		}

		std::string lexeme = "";
		lexeme += c;

		if (IsSeperator(lexeme))
		{
			retString.append(FormatSpacing("Seperator") + c + "\n");
			continue;
		}

		if(IsOperator(lexeme)){
			// scan until the lexeme isn't a valid operator
			while (IsOperator(lexeme))
			{
				file.get(c);
				std::string s;
				//if next character is not an operator, break
				if(!IsOperator(s += c)){
					break;
				}

				lexeme += c;
			}
			retString.append(FormatSpacing("Operator") + lexeme + "\n");
		}
			
		// -------------------------Check for operator/keyword/identifier/real -------------------

		lexeme = "";
		std::string closingSeperator = "";

		// scan until whitespace
		while (c != ' ' && c != '\n' && !file.eof())
		{
			lexeme += c;
			file.get(c);
			// if scanned character is a seperator
			std::string s;
			if (IsSeperator(s += c))
			{
				closingSeperator = c;
				break;
			}
		}

		retString += AssignToken(lexeme);
		// if there was a closing seperator scanned
		if (closingSeperator != "")
		{
			retString.append(FormatSpacing("Seperator") + closingSeperator + "\n");
		}
	}
	return retString;
}

std::string Lexer::AssignToken(const std::string lexeme) const
{
	std::string token = "";

	if (IsOperator(lexeme))
	{
		token.append(FormatSpacing("Operator") + lexeme + "\n");
	}
	else if (IsKeyword(lexeme))
	{
		token.append(FormatSpacing("Keyword") + lexeme + "\n");
	}
	else if (IsIdentifier(lexeme))
	{
		token.append(FormatSpacing("Identifier") + lexeme + "\n");
	}
	else if (IsReal(lexeme))
	{
		token.append(FormatSpacing("Real") + lexeme + "\n");
	}
	else if (IsInteger(lexeme))
	{
		token.append(FormatSpacing("Integer") + lexeme + "\n");
	}

	return token;
}

bool Lexer::IsComment(const std::string s) const
{
	return false;
}

bool Lexer::IsSeperator(const std::string s) const
{
	// Check if s is a seperator
	if (std::find(SEPERATORS.begin(), SEPERATORS.end(), s) != SEPERATORS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsOperator(const std::string s) const
{
	if (std::find(OPERATORS.begin(), OPERATORS.end(), s) != OPERATORS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsKeyword(const std::string s) const
{
	if (std::find(KEYWORDS.begin(), KEYWORDS.end(), s) != KEYWORDS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsIdentifier(const std::string s) const
{
	// use FSM
	/*
		- starting state: 1
		- accepting state: 2, 3
		- failing states: 1, 3, 4

		l = letter
		d = digit
		o = other

		0	l	n	o
		1	2	5	5
		2	3	4	5
		3	3	4	5
		4	3	4	5
		5   5	5	5
	*/
	int state = 1;

	// while

	int fsm[6][4] = {
		0, 'l', 'd', 'o', // provide clarity and make fsm[state=1] actually fsm[1] and not fsm[2]
		1, 2, 5, 5,
		2, 3, 4, 5,
		3, 3, 4, 5,
		4, 3, 4, 5,
		5, 5, 5, 5};

	for (int i = 0; i < s.length(); i++)
	{
		char c = s[i];

		if (std::isalpha(c))
		{
			state = fsm[state][1];
		}
		else if (std::isdigit(c))
		{
			state = fsm[state][2];
		}
		else
		{
			state = fsm[state][3];
		}
	}

	if (state == 2 || state == 3)
		return true;

	return false;
}

bool Lexer::IsReal(const std::string s) const
{
	// use FSM

	/*
	- starting state: 1
	- accepting state: 4
	- failing states: 1, 2, 3, 5

	d = digit
	. = dot
	o = other

	0	d	.	o
	1	2	5	5
	2	2	3	5
	3	4	5	5
	4	4	5	5
	5	5	5	5
*/
	int state = 1;

	// while

	int fsm[6][4] = {
		0, 'd', '.', 'o', // provide clarity and make fsm[state=1] actually fsm[1] and not fsm[2]
		1, 2, 5, 5,
		2, 2, 3, 5,
		3, 4, 5, 5,
		4, 4, 5, 5,
		5, 5, 5, 5};

	for (int i = 0; i < s.length(); i++)
	{
		char c = s[i];

		if (std::isdigit(c))
		{
			state = fsm[state][1];
		}
		else if (c == '.')
		{
			state = fsm[state][2];
		}
		else
		{
			state = fsm[state][3];
		}
	}

	if (state == 4)
		return true;

	return false;
}

bool Lexer::IsInteger(const std::string s) const
{
	// use FSM

	/*
	- starting state: 1
	- accepting state: 2
	- failing states: 1, 3

	n = number
	o = other
*/
	int state = 1;

	// while

	int fsm[4][3] = {
		0, 'n', 'o', // provide clarity and make fsm[state=1] actually fsm[1] and not fsm[2]
		1, 2, 3,
		2, 2, 3,
		3, 3, 3};

	for (int i = 0; i < s.length(); i++)
	{
		char c = s[i];

		if (std::isdigit(c))
		{
			state = fsm[state][1];
		}
		else
		{
			state = fsm[state][2];
		}
	}

	if (state == 2)
		return true;

	return false;
}

std::string Lexer::FormatSpacing(const std::string s) const
{
	std::string ret = s;
	int n = 0;
	int diff = MAX_LETTERS - s.length();
	for (int i = 0; i < diff; i++)
	{
		ret += " ";
	}
	ret += SPACE;

	return ret;
}
