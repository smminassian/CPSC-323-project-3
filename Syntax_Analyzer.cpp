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

int indexPos = 0;
bool printSwitch = true; // toggle verbose printing

// Function prototypes
void Rat25F();
void OptFunctionDefinitions();
void FunctionDefinitions();
void Function();
void OptParameterList();
void ParameterList();
void Parameter();
void Qualifier();
void Body();
void OptDeclarationList();
void DeclarationList();
void Declaration();
void IDs();
void StatementList();
void Statement();
void Compound();
void Assign();
void IfStmt();
void ReturnStmt();
void PrintStmt();
void ScanStmt();
void WhileStmt();
void Condition();
void Relop();
void Expression();
void ExpressionPrime();
void Term();
void TermPrime();
void Factor();
void Primary();

// Helper functions
string currentLexeme() {
    if (indexPos < (int)globalToken.lexeme.size()) return globalToken.lexeme[indexPos];
    return "";
}

bool atEnd() {
    return indexPos >= (int)globalToken.lexeme.size();
}

// Skip comment lexemes ("comment")
void SkipComments() {
    while (!atEnd() && currentLexeme()[0] == '"') {
        if (printSwitch) cout << "Skipping comment: " << currentLexeme() << endl;
        indexPos++;
    }
}

// Syntax error
void syntaxError(const string &msg) {
    string lex = atEnd() ? "EOF" : currentLexeme();
    cerr << "Syntax Error: " << msg << " at token '" << lex << "' (index " << indexPos << ")" << endl;
    exit(1);
}

// Match a specific lexeme
void Match(const string &expected) {
    SkipComments();
    if (atEnd()) syntaxError("Expected '" + expected + "' but found EOF");
    string cur = currentLexeme();
    if (cur == expected) {
        if (printSwitch) cout << "Token: " << cur << "   (matched '" << expected << "')" << endl;
        indexPos++;
        SkipComments();
    } else syntaxError("Expected '" + expected + "' but found '" + cur + "'");
}

// ===== Grammar Rules =====

// <Rat25F> ::= <Opt Function Definitions> # <Opt Declaration List> <Statement List> #
void Rat25F() {
    if (printSwitch) cout << "<Rat25F> -> <Opt Function Definitions> # <Opt Declaration List> <Statement List> #" << endl;

    OptFunctionDefinitions();
    Match("#");
    OptDeclarationList();
    StatementList();
    Match("#");

    if (!atEnd()) {
        cerr << "Warning: extra tokens remain starting at index " << indexPos << endl;
    } else {
        cout << "Parsing completed successfully." << endl;
    }
}

// <Opt Function Definitions> ::= <Function Definitions> | ε
void OptFunctionDefinitions() {
    if (currentLexeme() == "function") {
        if (printSwitch) cout << "<Opt Function Definitions> -> <Function Definitions>" << endl;
        FunctionDefinitions();
    } else if (printSwitch) cout << "<Opt Function Definitions> -> ε" << endl;
}

// <Function Definitions> ::= <Function> (<Function Definitions>)*
void FunctionDefinitions() {
    if (printSwitch) cout << "<Function Definitions> -> <Function> (<Function Definitions>)*" << endl;
    Function();
    while (currentLexeme() == "function") Function();
}

// <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void Function() {
    if (printSwitch) cout << "<Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << endl;
    Match("function");
    if (isIdentifierLexeme(currentLexeme())) Match(currentLexeme());
    else syntaxError("Expected function name (identifier)");
    Match("(");
    OptParameterList();
    Match(")");
    OptDeclarationList();
    Body();
}

// <Opt Parameter List> ::= <Parameter List> | ε
void OptParameterList() {
    if (isIdentifierLexeme(currentLexeme())) {
        if (printSwitch) cout << "<Opt Parameter List> -> <Parameter List>" << endl;
        ParameterList();
    } else if (printSwitch) cout << "<Opt Parameter List> -> ε" << endl;
}

// <Parameter List> ::= <Parameter> (, <Parameter>)*
void ParameterList() {
    if (printSwitch) cout << "<Parameter List> -> <Parameter> (, <Parameter>)*" << endl;
    Parameter();
    while (currentLexeme() == ",") {
        Match(",");
        Parameter();
    }
}

// <Parameter> ::= <IDs> <Qualifier>
void Parameter() {
    if (printSwitch) cout << "<Parameter> -> <IDs> <Qualifier>" << endl;
    IDs();
    Qualifier();
}

// <Qualifier> ::= integer | boolean | real
void Qualifier() {
    if (printSwitch) cout << "<Qualifier> -> integer | boolean | real" << endl;
    string lex = currentLexeme();
    if (lex == "integer" || lex == "boolean" || lex == "real") Match(lex);
    else syntaxError("Expected type qualifier (integer, boolean, or real)");
}

// <Body> ::= { <Statement List> }
void Body() {
    if (printSwitch) cout << "<Body> -> { <Statement List> }" << endl;
    Match("{");
    StatementList();
    Match("}");
}

// <Opt Declaration List> ::= <Declaration List> | ε
void OptDeclarationList() {
    string lex = currentLexeme();
    if (lex == "integer" || lex == "boolean" || lex == "real") {
        if (printSwitch) cout << "<Opt Declaration List> -> <Declaration List>" << endl;
        DeclarationList();
    } else if (printSwitch) cout << "<Opt Declaration List> -> ε" << endl;
}

// <Declaration List> ::= <Declaration> ; (<Declaration> ;)*
void DeclarationList() {
    if (printSwitch) cout << "<Declaration List> -> <Declaration> ; (<Declaration> ;)*" << endl;
    Declaration();
    Match(";");
    while (currentLexeme() == "integer" || currentLexeme() == "boolean" || currentLexeme() == "real") {
        Declaration();
        Match(";");
    }
}

// <Declaration> ::= <Qualifier> <IDs>
void Declaration() {
    if (printSwitch) cout << "<Declaration> -> <Qualifier> <IDs>" << endl;
    Qualifier();
    IDs();
}

// <IDs> ::= <Identifier> (, <Identifier>)*
void IDs() {
    if (printSwitch) cout << "<IDs> -> <Identifier> (, <Identifier>)*" << endl;
    if (isIdentifierLexeme(currentLexeme())) Match(currentLexeme());
    else syntaxError("Expected identifier in <IDs>");
    while (currentLexeme() == ",") {
        Match(",");
        if (isIdentifierLexeme(currentLexeme())) Match(currentLexeme());
        else syntaxError("Expected identifier after ',' in <IDs>");
    }
}

// <Statement List> ::= (<Statement>)*
void StatementList() {
    if (printSwitch) cout << "<Statement List> -> (<Statement>)*" << endl;
    while (!atEnd()) {
        string lex = currentLexeme();
        if (lex == "{" || isIdentifierLexeme(lex) || lex == "if" || lex == "return" || lex == "put" || lex == "get" || lex == "while") {
            Statement();
        } else break;
    }
}

// <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
void Statement() {
    string lex = currentLexeme();
    if (lex == "{") Compound();
    else if (lex == "if") IfStmt();
    else if (lex == "return") ReturnStmt();
    else if (lex == "put") PrintStmt();
    else if (lex == "get") ScanStmt();      
    else if (lex == "while") WhileStmt();
    else if (isIdentifierLexeme(lex)) Assign();
    else syntaxError("Unrecognized start of <Statement>");
}

// <Compound> ::= { <Statement List> }
void Compound() {
    Match("{");
    StatementList();
    Match("}");
}

// <Assign> ::= <Identifier> = <Expression> ;
void Assign() {
    Match(currentLexeme());
    Match("=");
    Expression();
    Match(";");
}

// <If> ::= if ( <Condition> ) <Statement> ( else <Statement> )? fi
void IfStmt() {
    Match("if");
    Match("(");
    Condition();
    Match(")");
    Statement();
    if (currentLexeme() == "else") {
        Match("else");
        Statement();
    }
    Match("fi");
}

// <Return> ::= return ( <Expression> | ε ) ;
void ReturnStmt() {
    Match("return");
    if (currentLexeme() != ";") Expression();
    Match(";");
}

// <Print> ::= put ( <Expression> ) ;
void PrintStmt() {
    Match("put");
    Match("(");
    Expression();
    Match(")");
    Match(";");
}

// <Scan> ::= get ( <IDs> ) ;
void ScanStmt() {
    Match("get");
    Match("(");
    IDs();
    Match(")");
    Match(";");
}

// <While> ::= while ( <Condition> ) <Statement>
void WhileStmt() {
    Match("while");
    Match("(");
    Condition();
    Match(")");
    Statement();
}

// <Condition> ::= <Expression> <Relop> <Expression>
void Condition() {
    Expression();
    Relop();
    Expression();
}

// <Relop> ::= == | != | > | < | <= | >=
void Relop() {
    if (isRelopLexeme(currentLexeme())) Match(currentLexeme());
    else syntaxError("Expected relational operator (==, !=, >, <, <=, >=)");
}

// <Expression> ::= <Term> <ExpressionPrime>
void Expression() {
    Term();
    ExpressionPrime();
}

// <ExpressionPrime> ::= (+|-) <Term> <ExpressionPrime> | ε
void ExpressionPrime() {
    string lex = currentLexeme();
    if (lex == "+" || lex == "-") {
        Match(lex);
        Term();
        ExpressionPrime();
    }
}

// <Term> ::= <Factor> <TermPrime>
void Term() {
    Factor();
    TermPrime();
}

// <TermPrime> ::= (*|/) <Factor> <TermPrime> | ε
void TermPrime() {
    string lex = currentLexeme();
    if (lex == "*" || lex == "/") {
        Match(lex);
        Factor();
        TermPrime();
    }
}

// <Factor> ::= - <Primary> | <Primary>
void Factor() {
    if (currentLexeme() == "-") {
        Match("-");
        Primary();
    } else Primary();
}

// <Primary> ::= <Identifier> | <Integer> | (<Expression>) | true | false
void Primary() {
    string lex = currentLexeme();
    if (isIdentifierLexeme(lex) || isNumberLexeme(lex) || lex == "true" || lex == "false") {
        Match(lex);
    } else if (lex == "(") {
        Match("(");
        Expression();
        Match(")");
    } else syntaxError("Unexpected primary");
}

// ===== Main =====
int main() {
    ifstream myFile("Rat25f.txt");
    if (!myFile) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    globalToken = lexer(myFile);
    myFile.close();

    Rat25F();

    return 0;
}

