#include <iostream>
#include <queue>
#include "Lexer.h"

void SyntaxChecker(std::string fileName);
bool RAT23F(std::ifstream &readFile);
bool OptFuncDef(std::ifstream &readFile);
bool FuncDefs(std::ifstream &readFile);
bool FuncDefsPrim(std::ifstream &readFile);
bool Func(std::ifstream &readFile);
bool OptParmList(std::ifstream &readFile);
bool ParmList(std::ifstream &readFile);
bool ParmListPrim(std::ifstream &readFile);
bool Parameter(std::ifstream &readFile);
bool Qualifier(std::ifstream &readFile);
bool Body(std::ifstream &readFile);
bool OptDecList(std::ifstream &readFile);
bool DecList(std::ifstream &readFile);
bool DecListPrim(std::ifstream &readFile);
bool Declaration(std::ifstream &readFile);
bool IDs(std::ifstream &readFile);
bool IDsPrime(std::ifstream &readFile);
bool StateList(std::ifstream &readFile);
bool StateListPrime(std::ifstream &readFile);
bool State(std::ifstream &readFile);
bool Compound(std::ifstream &readFile);
bool Assign(std::ifstream &readFile);
bool If(std::ifstream &readFile);
bool IfPrim(std::ifstream &readFile);
bool Ret(std::ifstream &readFile);
bool RetPrime(std::ifstream &readFile);
bool Print(std::ifstream &readFile);
bool Scan(std::ifstream &readFile);
bool Whi(std::ifstream &readFile);
bool Condition(std::ifstream &readFile);
bool Relop(std::ifstream &readFile);
bool Expression(std::ifstream &readFile);
bool ExpressionPrime(std::ifstream &readFile);
bool Term(std::ifstream &readFile);
bool TermPrime(std::ifstream &readFile);
bool Factor(std::ifstream &readFile);
bool Primary(std::ifstream &readFile);
bool Empty(std::ifstream &readFile);
std::string SetTokLex(std::ifstream &readFile);