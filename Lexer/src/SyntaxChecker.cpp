#include "SyntaxChecker.h"
#include <stack>
#include <iomanip>
#include <iostream>
#define BLANK -9999

// Initialize for syntax checker
bool textOn = true;
std::string token;
std::string lexeme;
std::string tempString;
std::queue<std::string> ruleBacklog;
int emptyUsed = 0;
std::string lineCountString = "";
// Initialize for symbol table and instruction table
int memory_address = 7000;
int count_symbol = 0;
bool isFromDeclaration = false;
std::string prevLexeme;
std::string temp;
std::string save;

// Default constructor
Syntax::Syntax(){
    symbol_index = 0;
    symbol_table[symbol_index].mem_loc = 0;
    instr_idx = 1;
}
// Destructor
Syntax::~Syntax()
{
	memory_address = 7000;
	count_symbol = 0;
	isFromDeclaration = false;
	prevLexeme = "";
}
bool Syntax::check_symbol(std::string lexeme){
    for (int i = 0; i < symbol_index; i++){
        if (lexeme == symbol_table[i].id){
            save = lexeme;
            if (isFromDeclaration){
                count_symbol++;
                return true;
            }
        }
    }
    return false;
}
void Syntax::gen_symbol(std::string lexeme, std::string id_type) {
    bool exists = false;
    for (int i = 0; i < symbol_index; i++) {
        if (symbol_table[i].id == lexeme) {
            exists = true;
            break;
        }
    }
    if (!exists) {
        symbol_table[symbol_index].id = lexeme;
        symbol_table[symbol_index].mem_loc = memory_address;
        symbol_table[symbol_index].id_type = id_type;
        symbol_index++;
        memory_address++;
    }
}
int Syntax::get_address(std::string save) const
{
	int address = 0;
	for (int i = 0; i < symbol_index; i++) {
		if (symbol_table[i].id == save) {
			address = symbol_table[i].mem_loc;
		}
	}
	return address;
}
void Syntax::back_patch(int jump_address)
{
	int address = jumpstack.top();
	jumpstack.pop();
	instr_table[address].oprnd = jump_address;
}

void Syntax::gen_instr(std::string op, int oprnd)
{
	instr_table[instr_idx].address = instr_idx;
	instr_table[instr_idx].op = op;
	instr_table[instr_idx].oprnd = oprnd;
	instr_idx++;
}
std::string Syntax::get_type(std::string input) {
    std::string var = "";
    for (int i = 0; i < symbol_index; i++) {
        if (symbol_table[i].id == input) {
            var = symbol_table[i].id_type;
        }
    }
    if (input == "true" || input == "false") {
        var = "bool";
    }
    return var;
}
void Syntax::check_TypeMatch(std::string prevLexeme, std::string lexeme, std::ifstream &readFile) {
    if (get_type(prevLexeme) == "bool" || get_type(lexeme) == "bool" ) {
        std::cout << "Bool does not accept arithmetic operations\n";
    }
}
void Syntax::SyntaxChecker(std::string fileName) {
    std::ifstream readFile;
    readFile.open("output.txt");
    int j = 7;
    std::getline(readFile, tempString);
    if (RAT23F(readFile)) {
        // std::cout << "hello?";
        std::cout << "\nSYMBOL TABLE\n";
        std::cout << std::left << std::setw(13) << "Identifier" << std::setw(10) << "Memory" << std::setw(10) << "Type" << std::endl;
	    for (int i = 0; i < symbol_index; i++)
	    {
		    std::cout << std::left << std::setw(13) << symbol_table[i].id << std::setw(10) << symbol_table[i].mem_loc << std::setw(10) << symbol_table[i].id_type;
            std::cout << endl;
	    }

        std::string a;
	    std::cout << "\nINSTRUCTION TABLE\n";
	    for (int i = 1; i < instr_idx; i++)
	    {
		    if (instr_table[i].oprnd == BLANK) {
			    a = "";
			    std::cout << instr_table[i].address << " " << instr_table[i].op << " " << a;
			    std::cout << endl;
		    }
		    else {
			    std::cout << instr_table[i].address << " " << instr_table[i].op << " " << instr_table[i].oprnd;
			    std::cout << endl;
		    }
	    }
        std::cout << "==========================Done==========================\n";
    }
    readFile.close();
    exit(0);
}
std::string Syntax::SetTokLex(std::ifstream &readFile) {
    std::getline(readFile, tempString);
    int j = 10;
    while (tempString[j] != ' ' && tempString[j] != '\t' && tempString[j] != '\n') {j++;}
    token = tempString.substr(10, j);
    token.erase(remove(token.begin(), token.end(), ' '), token.end());
    lexeme = tempString.substr(55);
    // std::cout << "=====================================" << lexeme << "===================================\n";
    // std::cout << "=====================================" << save << "===================================\n";
    lexeme.erase(remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
    lineCountString = tempString.substr(0, 2);
    return "\n" + tempString.substr(3);
}
// R1
bool Syntax::RAT23F(std::ifstream &readFile) {
    // <Rat23F> ::= <Opt Function Definitions> # <Opt Declaration List> <Statement List> #
    std::cout << "<Rat23F> ::= <Opt Function Definitions> # <Opt Declaration List> <Statement List> #\n";
    OptFuncDef(readFile);
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "#") {
        OptDecList(readFile);
        StateList(readFile);
        if (lexeme != "#") {
            std::cout << "ERROR: Line# " << lineCountString << " Line# " << lineCountString << " expected lexeme \"#\". Received " << lexeme << "\n";
            exit(0);
        }
    } else {
        std::cout << "ERROR: Line# " << lineCountString << " Line# " << lineCountString << " expected lexeme \"#\". Received " << lexeme << "\n";
        exit(0);
    }
    return true;
}
// R2
bool Syntax::OptFuncDef(std::ifstream &readFile) {
    // <Opt Function Definitions> ::= <Function Definitions> | <Empty>
    std::cout << "<Opt Function Definitions> ::= <Function Definitions> | <Empty>\n";
    if(!FuncDefs(readFile)) {
        Empty(readFile);
    }
    return true;
}
// R3
bool Syntax::FuncDefs(std::ifstream &readFile) {
    // <Function Definitions> ::= <Function> <Function Definitions*>
    std::cout << "<Function Definitions> ::= <Function> <Function Definitions*>\n";
    if (Func(readFile)) {
        FuncDefsPrim(readFile);
    } else {
        Empty(readFile);
    }
    return true;
}
// R4
bool Syntax::FuncDefsPrim(std::ifstream &readFile) {
    // <Function Definitions Prime> ::= <Function> <Function Definitions*> | <empty>
    std::cout << "<Function Definitions Prime> ::= <Function> <Function Definitions*> | <empty>\n";
    if (Func(readFile)) {
        FuncDefsPrim(readFile);
    } else {
        Empty(readFile);
    }
    return true;
}
// R5
bool Syntax::Func(std::ifstream &readFile) {
    // <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
    std::cout << "<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "function") {
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (token == "Identifier") {
            if (emptyUsed > 0) {
                emptyUsed -= 1;
            } else {
                std::cout << SetTokLex(readFile) << "\n";
            }
            if (lexeme == "(") {
                OptParmList(readFile);
                if (emptyUsed > 0) {
                    emptyUsed -= 1;
                } else {
                    std::cout << SetTokLex(readFile) << "\n";
                }
                if (lexeme == ")") {
                    OptDecList(readFile);
                    Body(readFile);
                } else {
                    std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \")\". Received " << lexeme << "\n";
                    exit(0);
                }
            } else {
                std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \"(\". Received " << lexeme << "\n";
                exit(0);
            }
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected token \"Identifier\". Received " << token << "\n";
            exit(0);
        }
    } else {
        // std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \"function\". Received " << lexeme << "\n";
        // exit(0);
        return false;
    }
    return true;
}
// R6
bool Syntax::OptParmList(std::ifstream &readFile) {
    // <Opt Parameter List> ::= <Parameter List> | <Empty>
    std::cout << "<Opt Parameter List> ::= <Parameter List> | <Empty>\n";
    if (!ParmList(readFile)) {
        Empty(readFile);
    }
    return true;
}
// R7
bool Syntax::ParmList(std::ifstream &readFile) {
    // <Parameter List> ::= <Parameter> <Parameter List*> 
    std::cout << "<Parameter List> ::= <Parameter> <Parameter List*>\n";
    Parameter(readFile);
    ParmListPrim(readFile); 
    return true;  
}
// R8
bool Syntax::ParmListPrim(std::ifstream &readFile) {
    // <Parameter List Prime> ::= , <Parameter List> | <empty>
    std::cout << "<Parameter List Prime> ::= , <Parameter List> | <empty>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == ",") {
        ParmList(readFile);
    } else {
        Empty(readFile);
    }
    return true;
}
// R9
bool Syntax::Parameter(std::ifstream &readFile) {
    // <Parameter> ::= <IDs> <Qualifier>
    std::cout << "<Parameter> ::= <IDs> <Qualifier>\n";
    IDs(readFile);
    Qualifier(readFile);
    return true;
}
// R10
bool Syntax::Qualifier(std::ifstream &readFile) {
    // <Qualifier> ::= integer | bool | real
    std::cout << "<Qualifier> ::= integer | bool | real\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme != "integer" && lexeme != "bool" && lexeme != "real") {
        std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \"integer\", \"bool\", or \"real\". Received " << lexeme << "\n";
        exit(0);
    }
    return true;
}
// R11
bool Syntax::Body(std::ifstream &readFile) {
    // <Body> ::= { < Statement List> }
    std::cout << "<Body> ::= { < Statement List> }\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "{") {
        StateList(readFile);
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme != "}") {
            std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \"}\". Received " << lexeme << "\n";
            exit(0);
        }
    } else {
        std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \"{\". Received " << lexeme << "\n";
        exit(0);
        // return false;
    }
    return true;
}
// R12
bool Syntax::OptDecList(std::ifstream &readFile) {
    // <Opt Declaration List> ::= <Declaration List> | <Empty>
    std::cout << "<Opt Declaration List> ::= <Declaration List> | <Empty>\n";
    if (!DecList(readFile)) {
        Empty(readFile);
    }
    return true;
}
// R13
bool Syntax::DecList(std::ifstream &readFile) {
    // <Declaration List> := <Declaration> ; <Declaration List*>
    std::cout << "<Declaration List> := <Declaration> ; <Declaration List*>\n";
    Declaration(readFile);
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == ";") {
        DecListPrim(readFile);
    } else {
        // std::cout << "ERROR: Line# " << lineCountString << "  expected \";\"\n";
        // exit(0);
        return false;
    }
    return true;
}
// R14
bool Syntax::DecListPrim(std::ifstream &readFile) {
    // <Declaration List Prime> := <Declaration> ; <Declaration List*> | <empty>
    std::cout << "<Declaration List Prime> := <Declaration> ; <Declaration List*> | <empty>\n";
    if (Declaration(readFile)) {
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme == ";") {
            DecList(readFile);
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected \";\". Received " << lexeme << "\n";
            exit(0);
        }
    } else {
        Empty(readFile);
    }
    return true;
}
// R15
bool Syntax::Declaration(std::ifstream &readFile) {
    // <Declaration> ::= integer <IDs>| bool <IDs>| real <IDs>
    std::cout << "<Declaration> ::= integer <IDs>| bool <IDs>| real <IDs>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "integer") {
        current_type = lexeme;
        IDs(readFile);
        return true;
    } else if (lexeme == "bool") {
        current_type = lexeme;
        IDs(readFile);
        return true;
    } else if (lexeme == "real") {
        IDs(readFile);
        return true;
    } else {
        // std::cout << "ERROR: Line# " << lineCountString << "  expected \"integer\", \"bool\", \"real\". Received " << lexeme << "\n";
        // exit(0);
        // std::cout << "HERE\n";
        emptyUsed += 1;
        return false;
    }
}
// R16
bool Syntax::IDs(std::ifstream &readFile) {
    // <IDs> ::= <Identifier> <IDs*>
    std::cout << "<IDs> ::= <Identifier> <IDs*>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (token == "Identifier") {
        // std::cout << "Testing";
        if (!check_symbol(lexeme))
		{
			gen_symbol(lexeme, current_type);
		}
        if (count_symbol == 2) {
            std::cout << "Identifier " << lexeme << " has been declared more than once\n";
        }

        IDsPrime(readFile);
        
        return true;
    } else {
        std::cout << "ERROR: Line# " << lineCountString << "  expected token \"Identifier\". Received " << token << "\n";
        exit(0);
        // return false;
    }
}
// R17
bool Syntax::IDsPrime(std::ifstream &readFile) {
    // <IDs*> ::= , <IDs> | <empty>
    std::cout << "<IDs*> ::= , <IDs> | <empty>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == ",") {
        IDs(readFile);
    } else {
        Empty(readFile);
    }
    
    return true;
}
// R18
bool Syntax::StateList(std::ifstream &readFile) {
    // <Statement List> ::= <Statement>  <Statement List*>
    std::cout << "<Statement List> ::= <Statement>  <Statement List*>\n";
    State(readFile);
    StateListPrime(readFile);
    return true;
}
// R19
bool Syntax::StateListPrime(std::ifstream &readFile) {
    // <Statement List Prime> ::= <Statement>  <Statement List*> | <empty>
    std::cout << "<Statement List Prime> ::= <Statement>  <Statement List*> | <empty>\n";
    if(State(readFile)) {
        StateListPrime(readFile);
    } else {
        Empty(readFile);
    }
    
    return true;
}
// R20
bool Syntax::State(std::ifstream &readFile) {
    // <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
    std::cout << "<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (Compound(readFile)) {
        return true;
    } else if (Assign(readFile)) {
        // std::string save = lexeme;
        // Assign(readFile);
        // int addr = get_address(save);
        // gen_instr("POPM - a", addr);
        return true;
    } else if (If(readFile)) {
        return true;
    } else if (Ret(readFile)) {
        return true;
    } else if (Print(readFile)) {
        return true;
    } else if (Scan(readFile)) {
        // std::string save = lexeme;
        // Scan(readFile);
        // int addr = get_address(save);
		// gen_instr("STDIN", BLANK);
		// gen_instr("POPM - b", addr);
        return true;
    } else if (Whi(readFile)) {
        return true;
    } else {
        return false;
    }
}
// R21
bool Syntax::Compound(std::ifstream &readFile) {
    // <Compound> ::= { <Statement List> }
    if (lexeme == "{") {
        std::cout << "<Compound> ::= { <Statement List> }\n";
        StateList(readFile);
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme != "}") {
            std::cout << "ERROR: Line# " << lineCountString << " expected lexeme \"}\". Received " << lexeme << "\n";
            exit(0);
        } else {
            return true;
        }
    } else {
        return false;
    }
}
// R22
bool Syntax::Assign(std::ifstream &readFile) {
    // <Assign> ::= <Identifier> = <Expression> ;
    std::cout << "<Assign> ::= <Identifier> = <Expression> ;\n";
    if (token == "Identifier") {
        std::cout << "<Assign> ::= <Identifier> = <Expression> ;\n";
        save = lexeme;
        std::cout << SetTokLex(readFile) << "\n";

        if (emptyUsed > 0) {
            emptyUsed -= 1;
        }
        if (lexeme == "=") {
            Expression(readFile);
            // std::cout << "===========Assign save is: " << save << "===========\n\n\n";
            int addr = get_address(save);
            // std::cout << "===========Assign save add is: " << addr << "===========\n\n\n";
            gen_instr("POPM", addr);
            if (emptyUsed > 0) {
                emptyUsed -= 1;
            } else {
                std::cout << SetTokLex(readFile) << "\n";
            }
            if (lexeme != ";") {
                std::cout << "ERROR: Line# " << lineCountString << "  expected \";\". Received " << lexeme << "\n";
                exit(0);
            } else {
                return true;
            }
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected \"=\". Received " << lexeme << "\n";
            exit(0);
        }
    } else {
        return false;
    }
}
// R23
bool Syntax::If(std::ifstream &readFile) {
    // <If> ::= if ( <Condition> ) <Statement> <If*>
    if (lexeme == "if") {
        std::cout << "<If> ::= if ( <Condition> ) <Statement> <If*>\n";
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme == "(") {
            Condition(readFile);

            gen_instr("JUMPZ", BLANK);
            jumpstack.push(instr_idx);

            if (emptyUsed > 0) {
                emptyUsed -= 1;
            } else {
                std::cout << SetTokLex(readFile) << "\n";
            }
            // gen_instr("JUMPZ", BLANK);
            // jumpstack.push(instr_idx);

            if (lexeme == ")") {
                State(readFile);

                back_patch(instr_idx);
                gen_instr("LABEL", BLANK);

                IfPrim(readFile);
                
                return true;
            } else {
                std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \")\". Received " << lexeme << "\n";
                exit(0);
            }
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected \"(\". Received " << lexeme << "\n";
            exit(0);
        }
    } else {
        //ruleBacklog.pop();
        return false;
    }
}
// R24
bool Syntax::IfPrim(std::ifstream &readFile) {
    // <If Prime> ::= endif | else <Statement> endif
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "endif") {
        std::cout << "<If Prime> ::= endif | else <Statement> endif\n";
        return true;
    } else if (lexeme == "else") {
        std::cout << "<If Prime> ::= endif | else <Statement> endif\n";
        
        int addr = jumpstack.top();
        jumpstack.pop();
        jumpstack.push(instr_idx);
        gen_instr("JUMP", BLANK);
        gen_instr("LABEL", BLANK);
        back_patch(instr_idx);
        jumpstack.push(addr);

        State(readFile);
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme == "endif") {
            return true;
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected \"endif\". Received " << lexeme << "\n";
            exit(0);
        }
    } else {
        //ruleBacklog.pop();
        return false;
    }
}
// R25
bool Syntax::Ret(std::ifstream &readFile) {
    // <Return> ::= ret <Return*>
    if (lexeme == "ret") {
        std::cout << "<Return> ::= ret <Return*>\n";
        RetPrime(readFile);
        return true;
    } else {
        //ruleBacklog.pop();
        return false;
    }
}
// R26
bool Syntax::RetPrime(std::ifstream &readFile) {
    // <Return*> ::= ; | <Expression> ;
    if (lexeme == ";") {
        std::cout << "<Return*> ::= ; | <Expression> ;\n";
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        return true;
    } else if (Expression(readFile)) {
        std::cout << "<Return*> ::= ; | <Expression> ;\n";
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme == ";") {
            return true;
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expeced \";\". Received " << lexeme << "\n";
            exit(0);
        }
        // return true;
    } else {
        //ruleBacklog.pop();
        return false;
    }
}
// R27
bool Syntax::Print(std::ifstream &readFile) {
    // <Print> ::= put ( <Expression>);
    if (lexeme == "put") {
        std::cout << "<Print> ::= put ( <Expression> );\n";
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme == "(") {
            Expression(readFile);
            if (emptyUsed > 0) {
                emptyUsed -= 1;
            } else {
                std::cout << SetTokLex(readFile) << "\n";
            }
            if (lexeme == ")") {
                gen_instr("STDOUT", BLANK);
                if (emptyUsed > 0) {
                    emptyUsed -= 1;
                } else {
                    std::cout << SetTokLex(readFile) << "\n";
                }
                if (lexeme == ";") {
                    return true;
                } else {
                    //ruleBacklog.pop();
                    return false;
                }
            } else {
                std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \")\". Received " << lexeme << "\n";
                exit(0);
            }
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \"(\". Received " << lexeme << "\n";
            exit(0);
        }
    }
    //ruleBacklog.pop();
    return false;
}
// R28
bool Syntax::Scan(std::ifstream &readFile) {
    // <Scan> ::= get ( <IDs> );
    if (lexeme == "get") {
        std::cout << "<Scan> ::= get ( <IDs> );\n";
        // emptyUsed += 1;
        // if emptyUsed here then it will work with multiple "get" inputs
        emptyUsed -= 1;
        // std::cout << "===========emptyUsed = " << emptyUsed << "===========\n";
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        // std::cout << "===========emptyUsed = " << emptyUsed << "===========\n";
        if (lexeme == "(") {
            // std::string save = lexeme;
            IDs(readFile);
            if (emptyUsed > 0) {
                emptyUsed -= 1;
            } else {
                std::cout << SetTokLex(readFile) << "\n";
            }
            if (lexeme == ")") {

                int addr = get_address(save);
				gen_instr("STDIN", BLANK);
				gen_instr("POPM", addr);

                if (emptyUsed > 0) {
                    emptyUsed -= 1;
                } else {
                    std::cout << SetTokLex(readFile) << "\n";
                }
                if (lexeme == ";") {
                    return true;
                } else {
                    std::cout << "ERROR: Line# " << lineCountString << "  expected \";\". Received " << lexeme << "\n";
                    exit(0);
                }
            } else {
                std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \")\". Received " << lexeme << "\n";
                exit(0);
            }
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \"(\". Received " << lexeme << "\n";
            exit(0);
        }
    } else {
        //ruleBacklog.pop();
        return false;
    }
}
// R29
bool Syntax::Whi(std::ifstream &readFile) {
    // <While> ::= while ( <Condition> ) <Statement>
    // std::cout << "<While> ::= while ( <Condition> ) <Statement>\n";
    if (lexeme == "while") {
        std::cout << "<While> ::= while ( <Condition> ) <Statement>\n";
        
        int addr = instr_idx;
        gen_instr("LABEL", BLANK);

        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme == "(") {
            Condition(readFile);
            if (emptyUsed > 0) {
                emptyUsed -= 1;
            } else {
                std::cout << SetTokLex(readFile) << "\n";
            }
            if (lexeme == ")" || lexeme == "{") {
                State(readFile);
                gen_instr("JUMP", addr);
                back_patch(instr_idx);
                // Not needed. The professors does not have this line
                // gen_instr("LABEL4", BLANK);
                return true;
            } else {
                std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \")\". Received " << lexeme << "\n";
                exit(0);
            }
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \"(\". Received " << lexeme << "\n";
            exit(0);
        }
    } else {
        //ruleBacklog.pop();
        return false;
    }
}
// R30
bool Syntax::Condition(std::ifstream &readFile) {
    // <Condition> ::= <Expression> <Relop> <Expression>
    std::cout << "<Condition> ::= <Expression> <Relop> <Expression>\n";
    Expression(readFile);
    std::string op = lexeme;
    Relop(readFile);
    Expression(readFile);

    if (op == "<"){
        gen_instr("LES", BLANK);
        jumpstack.push(instr_idx);
        gen_instr("JUMPZ", BLANK);
    } else if (op == ">"){
        gen_instr("GRT", BLANK);
        jumpstack.push(instr_idx);
        gen_instr("JUMPZ", BLANK);
    }
    else if (op == "=>"){
        gen_instr("GEQ", BLANK);
        jumpstack.push(instr_idx);
        gen_instr("JUMPZ", BLANK);
    }
    else if (op == "<="){
        gen_instr("LEQ", BLANK);
        jumpstack.push(instr_idx);
        gen_instr("JUMPZ", BLANK);
    }
    else if (op == "=="){
        gen_instr("EQU", BLANK);
        jumpstack.push(instr_idx);
        gen_instr("JUMPZ", BLANK);
    }
    else if (op == "!="){
        gen_instr("NEQ", BLANK);
        jumpstack.push(instr_idx);
        gen_instr("JUMPZ", BLANK);
    }

    return true;
}
// R31
bool Syntax::Relop(std::ifstream &readFile) {
    // <Relop> ::= == | != | > | < | <= | =>
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "==") {
        
        return true;
    } else if (lexeme == "!=") {
        
        return true;
    } else if (lexeme == ">") {
        
        return true;
    } else if (lexeme == "<") {
        
        return true;
    } else if (lexeme == "<=") {
        
        return true;
    } else if (lexeme == "=>") {
        
        return true;
    } else {
        
        return false;
    }
}
// R32
bool Syntax::Expression(std::ifstream &readFile) {
    // <Expression> ::= <Term> <Expression*>
    std::cout << "<Expression> ::= <Term> <Expression*>\n";
    Term(readFile);
    ExpressionPrime(readFile);
    
    return true;
}
// R33
bool Syntax::ExpressionPrime(std::ifstream &readFile) {
    // <Expression  Prime> ::= + <Term> <Expression*>| - <Term> <Expression*>| <Empty>
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "+") {
        check_TypeMatch(prevLexeme, lexeme, readFile);
        Term(readFile);
        gen_instr("ADD", BLANK);
        ExpressionPrime(readFile);
        return true;
    } else if (lexeme == "-") {
        check_TypeMatch(prevLexeme, lexeme, readFile);
        Term(readFile);
        gen_instr("SUB", BLANK);
        ExpressionPrime(readFile);
        return true;
    } else {
        Empty(readFile);
        return true;
    }
}
// R34
bool Syntax::Term(std::ifstream &readFile) {
    // <Term> ::= <Factor> <Term*>
    std::cout << "<Term> ::= <Factor> <Term*>\n";
    Factor(readFile);
    TermPrime(readFile);
    return true;
}
// R35
bool Syntax::TermPrime(std::ifstream &readFile) {
    // <Term Prime> ::= * <Factor> <Term*> | / <Factor> <Term*> | <Empty>
    std::cout << "<Term Prime> ::= * <Factor> <Term*> | / <Factor> <Term*> | <Empty>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        if (lexeme != "-" && lexeme != "+") {
            std::cout << SetTokLex(readFile) << "\n";
        }
    }
    if (lexeme == "*") {
        check_TypeMatch(prevLexeme, lexeme, readFile);
        Factor(readFile);
        gen_instr("MUL", BLANK);
        TermPrime(readFile);
        return true;
    } else if (lexeme == "/") {
        check_TypeMatch(prevLexeme, lexeme, readFile);
        Factor(readFile);
        gen_instr("DIV", BLANK);
        TermPrime(readFile);
        return true;
    } else {
        Empty(readFile);
        return true;
    }
}
// R36
bool Syntax::Factor(std::ifstream &readFile) {
    // <Factor> ::= - <Primary> | <Primary>
    std::cout << "<Factor> ::= - <Primary> | <Primary>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "-") {
        gen_instr("SUB", BLANK);
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        Primary(readFile);
        return true;
    } else {
        Primary(readFile);
        
        return true;
    }
}
// R37
bool Syntax::Primary(std::ifstream &readFile) {
    // <Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
    if (token == "Identifier") {
    
        int addr = get_address(lexeme);
        gen_instr("PUSHM", addr);

        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme == "(") {
            IDs(readFile);
            if (emptyUsed > 0) {
                emptyUsed -= 1;
            } else {
                std::cout << SetTokLex(readFile) << "\n";
            }
            if (lexeme == ")") {
                return true;
            } else {
                std::cout << "ERROR: Line# " << lineCountString << "  expected lexeme \")\". Received " << lexeme << "\n";
                exit(0);
            }
        }
        emptyUsed += 1;
        return true;
    } else if (token == "Integer") {

        int a = 0;
        if (prevLexeme == "-")
		{
			prevLexeme = prevLexeme + lexeme;
			a = std::stoi(prevLexeme);
		}
		else {
			a = std::stoi(lexeme);
		}
        gen_instr("PUSHI", a);
        
        return true;
    } else if (lexeme == "(") {
        Expression(readFile);
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        if (lexeme == ")") {
            
            return true;
        } else {
            std::cout << "ERROR: Line# " << lineCountString << "  expected \")\". Received " << lexeme << "\n";
        }
    } else if (token == "Real") {
        return true;
    } else if (lexeme == "true") {

        gen_instr("PUSHI", 1);

        return true;
    } else if (lexeme == "false") {

        gen_instr("PUSHI", 0);

        return true;
    } 
    return false;
}
// R38
bool Syntax::Empty(std::ifstream &readFile) {
    if (textOn) {
        std::cout << "<Empty> ::= Ɛ\n";
    }
    emptyUsed += 1;
    return true;
}