#include "Lexer.h"

const std::string SPACE = "              ";

const int MAX_LETTERS = 30;

const std::vector<std::string> SEPERATORS{
	"{", "}", "(", ")", ",", ";", "#"};

const std::vector<std::string> OPERATORS{
"+", "-", "*", "/", "<", ">", "=", "!"};

const std::vector<std::string> KEYWORDS{
	"true", "false", "while", "if", "else", "endif", "get", "put", "ret", "integer", "bool", "function"};

Lexer::Lexer()
{
}

// converts input file to a lexeme output as a string
std::string Lexer::lexer(std::ifstream &file)
{
	// std::ifstream file;
	// file.open(fileName);
	// std::cout << "Started" << std::endl;

	std::string output = "";
	char c;
	// file.get(c);
	// file.get(c);
	// file.get(c);
	while (!file.eof())
	{
		file.get(c);

		// -------------------------Check for whitespace ------------------
		if (c == '\n') {
			// std::cout << "========Curr count: " << lineCount << "========\n";
			lineCount += 1;
		}
		// ignore whitespace and next line character
		if (isblank(c) || isspace(c))
		{	
			continue;
		}

		// ignore all characters between [* *]
		if (c == '[')
		{
			std::string ignored = "[";
			while (c != ']')
			{
				file.get(c);
				ignored += c;
			}
			// std::cout << "Comment: " << ignored << std::endl;
			//ignoring comments for syntax checker
			// output.append(FormatSpacing("Comment:") + ignored + "\n");
			continue;
		}

		std::string lexeme = "";
		lexeme += c;

		if (IsSeperator(lexeme))
		{
			output.append(FormatSpacing("Token: Seperator") + "Lexeme: " + lexeme + "\n");
			SetLexeme(lexeme);
			continue;
		}

		if (IsOperator(lexeme))
		{
			char peek = file.peek();
			std::string s = "";
			//check if peeked character is an operator
			if(IsOperator(s + peek))
			{
				file.get(c);
				lexeme += c;
			}
			output.append(FormatSpacing("Token: Operator") + "Lexeme: " + lexeme + "\n");
			SetLexeme(lexeme);
			continue;
		}

		// -------------------------Check for keyword/identifier/real -------------------
		char peek;

		if(lexeme == " " || c == ' '){
			std::cout << "Space Detected" << std::endl;
		}

		// scan until whitespace
		while (!file.eof())
		{
			peek = file.peek();
			// if peeked char is a seperator or operator, break
			std::string s;
			if(IsOperator(s+peek) || IsSeperator(s+peek) || isblank(peek) || isspace(peek))
				break;

			file.get(c);		
			lexeme += c;
		}

		if(isspace(lexeme[0])){
			std::cout << "Space caught on" << lexeme << std::endl;
			SetLexeme(lexeme);
		}

		std::string token = AssignToken(lexeme);
		output += token;		
		
	}
	return output;
}

std::string Lexer::AssignToken(const std::string lexeme) 
{
	std::string token = "";

	if (IsKeyword(lexeme))
	{
		token.append(FormatSpacing("Token: Keyword") + "Lexeme: " + lexeme + "\n");
	}
	else if (IsIdentifier(lexeme))
	{
		token.append(FormatSpacing("Token: Identifier") + "Lexeme: " + lexeme + "\n");
	}
	else if (IsReal(lexeme))
	{
		token.append(FormatSpacing("Token: Real") + "Lexeme: " + lexeme + "\n");
	}
	else if (IsInteger(lexeme))
	{
		token.append(FormatSpacing("Token: Integer") + "Lexeme: " + lexeme + "\n");
	}
	else
	{
		token.append(FormatSpacing("Token: Invalid") + "Lexeme: " + lexeme + "\n");
	}

	SetLexeme(lexeme);
	return token;
}

bool Lexer::IsSeperator(const std::string s) 
{
	// Check if s is a seperator
	if (std::find(SEPERATORS.begin(), SEPERATORS.end(), s) != SEPERATORS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsOperator(const std::string s) 
{
	if (std::find(OPERATORS.begin(), OPERATORS.end(), s) != OPERATORS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsKeyword(const std::string s) 
{
	if (std::find(KEYWORDS.begin(), KEYWORDS.end(), s) != KEYWORDS.end())
	{
		return true;
	}
	return false;
}

bool Lexer::IsIdentifier(const std::string s) 
{
	// use FSM
	/*
		- starting state: 1
		- accepting state: 3
		- failing states: 1, 2, 3, 4

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

bool Lexer::IsReal(const std::string s) 
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

bool Lexer::IsInteger(const std::string s) 
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

std::string Lexer::FormatSpacing(const std::string s) 
{
	std::string lineNumString = std::to_string(lineCount);
	std::string ret = FormatNoSpace(lineNumString);
	ret += s;
	int n = 0;
	int diff = MAX_LETTERS - s.length();
	for (int i = 0; i < diff; i++)
	{
		ret += " ";
	}
	ret += SPACE;

	return ret;
}
std::string Lexer::FormatNoSpace(const std::string s) 
{
	std::string ret = s;
	int n = 0;
	int diff = 3 - s.length();
	for (int i = 0; i < diff; i++)
	{
		ret += " ";
	}

	return ret;
}

void Lexer::SetLexeme(std::string lexeme){
	TempLexeme = lexeme;
}

std::string Lexer::GetLexeme() {
	return TempLexeme;
}