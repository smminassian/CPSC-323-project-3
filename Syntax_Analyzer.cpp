#include "Lexical_Analyzer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

extern Token globalToken;
int indexPos = 0;
bool printSwitch = false;



struct Symbol {
    int memory;
    string type;
};

vector<string> symbolNames;
vector<Symbol> symbolTable;
int Memory_address = 10000;



vector<string> assembly;
int instrAddress = 1;




// Function prototypes
void Rat25F();
void OptDeclarationList();
void DeclarationList();
void Declaration();
void IDs();
void StatementList();
void Statement();
void Assign();
void IfStmt();
void CompoundStmt();
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
string currentLexeme()
{
    if (indexPos < (int)globalToken.lexeme.size())
        return globalToken.lexeme[indexPos];
    return "";
}

int emitInstruction(const string &instr) //PushM add
{
    assembly.push_back(to_string(instrAddress) + " " + instr);
    return instrAddress++;
}

bool atEnd()
{
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
    //emitInstruction("POPM " + s.address);
    Match(";");
    emitInstruction("POPM " + to_string(s.memory));
}

void PrintStmt() {
    Match("put");
    Match("(");
    Expression();
    Match(")");
    Match(";");
    emitInstruction("STDOUT");
}

void ScanStmt() {
    Match("get");
    Match("(");
    string id = currentLexeme();
    Symbol s = getSymbol(id);
    Match(id);
    Match(")");
    Match(";");
    emitInstruction("STDIN");
    emitInstruction("POPM " + to_string(s.memory));
}

void WhileStmt() {
    Match("while");
    Match("(");
    int start = instrAddress;
    emitInstruction("LABEL");
    Condition();
    int jumpz = instrAddress;
    emitInstruction("JUMPZ 0");
    Match(")");
    Statement();
    emitInstruction("JUMP " + to_string(start));
    assembly[jumpz - 1] = to_string(jumpz) + " JUMPZ " + to_string(instrAddress);
}

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

}


void Condition() {
    Expression();
    string op = currentLexeme(); 
    Relop();                      
    Expression();

    if (op == "<") emitInstruction("LES");
    else if (op == ">") emitInstruction("GRT");
    else if (op == "==") emitInstruction("EQU");
    else if (op == "!=") emitInstruction("NEQ");
    else if (op == "<=") emitInstruction("LEQ");
    else if (op == ">=") emitInstruction("GEQ");
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
        if (op == "+") emitInstruction("ADD");
        else emitInstruction("SUB");
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
        if (op == "*") emitInstruction ("MUL");
        else emitInstruction("DIV");
        TermPrime();
       
    }
}

void Factor() {
    if (currentLexeme() == "-") {
        Match("-");
        Primary(); // x
        emitInstruction("PUSHI -1");
        emitInstruction("MUL");
        //-x 
    } else Primary();

}
void Primary() {
    string lex = currentLexeme();
    if (isIdentifierLexeme(lex)) {
        Symbol s = getSymbol(lex);
        Match(lex);
        emitInstruction("PUSHM " + to_string(s.memory));
    }
    else if (isNumberLexeme(lex)) {
        emitInstruction("PUSHI " + lex);
        Match(lex);
    }
    else if (lex == "true") {
        emitInstruction("PUSHI 1");
        Match("true");
    }
    else if (lex == "false") {
        emitInstruction("PUSHI 0");
        Match("false");
    }
    else if (lex == "(") {
        Match("(");
        Expression();
        Match(")");
    }
    else syntaxError("Invalid primary");
}



int main() {
    vector<string> inputFiles = { "Rat25f1.txt", "Rat25f2.txt", "Rat25f3.txt" };
    vector<string> outputFiles = { "output1.txt", "output2.txt", "output3.txt" };

    if (inputFiles.size() != outputFiles.size()) {
        cerr << "Error: Input and output file lists do not match in size." << endl;
        return 1;
    }

    for (size_t i = 0; i < inputFiles.size(); i++) {
        ifstream myFile(inputFiles[i]);
        if (!myFile) {
            cerr << "Error opening input file: " << inputFiles[i] << endl;
            continue;
        }

        indexPos = 0;
        symbolNames.clear();
        symbolTable.clear();
        Memory_address = 10000;
        assembly.clear();
        instrAddress = 1;

        globalToken = lexer(myFile);
        myFile.close();

        Rat25F();

        ofstream outFile(outputFiles[i]);
        if (!outFile) {
            cerr << "Error opening output file: " << outputFiles[i] << endl;
            continue;
        }

        for (const string &s : assembly)
            outFile << s << endl;

        outFile << "\nSymbol Table\n";
        outFile << "Identifier\tMemory\tType\n";
        for (size_t j = 0; j < symbolNames.size(); j++)
            outFile << symbolNames[j] << "\t\t"
                    << symbolTable[j].memory << "\t"
                    << symbolTable[j].type << endl;
        outFile.close();

        cout << "Processed " << inputFiles[i] << " -> " << outputFiles[i] << endl;
    }

    return 0;
}


