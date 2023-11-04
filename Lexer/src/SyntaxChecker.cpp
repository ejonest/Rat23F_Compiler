#include "SyntaxChecker.h"

bool textOn = true;
std::string token;
std::string lexeme;
std::string tempString;
int emptyUsed = 0;
std::string lineCountString = "";

void SyntaxChecker(std::string fileName) {
    std::ifstream readFile;
    readFile.open("output.txt");
    int j = 7;
    std::getline(readFile, tempString);
    if (RAT23F(readFile)) {
        std::cout << "==========================Syntax Correct==========================\n";
    }
    readFile.close();
    exit(0);
}
std::string SetTokLex(std::ifstream &readFile) {
    // std::getline(readFile, tempString);
    std::getline(readFile, tempString);
    int j = 10;
    while (tempString[j] != ' ' && tempString[j] != '\t' && tempString[j] != '\n') {j++;}
    token = tempString.substr(10, j);
    token.erase(remove(token.begin(), token.end(), ' '), token.end());
    // std::cout << "Token[6, " << j << "]: " << token <<";\n";
    lexeme = tempString.substr(55);
    lexeme.erase(remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
    lineCountString = tempString.substr(0, 2);
    // lineCountString.erase(remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
    // std::cout << "Lexeme[52, ]: " << lexeme <<"\n";
    // std::cout << "can you see this far?\n";
    return "\n" + tempString.substr(3);
}

bool RAT23F(std::ifstream &readFile) {
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
bool OptFuncDef(std::ifstream &readFile) {
    // <Opt Function Definitions> ::= <Function Definitions> | <Empty>
    std::cout << "<Opt Function Definitions> ::= <Function Definitions> | <Empty>\n";
    if(!FuncDefs(readFile)) {
        Empty(readFile);
    }
    return true;
}
bool FuncDefs(std::ifstream &readFile) {
    // <Function Definitions> ::= <Function> <Function Definitions*>
    std::cout << "<Function Definitions> ::= <Function> <Function Definitions*>\n";
    // Func(readFile);
    // FuncDefsPrim(readFile);
    // return true;
    if (Func(readFile)) {
        FuncDefsPrim(readFile);
    } else {
        Empty(readFile);
    }
    return true;
}
bool FuncDefsPrim(std::ifstream &readFile) {
    // <Function Definitions Prime> ::= <Function> <Function Definitions*> | <empty>
    std::cout << "<Function Definitions Prime> ::= <Function> <Function Definitions*> | <empty>\n";
    if (Func(readFile)) {
        FuncDefsPrim(readFile);
    } else {
        Empty(readFile);
    }
    return true;
}
bool Func(std::ifstream &readFile) {
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
bool OptParmList(std::ifstream &readFile) {
    // <Opt Parameter List> ::= <Parameter List> | <Empty>
    std::cout << "<Opt Parameter List> ::= <Parameter List> | <Empty>\n";
    if (!ParmList(readFile)) {
        Empty(readFile);
    }
    return true;
}
bool ParmList(std::ifstream &readFile) {
    // <Parameter List> ::= <Parameter> <Parameter List*> 
    std::cout << "<Parameter List> ::= <Parameter> <Parameter List*>\n";
    Parameter(readFile);
    ParmListPrim(readFile); 
    return true;  
}
bool ParmListPrim(std::ifstream &readFile) {
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
bool Parameter(std::ifstream &readFile) {
    // <Parameter> ::= <IDs> <Qualifier>
    std::cout << "<Parameter> ::= <IDs> <Qualifier>\n";
    IDs(readFile);
    Qualifier(readFile);
    return true;
}
bool Qualifier(std::ifstream &readFile) {
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
bool Body(std::ifstream &readFile) {
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
bool OptDecList(std::ifstream &readFile) {
    // <Opt Declaration List> ::= <Declaration List> | <Empty>
    std::cout << "<Opt Declaration List> ::= <Declaration List> | <Empty>\n";
    if (!DecList(readFile)) {
        Empty(readFile);
    }
    return true;
}
bool DecList(std::ifstream &readFile) {
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
bool DecListPrim(std::ifstream &readFile) {
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
bool Declaration(std::ifstream &readFile) {
    // <Declaration> ::= integer <IDs>| bool <IDs>| real <IDs>
    std::cout << "<Declaration> ::= integer <IDs>| bool <IDs>| real <IDs>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "integer") {
        IDs(readFile);
        
        return true;
    } else if (lexeme == "bool") {
        IDs(readFile);
        
        return true;
    } else if (lexeme == "real") {
        IDs(readFile);
        
        return true;
    } else {
        // std::cout << "ERROR: Line# " << lineCountString << "  expected \"integer\", \"bool\", \"real\". Received " << lexeme << "\n";
        // exit(0);
        
        emptyUsed += 1;
        return false;
    }
}
bool IDs(std::ifstream &readFile) {
    // <IDs> ::= <Identifier> <IDs*>
    std::cout << "<IDs> ::= <Identifier> <IDs*>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (token == "Identifier") {
        IDsPrime(readFile);
        
        return true;
    } else {
        std::cout << "ERROR: Line# " << lineCountString << "  expected token \"Identifier\". Received " << token << "\n";
        exit(0);
        // return false;
    }
}
bool IDsPrime(std::ifstream &readFile) {
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
bool StateList(std::ifstream &readFile) {
    // <Statement List> ::= <Statement>  <Statement List*>
    std::cout << "<Statement List> ::= <Statement>  <Statement List*>\n";
    State(readFile);
    StateListPrime(readFile);
    return true;
}
bool StateListPrime(std::ifstream &readFile) {
    // <Statement List Prime> ::= <Statement>  <Statement List*> | <empty>
    std::cout << "<Statement List Prime> ::= <Statement>  <Statement List*> | <empty>\n";
    if(State(readFile)) {
        StateListPrime(readFile);
    } else {
        Empty(readFile);
    }
    
    return true;
}
bool State(std::ifstream &readFile) {
    // <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
    std::cout << "<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (!Compound(readFile) && !Assign(readFile) && !If(readFile) && !Ret(readFile) && !Print(readFile) && !Scan(readFile) && !Whi(readFile)) {
        
        return false;
    } else {
        
        return true;
    }
}
bool Compound(std::ifstream &readFile) {
    // <Compound> ::= { <Statement List> }
    std::cout << "<Compound> ::= { <Statement List> }\n";
    if (lexeme == "{") {
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
bool Assign(std::ifstream &readFile) {
    // <Assign> ::= <Identifier> = <Expression> ;
    std::cout << "<Assign> ::= <Identifier> = <Expression> ;\n";
    if (token == "Identifier") {
        // if (emptyUsed > 0) {
        //     std::cout << "====Current emptyUsed = " << emptyUsed << "\n";
        //     emptyUsed -= 1;
        // } else {
        //     std::cout << SetTokLex(readFile) << "\n";
        // }
        std::cout << SetTokLex(readFile) << "\n";
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        }
        if (lexeme == "=") {
            Expression(readFile);
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
bool If(std::ifstream &readFile) {
    // <If> ::= if ( <Condition> ) <Statement> <If*>
    std::cout << "<If> ::= if ( <Condition> ) <Statement> <If*>\n";
    if (lexeme == "if") {
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
            if (lexeme == ")") {
                State(readFile);
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
        
        return false;
    }
}
bool IfPrim(std::ifstream &readFile) {
    // <If Prime> ::= endif | else <Statement> endif
    std::cout << "<If Prime> ::= endif | else <Statement> endif\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "endif") {
        
        return true;
    } else if (lexeme == "else") {
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
        
        return false;
    }
}
bool Ret(std::ifstream &readFile) {
    // <Return> ::= ret <Return*>
    std::cout << "<Return> ::= ret <Return*>\n";
    if (lexeme == "ret") {
        RetPrime(readFile);
        
        return true;
    } else {
        
        return false;
    }
}
bool RetPrime(std::ifstream &readFile) {
    // <Return*> ::= ; | <Expression> ;
    std::cout << "<Return*> ::= ; | <Expression> ;\n";
    if (lexeme == ";") {
        if (emptyUsed > 0) {
            emptyUsed -= 1;
        } else {
            std::cout << SetTokLex(readFile) << "\n";
        }
        
        return true;
    } else if (Expression(readFile)) {
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
        
        return false;
    }
}
bool Print(std::ifstream &readFile) {
    // <Print> ::= put ( <Expression>);
    std::cout << "<Print> ::= put ( <Expression> );\n";
    if (lexeme == "put") {
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
                if (emptyUsed > 0) {
                    emptyUsed -= 1;
                } else {
                    std::cout << SetTokLex(readFile) << "\n";
                }
                if (lexeme == ";") {
                    
                    return true;
                } else {
                    
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
    
    return false;
}
bool Scan(std::ifstream &readFile) {
    // <Scan> ::= get ( <IDs> );
    std::cout << "<Scan> ::= get ( <IDs> );\n";
    if (lexeme == "get") {
        emptyUsed -= 1;
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
        
        return false;
    }
}
bool Whi(std::ifstream &readFile) {
    // <While> ::= while ( <Condition> ) <Statement>
    if (lexeme == "while") {
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
        
        return false;
    }
}
bool Condition(std::ifstream &readFile) {
    // <Condition> ::= <Expression> <Relop> <Expression>
    std::cout << "<Condition> ::= <Expression> <Relop> <Expression>\n";
    Expression(readFile);
    Relop(readFile);
    Expression(readFile);
    
    return true;
}
bool Relop(std::ifstream &readFile) {
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
bool Expression(std::ifstream &readFile) {
    // <Expression> ::= <Term> <Expression*>
    std::cout << "<Expression> ::= <Term> <Expression*>\n";
    Term(readFile);
    ExpressionPrime(readFile);
    
    return true;
}
bool ExpressionPrime(std::ifstream &readFile) {
    // <Expression  Prime> ::= + <Term> <Expression*>| - <Term> <Expression*>| <Empty>
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "+") {
        Term(readFile);
        ExpressionPrime(readFile);
        
        return true;
    } else if (lexeme == "-") {
        Term(readFile);
        ExpressionPrime(readFile);
        
        return true;
    } else {
        Empty(readFile);
        
        return true;
    }
}
bool Term(std::ifstream &readFile) {
    // <Term> ::= <Factor> <Term*>
    std::cout << "<Term> ::= <Factor> <Term*>\n";
    Factor(readFile);
    TermPrime(readFile);
    
    return true;
}
bool TermPrime(std::ifstream &readFile) {
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
        Factor(readFile);
        TermPrime(readFile);
        
        return true;
    } else if (lexeme == "/") {
        Factor(readFile);
        TermPrime(readFile);
        
        return true;
    } else {
        Empty(readFile);
        
        return true;
    }
}
bool Factor(std::ifstream &readFile) {
    // <Factor> ::= - <Primary> | <Primary>
    std::cout << "<Factor> ::= - <Primary> | <Primary>\n";
    if (emptyUsed > 0) {
        emptyUsed -= 1;
    } else {
        std::cout << SetTokLex(readFile) << "\n";
    }
    if (lexeme == "-") {
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
bool Primary(std::ifstream &readFile) {
    // <Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
    if (token == "Identifier") {
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
            // std::cout << "this may be the probelm area\n";
            std::cout << "ERROR: Line# " << lineCountString << "  expected \")\". Received " << lexeme << "\n";
        }
    } else if (token == "Real") {
        
        return true;
    } else if (lexeme == "true") {
        
        return true;
    } else if (lexeme == "false") {
        
        return true;
    } 
    
    return false;
    
}
bool Empty(std::ifstream &readFile) {
    if (textOn) {
        std::cout << "<Empty> ::= Ɛ\n";
    }
    emptyUsed += 1;
    
    return true;
}