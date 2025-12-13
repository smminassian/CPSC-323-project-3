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
#include "Symbol_Table.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

SymbolTable symtab;   // [A3 CHANGE] Global symbol table for Assignment 3

int indexPos = 0;
bool printSwitch = true; // toggle verbose printing

// [A3 CODEGEN] Assembly instruction list (1-based indexing)
vector<string> instructions;
int instructionIndex = 1;

// [A3 CODEGEN] emit helpers
int emitInstruction(const string& instr) {
    instructions.push_back(to_string(instructionIndex) + " " + instr);
    return instructionIndex++;
}

// [A3 CODEGEN] patch operand of a previously emitted instruction (for JUMP/JUMPZ)
void patchInstructionOperand(int instrNumber1Based, int target) {
    if (instrNumber1Based < 1 || instrNumber1Based > (int)instructions.size()) return;
    string& line = instructions[instrNumber1Based - 1];

    // Keep everything up to first space after the line number and mnemonic.
    // Example: "11 JUMPZ 0" -> replace last operand with target
    // We'll replace everything after the last space with target.
    size_t lastSpace = line.find_last_of(' ');
    if (lastSpace == string::npos) return;
    line = line.substr(0, lastSpace + 1) + to_string(target);
}

// [A3 CODEGEN] print assembly listing
void printInstructions() {
    cout << "\nAssembly Code Listing\n";
    for (const auto& s : instructions) cout << s << "\n";
}

// Function prototypes
void Rat25F();
string Qualifier();          // [A3 CHANGE] now returns declared type
void Body();
void OptDeclarationList();
void DeclarationList();
void Declaration();

// [A3 CHANGE] Split IDs into declaration vs usage
void IDsDecl(const string& type);
void IDsUse();

void StatementList();
void Statement();
void Compound();
void Assign();
void PrintStmt();
void ScanStmt();
void WhileStmt();
void Condition();
void Expression();
void ExpressionPrime();
void Term();
void TermPrime();
void Factor();
void Primary();
string Relop();


// Helper functions
string currentLexeme() {
    if (indexPos < (int)globalToken.lexeme.size()) return globalToken.lexeme[indexPos];
    return "";
}

bool atEnd() {
    return indexPos >= (int)globalToken.lexeme.size();
}

// Syntax error
void syntaxError(const string &msg) {
    string lex = atEnd() ? "EOF" : currentLexeme();
    cerr << "Syntax Error: " << msg
         << " at token '" << lex
         << "' (index " << indexPos << ")" << endl;
    exit(1);
}

// Match a specific lexeme
void Match(const string &expected) {
    if (atEnd()) syntaxError("Expected '" + expected + "' but found EOF");
    string cur = currentLexeme();
    if (cur == expected) {
        if (printSwitch)
            cout << "Token: " << cur << "   (matched '" << expected << "')" << endl;
        indexPos++;
    } else syntaxError("Expected '" + expected + "' but found '" + cur + "'");
}

// ===== Grammar Rules =====

// <Rat25F> ::= # <Opt Declaration List> <Statement List> #
// [A3 CHANGE] Simplified Rat25F – no function definitions
void Rat25F() {
    if (printSwitch)
        cout << "<Rat25F> -> # <Opt Declaration List> <Statement List> #" << endl;

    Match("#");
    OptDeclarationList();
    StatementList();
    Match("#");

    if (!atEnd())
        cerr << "Warning: extra tokens remain starting at index "
             << indexPos << endl;
    else
        cout << "Parsing completed successfully." << endl;

    symtab.print();   // [A3 CHANGE] required symbol table listing
    printInstructions(); // [A3 CODEGEN] required assembly listing
}

// <Qualifier> ::= integer | boolean | real
// [A3 CHANGE] real is NOT allowed in Assignment 3
string Qualifier() {
    if (printSwitch)
        cout << "<Qualifier> -> integer | boolean" << endl;

    string lex = currentLexeme();
    if (lex == "integer" || lex == "boolean") {
        Match(lex);
        return lex;
    }
    if (lex == "real")
        syntaxError("Type 'real' is not allowed in Assignment 3");

    syntaxError("Expected type qualifier (integer or boolean)");
    return "";
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
    if (lex == "integer" || lex == "boolean") {
        if (printSwitch)
            cout << "<Opt Declaration List> -> <Declaration List>" << endl;
        DeclarationList();
    } else if (printSwitch)
        cout << "<Opt Declaration List> -> ε" << endl;
}

// <Declaration List> ::= <Declaration> ; (<Declaration> ; )*
void DeclarationList() {
    if (printSwitch)
        cout << "<Declaration List> -> <Declaration> ; (<Declaration> ;)*" << endl;

    Declaration();
    Match(";");

    while (currentLexeme() == "integer" || currentLexeme() == "boolean") {
        Declaration();
        Match(";");
    }
}

// <Declaration> ::= <Qualifier> <IDs>
void Declaration() {
    if (printSwitch)
        cout << "<Declaration> -> <Qualifier> <IDs>" << endl;

    string type = Qualifier();   // [A3 CHANGE]
    IDsDecl(type);               // [A3 CHANGE]
}

// <IDs> ::= <Identifier> (, <Identifier>)*
// [A3 CHANGE] Declaration version: inserts into symbol table
void IDsDecl(const string& type) {
    if (printSwitch)
        cout << "<IDs> -> <Identifier> (, <Identifier>)*" << endl;

    string name = currentLexeme();
    if (IdentifierFSM(name) != "identifier" || checkKeyword(name) == "keyword ")
        syntaxError("Expected identifier in declaration");

    symtab.insert(name, type);
    Match(name);

    while (currentLexeme() == ",") {
        Match(",");
        name = currentLexeme();
        if (IdentifierFSM(name) != "identifier" || checkKeyword(name) == "keyword ")
            syntaxError("Expected identifier after ','");

        symtab.insert(name, type);
        Match(name);
    }
}

// [A3 CHANGE] Usage version: lookup only (no insertion)
void IDsUse() {
    if (printSwitch)
        cout << "<IDs> -> <Identifier> (, <Identifier>)*" << endl;

    string name = currentLexeme();
    if (IdentifierFSM(name) != "identifier" || checkKeyword(name) == "keyword ")
        syntaxError("Expected identifier");

    symtab.lookup(name);
    Match(name);

    while (currentLexeme() == ",") {
        Match(",");
        name = currentLexeme();
        if (IdentifierFSM(name) != "identifier" || checkKeyword(name) == "keyword ")
            syntaxError("Expected identifier after ','");

        symtab.lookup(name);
        Match(name);
    }
}

// <Statement List> ::= (<Statement>)*
void StatementList() {
    if (printSwitch)
        cout << "<Statement List> -> (<Statement>)*" << endl;

    while (!atEnd()) {
        string lex = currentLexeme();
        if (lex == "{" || IdentifierFSM(lex) == "identifier" ||
            lex == "put" || lex == "get" || lex == "while")
            Statement();
        else break;
    }
}

// <Statement> ::= <Compound> | <Assign> | <Print> | <Scan> | <While>
void Statement() {
    string lex = currentLexeme();
    if (lex == "{") Compound();
    else if (lex == "put") PrintStmt();
    else if (lex == "get") ScanStmt();
    else if (lex == "while") WhileStmt();
    else if (IdentifierFSM(lex) == "identifier") Assign();
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
    string lhs = currentLexeme();
    auto sym = symtab.lookup(lhs);    // [A3 CHANGE] use-before-declare check
    Match(lhs);

    Match("=");
    Expression();

    // [A3 CODEGEN] store expression result into variable memory
    emitInstruction("POPM " + to_string(sym.memory));

    Match(";");
}

// <Print> ::= put ( <Expression> ) ;
void PrintStmt() {
    Match("put");
    Match("(");
    Expression();
    Match(")");

    // [A3 CODEGEN] output top-of-stack
    emitInstruction("STDOUT");

    Match(";");
}

// <Scan> ::= get ( <IDs> ) ;
void ScanStmt() {
    Match("get");
    Match("(");

    // [A3 CODEGEN] For Assignment 3 sample, get takes a single identifier.
    // We'll support multiple IDs by reading multiple inputs.
    if (printSwitch)
        cout << "<IDs> -> <Identifier> (, <Identifier>)*" << endl;

    // First id
    string name = currentLexeme();
    if (IdentifierFSM(name) != "identifier" || checkKeyword(name) == "keyword ")
        syntaxError("Expected identifier");
    auto sym = symtab.lookup(name);
    Match(name);

    emitInstruction("STDIN");
    emitInstruction("POPM " + to_string(sym.memory));

    // Additional ids: read more STDIN values
    while (currentLexeme() == ",") {
        Match(",");
        name = currentLexeme();
        if (IdentifierFSM(name) != "identifier" || checkKeyword(name) == "keyword ")
            syntaxError("Expected identifier after ','");
        sym = symtab.lookup(name);
        Match(name);

        emitInstruction("STDIN");
        emitInstruction("POPM " + to_string(sym.memory));
    }

    Match(")");
    Match(";");
}

// <While> ::= while ( <Condition> ) <Statement>
void WhileStmt() {
    // [A3 CODEGEN] while loop structure with backpatching
    int loopStart = emitInstruction("LABEL");  // label at top of loop

    Match("while");
    Match("(");
    Condition();
    Match(")");

    // If condition result is 0 -> jump out (patch later)
    int jzLine = emitInstruction("JUMPZ 0");

    Statement();

    // jump back to start label instruction location
    emitInstruction("JUMP " + to_string(loopStart));

    // patch JUMPZ target to instruction AFTER the loop
    patchInstructionOperand(jzLine, instructionIndex);
}

// <Condition> ::= <Expression> <Relop> <Expression>
void Condition() {
    Expression();
    string op = Relop();
    Expression();

    // [A3 CODEGEN] emit compare AFTER both expressions are on stack
    if (op == "==") emitInstruction("EQU");
    else if (op == "!=") emitInstruction("NEQ");
    else if (op == ">") emitInstruction("GRT");
    else if (op == "<") emitInstruction("LES");
    else if (op == "<=") emitInstruction("LEQ");
    else if (op == ">=") emitInstruction("GEQ");
}


// <Relop> ::= == | != | > | < | <= | >=
string Relop() {
    string op = currentLexeme();
    if (isRelopLexeme(op)) {
        Match(op);
        return op;
    }
    syntaxError("Expected relational operator");
    return "";
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

        // [A3 CODEGEN] emit arithmetic op after rhs is evaluated
        if (lex == "+") emitInstruction("ADD");
        else emitInstruction("SUB");

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

        // [A3 CODEGEN] emit arithmetic op after rhs is evaluated
        if (lex == "*") emitInstruction("MUL");
        else emitInstruction("DIV");

        TermPrime();
    }
}

// <Factor> ::= - <Primary> | <Primary>
void Factor() {
    if (currentLexeme() == "-") {
        Match("-");
        Primary();

        // [A3 CODEGEN] unary minus: multiply by -1
        emitInstruction("PUSHI -1");
        emitInstruction("MUL");
    } else {
        Primary();
    }
}

// <Primary> ::= <Identifier> | <Integer> | (<Expression>) | true | false
void Primary() {
    string lex = currentLexeme();

    if (IdentifierFSM(lex) == "identifier") {
        auto sym = symtab.lookup(lex);    // [A3 CHANGE]
        Match(lex);

        // [A3 CODEGEN] push variable value from memory location
        emitInstruction("PUSHM " + to_string(sym.memory));
    }
    else if (isNumberLexeme(lex) || lex == "true" || lex == "false") {
        if (lex == "true") {
            Match("true");
            emitInstruction("PUSHI 1");
        } else if (lex == "false") {
            Match("false");
            emitInstruction("PUSHI 0");
        } else {
            Match(lex);
            emitInstruction("PUSHI " + lex);
        }
    }
    else if (lex == "(") {
        Match("(");
        Expression();
        Match(")");
    }
    else syntaxError("Unexpected primary");
}

// ===== Main =====
int main(int argc, char** argv) {
    string filename = (argc > 1) ? argv[1] : "Rat25f.txt";

    ifstream myFile(filename);
    if (!myFile) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    globalToken = lexer(myFile);
    myFile.close();

    Rat25F();
    return 0;
}
