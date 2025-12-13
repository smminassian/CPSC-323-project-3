// /* R1. <Rat25F> ::= <Opt Function Definitions> # <Opt Declaration List> <Statement List> 
// R2. <Opt Function Definitions> ::= <Function Definitions> | <Empty>
// R3. <Function Definitions> ::= <Function> | <Function> <Function Definitions>
// R4. <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
// R5. <Opt Parameter List> ::= <Parameter List> | <Empty>
// R6. <Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>
// R7. <Parameter> ::= <IDs > <Qualifier>
// R8. <Qualifier> ::= integer | boolean | real
// R9. <Body> ::= { < Statement List> }
// R10. <Opt Declaration List> ::= <Declaration List> | <Empty>
// R11. <Declaration List> := <Declaration> ; | <Declaration> ; <Declaration List>
// R12. <Declaration> ::= <Qualifier > <IDs>
// R13. <IDs> ::= <Identifier> | <Identifier>, <IDs>
// R14. <Statement List> ::= <Statement> | <Statement> <Statement List>
// R15. <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
// R16. <Compound> ::= { <Statement List> }
// R17. <Assign> ::= <Identifier> = <Expression> ;
// R18. <If> ::= if ( <Condition> ) <Statement> fi |
//               else if ( <Condition> ) <Statement> else <Statement> fi
// R19. <Return> ::= return return Prime ; 
//      <Return Prime> ::= <Expression> | ε
// R21. <Print> ::= put ( <Expression>);
// R21. <Scan> ::= get ( <IDs> );
// R22. <While> ::= while ( <Condition> ) <Statement>
// R23. <Condition> ::= <Expression> <Relop> <Expression>
// R24. <Relop> ::= == | != | > | < | <= | =>
// R25. <Expression> ::= <Term> | Term + Term ExpressionPrime | Term - Term ExpressionPrime
//      <ExpressionPrime> ::= + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | ε
// R26. <Term> ::= <Factor> * <Factor> TermPrime | <Factor> / <Factor> TermPrime | <Factor>
// R26. <TermPrime> ::= * <Factor> <TermPrime> | / <Factor> <TermPrime> | ε
// R27. <Factor> ::= - <Primary> | <Primary>
// R28. <Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
// R29. <Empty> ::= ε
// */

// #include "Lexical_Analyzer.h"
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>
// #include <map>
// using namespace std;

// extern Token globalToken;
// int indexPos = 0;

// map<string, int> symbolTable;
// int nextMemory = 10000;

// int labelCount = 0;
// ofstream outFile;

// void Rat25F();
// void OptDeclarationList();
// void DeclarationList();
// void Declaration();
// void IDs();
// void StatementList();
// void Statement();
// void Assign();
// void PrintStmt();
// void ScanStmt();
// void WhileStmt();
// void IfStmt();
// void ReturnStmt();
// void Condition();
// void Expression();
// void ExpressionPrime();
// void Term();
// void TermPrime();
// void Factor();
// void Primary();
// void Relop();

// string currentLexeme() {
//     if (indexPos < (int)globalToken.lexeme.size()) return globalToken.lexeme[indexPos];
//     return "";
// }

// string currentToken() {
//     if (indexPos < (int)globalToken.token.size()) return globalToken.token[indexPos];
//     return "";
// }

// bool atEnd() {
//     return indexPos >= (int)globalToken.lexeme.size();
// }

// void skipComments() {
//     while (!atEnd() && currentLexeme()[0] == '"') {
//         indexPos++;
//         while (!atEnd() && currentLexeme()[0] != '"') indexPos++;
//         if (!atEnd()) indexPos++;
//     }
// }

// void syntaxError(const string &msg) {
//     string lex = atEnd() ? "EOF" : currentLexeme();
//     cerr << "Syntax Error: " << msg << " at token '" << lex << "' (index " << indexPos << ")" << endl;
//     exit(1);
// }

// void Match(const string &expected) {
//     skipComments();
//     if (atEnd()) syntaxError("Expected '" + expected + "' but found EOF");
//     string lex = currentLexeme();
//     if (lex == expected) indexPos++;
//     else syntaxError("Expected '" + expected + "' but found '" + lex + "'");
// }

// void addVariable(const string &id) {
//     if (symbolTable.find(id) == symbolTable.end()) {
//         symbolTable[id] = nextMemory++;
//     }
// }

// string newLabel() {
//     return "L" + to_string(labelCount++);
// }

// void emit(const string &instr) {
//     cout << instr << endl;
//     if (outFile.is_open()) outFile << instr << endl;
// }

// void Rat25F() {
//     outFile.open("output.txt");
//     if (!outFile) {
//         cerr << "Error opening output.txt" << endl;
//         exit(1);
//     }

//     while (!atEnd() && currentLexeme() != "#") indexPos++;

//     Match("#");

//     OptDeclarationList();
//     StatementList();

//     if (currentLexeme() == "#") Match("#");

//     cout << "\nSymbol Table:\nIdentifier\tMemory Location\n";
//     for (auto &pair : symbolTable)
//         cout << pair.first << "\t" << pair.second << endl;

//     outFile.close();
// }

// void OptDeclarationList() {
//     skipComments();
//     string lex = currentLexeme();
//     if (lex == "integer" || lex == "real" || lex == "boolean") DeclarationList();
// }

// void DeclarationList() {
//     Declaration();
//     Match(";");
//     while (true) {
//         skipComments();
//         string lex = currentLexeme();
//         if (lex != "integer" && lex != "real" && lex != "boolean") break;
//         Declaration();
//         Match(";");
//     }
// }

// void Declaration() {
//     string type = currentLexeme();
//     Match(type);
//     IDs();
// }

// void IDs() {
//     addVariable(currentLexeme());
//     string id = currentLexeme();
//     Match(id);
//     while (currentLexeme() == ",") {
//         Match(",");
//         addVariable(currentLexeme());
//         Match(currentLexeme());
//     }
// }

// void StatementList() {
//     while (!atEnd()) {
//         skipComments();
//         string lex = currentLexeme();
//         if (lex == "#" || lex == "}") break;
//         Statement();
//     }
// }

// void Statement() {
//     skipComments();
//     string lex = currentLexeme();
//     if (isIdentifierLexeme(lex)) Assign();
//     else if (lex == "put") PrintStmt();
//     else if (lex == "get") ScanStmt();
//     else if (lex == "while") WhileStmt();
//     else if (lex == "if") IfStmt();
//     else if (lex == "return") ReturnStmt();
//     else syntaxError("Unexpected statement");
// }

// void Assign() {
//     string id = currentLexeme();
//     int mem = symbolTable[id];
//     Match(id);
//     Match("=");
//     Expression();
//     Match(";");
//     emit("POPM " + to_string(mem));
// }

// void PrintStmt() {
//     Match("put");
//     Match("(");
//     Expression();
//     Match(")");
//     Match(";");
//     emit("STDOUT");
// }

// void ScanStmt() {
//     Match("get");
//     Match("(");
//     string id = currentLexeme();
//     int mem = symbolTable[id];
//     IDs();
//     Match(")");
//     Match(";");
//     emit("STDIN");
//     emit("POPM " + to_string(mem));
// }

// void WhileStmt() {
//     Match("while");
//     Match("(");
//     string startLabel = newLabel();
//     string endLabel = newLabel();
//     emit("LABEL " + startLabel);
//     Condition();
//     emit("JUMPZ " + endLabel);
//     Match(")");
//     Statement();
//     emit("JUMP " + startLabel);
//     emit("LABEL " + endLabel);
// }

// void IfStmt() {
//     Match("if");
//     Match("(");
//     Condition();
//     string elseLabel = newLabel();
//     string endLabel = newLabel();
//     emit("JUMPZ " + elseLabel);
//     Match(")");
//     Statement();
//     if (currentLexeme() == "else") {
//         Match("else");
//         emit("JUMP " + endLabel);
//         emit("LABEL " + elseLabel);
//         Statement();
//         emit("LABEL " + endLabel);
//     } else emit("LABEL " + elseLabel);
//     Match("fi");
// }

// void ReturnStmt() {
//     Match("return");
//     if (currentLexeme() != ";") Expression();
//     Match(";");
// }

// void Condition() {
//     Expression();
//     string op = currentLexeme();
//     Relop();
//     Expression();
//     if (op == "==") emit("EQU");
//     else if (op == "!=") emit("NEQ");
//     else if (op == "<") emit("LES");
//     else if (op == ">") emit("GRT");
//     else if (op == "<=") emit("LEQ");
//     else if (op == ">=") emit("GEQ");
// }

// void Expression() {
//     Term();
//     ExpressionPrime();
// }

// void ExpressionPrime() {
//     string lex = currentLexeme();
//     if (lex == "+" || lex == "-") {
//         Match(lex);
//         Term();
//         if (lex == "+") emit("ADD");
//         else emit("SUB");
//         ExpressionPrime();
//     }
// }

// void Term() {
//     Factor();
//     TermPrime();
// }

// void TermPrime() {
//     string lex = currentLexeme();
//     if (lex == "*" || lex == "/") {
//         Match(lex);
//         Factor();
//         if (lex == "*") emit("MUL");
//         else emit("DIV");
//         TermPrime();
//     }
// }

// void Factor() {
//     if (currentLexeme() == "-") {
//         Match("-");
//         Primary();
//         emit("PUSHI -1");
//         emit("MUL");
//     } else Primary();
// }

// void Primary() {
//     string lex = currentLexeme();
//     if (isIdentifierLexeme(lex)) {
//         int mem = symbolTable[lex];
//         Match(lex);
//         emit("PUSHM " + to_string(mem));
//     } else if (isNumberLexeme(lex)) {
//         emit("PUSHI " + lex);
//         Match(lex);
//     } else if (lex == "(") {
//         Match("(");
//         Expression();
//         Match(")");
//     } else if (lex == "true") {
//         emit("PUSHI 1");
//         Match("true");
//     } else if (lex == "false") {
//         emit("PUSHI 0");
//         Match("false");
//     } else syntaxError("Unexpected primary");
// }

// void Relop() {
//     string lex = currentLexeme();
//     if (lex=="==" || lex=="!=" || lex=="<" || lex==">" || lex=="<=" || lex==">=") Match(lex);
//     else syntaxError("Expected relational operator");
// }

// int main() {
//     ifstream myFile("Rat25f.txt");
//     if (!myFile) {
//         cerr << "Error opening input file" << endl;
//         return 1;
//     }

//     globalToken = lexer(myFile);
//     myFile.close();

//     Rat25F();

//     cout << "\nAssembly code has been written to output.txt and printed above.\n";
//     return 0;
// }
/* R1. <Rat25F> ::= <Opt Function Definitions> # <Opt Declaration List> <Statement List> 
R2. <Opt Function Definitions> ::= <Function Definitions> | <Empty>
R3. <Function Definitions> ::= <Function> | <Function> <Function Definitions>
R4. <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
R5. <Opt Parameter List> ::= <Parameter List> | <Empty>
R6. <Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>
R7. <Parameter> ::= <IDs > <Qualifier>
R8. <Qualifier> ::= integer | boolean | real
R9. <Body> ::= { < Statement List> }
R10. <Opt Declaration List> ::= <Declaration List> | <Empty>
R11. <Declaration List> := <Declaration> ; | <Declaration> ; <Declaration List>
R12. <Declaration> ::= <Qualifier > <IDs>
R13. <IDs> ::= <Identifier> | <Identifier>, <IDs>
R14. <Statement List> ::= <Statement> | <Statement> <Statement List>
R15. <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
R16. <Compound> ::= { <Statement List> }
R17. <Assign> ::= <Identifier> = <Expression> ;
R18. <If> ::= if ( <Condition> ) <Statement> fi |
              else if ( <Condition> ) <Statement> else <Statement> fi
R19. <Return> ::= return return Prime ; 
     <Return Prime> ::= <Expression> | ε
R21. <Print> ::= put ( <Expression>);
R21. <Scan> ::= get ( <IDs> );
R22. <While> ::= while ( <Condition> ) <Statement>
R23. <Condition> ::= <Expression> <Relop> <Expression>
R24. <Relop> ::= == | != | > | < | <= | =>
R25. <Expression> ::= <Term> | Term + Term ExpressionPrime | Term - Term ExpressionPrime
     <ExpressionPrime> ::= + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | ε
R26. <Term> ::= <Factor> * <Factor> TermPrime | <Factor> / <Factor> TermPrime | <Factor>
R26. <TermPrime> ::= * <Factor> <TermPrime> | / <Factor> <TermPrime> | ε
R27. <Factor> ::= - <Primary> | <Primary>
R28. <Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
R29. <Empty> ::= ε
*/

#include "Lexical_Analyzer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

extern Token globalToken;
int indexPos = 0;
bool printSwitch = false;

/* ================= SYMBOL TABLE ================= */

struct Symbol {
    int memory;
    string type;
};

vector<string> symbolNames;
vector<Symbol> symbolTable;
int Memory_address = 10000;

/* ================= ASSEMBLY ================= */

vector<string> assembly;
int instrAddress = 1;

void emit(const string &instr) {
    assembly.push_back(to_string(instrAddress++) + " " + instr);
}

/* ================= HELPERS ================= */

string currentLexeme() {
    if (indexPos < (int)globalToken.lexeme.size())
        return globalToken.lexeme[indexPos];
    return "";
}

bool atEnd() {
    return indexPos >= (int)globalToken.lexeme.size();
}

void SkipComments() {
    while (!atEnd() && currentLexeme()[0] == '"')
        indexPos++;
}

void syntaxError(const string &msg) {
    string lex = atEnd() ? "EOF" : currentLexeme();
    cerr << "Syntax Error: " << msg
         << " at token '" << lex
         << "' (index " << indexPos << ")" << endl;
    exit(1);
}

void Match(const string &expected) {
    SkipComments();
    if (atEnd())
        syntaxError("Expected '" + expected + "' but found EOF");

    if (currentLexeme() == expected) {
        if (printSwitch)
            cout << "Token: " << expected << "   (matched '" << expected << "')" << endl;
        indexPos++;
        SkipComments();
    } else {
        syntaxError("Expected '" + expected + "' but found '" + currentLexeme() + "'");
    }
}

/* ================= SYMBOL TABLE OPS ================= */

int findSymbol(const string &id) {
    for (int i = 0; i < (int)symbolNames.size(); i++)
        if (symbolNames[i] == id)
            return i;
    return -1;
}

void insertSymbol(const string &id, const string &type) {
    if (findSymbol(id) != -1) {
        cerr << "Semantic Error: redeclaration of '" << id << "'" << endl;
        exit(1);
    }
    Symbol s;
    s.memory = Memory_address++;
    s.type = type;
    symbolNames.push_back(id);
    symbolTable.push_back(s);
}

Symbol getSymbol(const string &id) {
    int idx = findSymbol(id);
    if (idx == -1) {
        cerr << "Semantic Error: '" << id << "' used before declaration" << endl;
        exit(1);
    }
    return symbolTable[idx];
}

/* ================= GRAMMAR PROTOTYPES ================= */

void Rat25F();
void OptDeclarationList();
void DeclarationList();
void Declaration();
void IDs();
void StatementList();
void Statement();
void CompoundStmt();
void Assign();
void PrintStmt();
void ScanStmt();
void WhileStmt();
void IfStmt();
void Condition();
void Expression();
void ExpressionPrime();
void Term();
void TermPrime();
void Factor();
void Primary();
void Relop();

/* ================= PARSER ================= */

void Rat25F() {
    if (printSwitch)
        cout << "<Rat25F> -> # <Opt Declaration List> <Statement List> #" << endl;

    Match("#");
    OptDeclarationList();
    StatementList();
    Match("#");

    cout << "\nAssembly Code\n";
    for (string s : assembly)
        cout << s << endl;

    cout << "\nSymbol Table\n";
    cout << "Identifier\tMemory\tType\n";
    for (int i = 0; i < (int)symbolNames.size(); i++)
        cout << symbolNames[i] << "\t\t"
             << symbolTable[i].memory << "\t"
             << symbolTable[i].type << endl;
}

void OptDeclarationList() {
    if (currentLexeme() == "integer" || currentLexeme() == "boolean")
        DeclarationList();
}

string currentType;

void DeclarationList() {
    Declaration();
    Match(";");
    while (currentLexeme() == "integer" || currentLexeme() == "boolean") {
        Declaration();
        Match(";");
    }
}

void Declaration() {
    currentType = currentLexeme();
    if (currentType != "integer" && currentType != "boolean")
        syntaxError("Invalid type (real not allowed)");
    Match(currentType);
    IDs();
}

void IDs() {
    string id = currentLexeme();
    insertSymbol(id, currentType);
    Match(id);

    while (currentLexeme() == ",") {
        Match(",");
        id = currentLexeme();
        insertSymbol(id, currentType);
        Match(id);
    }
}

void StatementList() {
    while (!atEnd()) {
        string lex = currentLexeme();
        if (lex == "#" || lex == "}")
            break;
        Statement();
    }
}

void Statement() {
    string lex = currentLexeme();

    if (lex == "get")
        ScanStmt();
    else if (lex == "put")
        PrintStmt();
    else if (lex == "while")
        WhileStmt();
    else if (lex == "if")          
        IfStmt();
    else if (lex == "{")
        CompoundStmt();
    else if (isIdentifierLexeme(lex))
        Assign();
    else
        syntaxError("Invalid statement");
}


void CompoundStmt() {
    Match("{");
    StatementList();
    Match("}");
}



void Assign() {
    string id = currentLexeme();
    Symbol s = getSymbol(id);
    Match(id);
    Match("=");
    Expression();
    Match(";");
    emit("POPM " + to_string(s.memory));
}

void PrintStmt() {
    Match("put");
    Match("(");
    Expression();
    Match(")");
    Match(";");
    emit("STDOUT");
}

void ScanStmt() {
    Match("get");
    Match("(");
    string id = currentLexeme();
    Symbol s = getSymbol(id);
    Match(id);
    Match(")");
    Match(";");
    emit("STDIN");
    emit("POPM " + to_string(s.memory));
}

void WhileStmt() {
    Match("while");
    Match("(");
    int start = instrAddress;
    emit("LABEL");
    Condition();
    int jumpz = instrAddress;
    emit("JUMPZ 0");
    Match(")");
    Statement();
    emit("JUMP " + to_string(start));
    assembly[jumpz - 1] = to_string(jumpz) + " JUMPZ " + to_string(instrAddress);
}

void IfStmt() {
    Match("if");
    Match("(");
    Condition();
    Match(")");

    // Parse the "then" statement or block
    Statement();

    // Optional "else" block
    if (currentLexeme() == "else") {
        Match("else");
        Statement();
    }

    // No "fi" needed
}


void Condition() {
    Expression();
    string op = currentLexeme(); 
    Relop();                      
    Expression();

    if (op == "<") emit("LES");
    else if (op == ">") emit("GRT");
    else if (op == "==") emit("EQU");
    else if (op == "!=") emit("NEQ");
    else if (op == "<=") emit("LEQ");
    else if (op == ">=") emit("GEQ");
}


void Relop() {
    if (isRelopLexeme(currentLexeme()))
        Match(currentLexeme());
    else syntaxError("Expected relational operator");
}

void Expression() {
    Term();
    ExpressionPrime();
}

void ExpressionPrime() {
    if (currentLexeme() == "+" || currentLexeme() == "-") {
        string op = currentLexeme();
        Match(op);
        Term();
        if (op == "+") emit("ADD");
        else emit("SUB");
        ExpressionPrime();
    }
}

void Term() {
    Factor();
    TermPrime();
}

void TermPrime() {
    if (currentLexeme() == "*" || currentLexeme() == "/") {
        string op = currentLexeme();
        Match(op);
        Factor();
        if (op == "*") emit("MUL");
        else emit("DIV");
        TermPrime();
    }
}

void Factor() {
    if (currentLexeme() == "-") {
        Match("-");
        Primary();
        emit("PUSHI -1");
        emit("MUL");
    } else Primary();
}

void Primary() {
    string lex = currentLexeme();
    if (isIdentifierLexeme(lex)) {
        Symbol s = getSymbol(lex);
        Match(lex);
        emit("PUSHM " + to_string(s.memory));
    }
    else if (isNumberLexeme(lex)) {
        emit("PUSHI " + lex);
        Match(lex);
    }
    else if (lex == "true") {
        emit("PUSHI 1");
        Match("true");
    }
    else if (lex == "false") {
        emit("PUSHI 0");
        Match("false");
    }
    else if (lex == "(") {
        Match("(");
        Expression();
        Match(")");
    }
    else syntaxError("Invalid primary");
}

/* ================= MAIN ================= */

int main() {
    ifstream myFile("Rat25f3.txt");
    if (!myFile) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    globalToken = lexer(myFile);
    myFile.close();

    Rat25F();
    return 0;
}


