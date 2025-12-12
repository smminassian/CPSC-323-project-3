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
#include <cctype>
#include <vector>
#include <string>
using namespace std;

extern Token globalToken; 
int indexPos = 0;
bool printSwitch = true; 


// function prototypes
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


// helper functions
string currentLexeme() {
    if (indexPos < (int)globalToken.lexeme.size()) return globalToken.lexeme[indexPos];
    return ""; 
}

string currentToken() {
	if (indexPos < (int)globalToken.token.size()) return globalToken.token[indexPos];
	return "";
}

bool atEnd() {
    return indexPos >= (int)globalToken.lexeme.size();
}

void syntaxError(const string &msg) {
    string lex = atEnd() ? "EOF" : currentLexeme();
    cerr << "Syntax Error: " << msg << " at token '" << lex << "' (index " << indexPos << ")" << endl;
    exit(1);
}

void Match(const string &expected) {
    if (atEnd()) {
        syntaxError("Expected '" + expected + "' but found EOF");
    }

    string cur = currentToken();
    string lex = currentLexeme();

    if (cur == expected || lex == expected) {
        if (printSwitch)
            cout << cur << " " << lex << endl;  
        indexPos++;
    } else {
        syntaxError("Expected '" + expected + "' but found '" + cur + "'");
    }
}



// parsing functions


// <Rat25F> ::= <Opt Function Definitions> # <Opt Declaration List> <Statement List>
void Rat25F() {
    if (printSwitch) cout << "<Rat25F> -> <Opt Function Definitions> # <Opt Declaration List> <Statement List>" << endl;
    OptFunctionDefinitions();
    if (currentLexeme() == "#") {
        Match("#");
    } else {
        syntaxError("Missing '#' (expected program divider)");
    }
    OptDeclarationList();
    StatementList();

    if (!atEnd()) {
        cerr << "Warning: extra tokens remain after parsing starting at index " << indexPos << endl;
    } else {
        cout << "Parsing completed successfully." << endl;
    }
}


// <Opt Function Definitions> ::= <Function Definitions> | ε
void OptFunctionDefinitions() {
    if (currentLexeme() == "function") {
        if (printSwitch) cout << "<Opt Function Definitions> -> <Function Definitions>" << endl;
        FunctionDefinitions();
    } else {
        if (printSwitch) cout << "<Opt Function Definitions> -> ε" << endl;
    }
}


// <Function Definitions> ::= <Function> | <Function> <Function Definitions>
void FunctionDefinitions() {
    if (printSwitch) cout << "<Function Definitions> -> <Function> <Function Definitions> | <Function>" << endl;
    Function();
    while (currentLexeme() == "function") {
        Function();
    }
}


// <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void Function() {
    if (printSwitch) cout << "<Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << endl;
    Match("function");
    if (isIdentifierLexeme(currentLexeme())) {
        Match(currentLexeme()); 
    } else {
        syntaxError("Expected function name (identifier)");
    }
    Match("(");
    OptParameterList();
    Match(")");
    OptDeclarationList();
    Body();
}


// parameter rules
void OptParameterList() {
    if (isIdentifierLexeme(currentLexeme())) {
        if (printSwitch) cout << "<Opt Parameter List> -> <Parameter List>" << endl;
        ParameterList();
    } else {
        if (printSwitch) cout << "<Opt Parameter List> -> ε" << endl;
    }
}


void ParameterList() {
    if (printSwitch) cout << "<Parameter List> -> <Parameter> ( , <Parameter> )*" << endl;
    Parameter();
    while (currentLexeme() == ",") {
        Match(",");
        Parameter();
    }
}


void Parameter() {
    if (printSwitch) cout << "<Parameter> -> <IDs> <Qualifier>" << endl;
    IDs();
    Qualifier();
}


// <Qualifier> ::= integer | boolean | real
void Qualifier() {
    if (printSwitch) cout << "<Qualifier> -> integer | boolean | real" << endl;
    string lex = currentLexeme();
    if (lex == "integer" || lex == "boolean" || lex == "real") {
        Match(lex);
    } else {
        syntaxError("Expected a type qualifier (integer, boolean, or real)");
    }
}


// <Body> ::= { <Statement List> }
void Body() {
    if (printSwitch) cout << "<Body> -> { <Statement List> }" << endl;
    Match("{");
    StatementList();
    Match("}");
}


// declaration list rules
void OptDeclarationList() {
    if (isIdentifierLexeme(currentLexeme()) && 
        (currentLexeme() == "integer" || currentLexeme() == "boolean" || currentLexeme() == "real")) {
    }
    string lex = currentLexeme();
    if (lex == "integer" || lex == "boolean" || lex == "real") {
        if (printSwitch) cout << "<Opt Declaration List> -> <Declaration List>" << endl;
        DeclarationList();
    } else {
        if (printSwitch) cout << "<Opt Declaration List> -> ε" << endl;
    }
}

void DeclarationList() {
    if (printSwitch) cout << "<Declaration List> -> <Declaration> ; ( <Declaration> ; )*" << endl;
    Declaration();
    Match(";");
    while (currentLexeme() == "integer" || currentLexeme() == "boolean" || currentLexeme() == "real") {
        Declaration();
        Match(";");
    }
}

void Declaration() {
    if (printSwitch) cout << "<Declaration> -> <Qualifier> <IDs>" << endl;
    Qualifier();
    IDs();
}

void IDs() {
    if (printSwitch) cout << "<IDs> -> <Identifier> ( , <Identifier> )*" << endl;
    if (isIdentifierLexeme(currentLexeme())) {
        Match(currentLexeme());
    } else {
        syntaxError("Expected identifier in <IDs>");
    }
    while (currentLexeme() == ",") {
        Match(",");
        if (isIdentifierLexeme(currentLexeme())) {
            Match(currentLexeme());
        } else {
            syntaxError("Expected identifier after ',' in <IDs>");
        }
    }
}

void StatementList() {
    if (printSwitch) cout << "<Statement List> -> (<Statement>)*" << endl;
    while (!atEnd()) {
        string lex = currentLexeme();
        if (lex == "{" || isIdentifierLexeme(lex) || lex == "if" || lex == "return" || lex == "put" || lex == "get" || lex == "while") {
            Statement();
        } else {
            break; 
        }
    }
}

void Statement() {
    string lex = currentLexeme();
    if (lex == "{") {
        if (printSwitch) cout << "<Statement> -> <Compound>" << endl;
        Compound();
    } else if (isIdentifierLexeme(lex)) {
        if (printSwitch) cout << "<Statement> -> <Assign>" << endl;
        Assign();
    } else if (lex == "if") {
        if (printSwitch) cout << "<Statement> -> <If>" << endl;
        IfStmt();
    } else if (lex == "return") {
        if (printSwitch) cout << "<Statement> -> <Return>" << endl;
        ReturnStmt();
    } else if (lex == "put") {
        if (printSwitch) cout << "<Statement> -> <Print>" << endl;
        PrintStmt();
    } else if (lex == "get") {
        if (printSwitch) cout << "<Statement> -> <Scan>" << endl;
        ScanStmt();
    } else if (lex == "while") {
        if (printSwitch) cout << "<Statement> -> <While>" << endl;
        WhileStmt();
    } else {
        syntaxError("Unrecognized start of <Statement>");
    }
}

void Compound() {
    if (printSwitch) cout << "<Compound> -> { <Statement List> }" << endl;
    Match("{");
    StatementList();
    Match("}");
}

void Assign() {
    if (printSwitch) cout << "<Assign> -> <Identifier> = <Expression> ;" << endl;
    if (!isIdentifierLexeme(currentLexeme())) {
        syntaxError("Expected identifier at start of assignment");
    }
    Match(currentLexeme()); 
    Match("=");            
    Expression();
    Match(";");
}

void IfStmt() {
    if (printSwitch) cout << "<If> -> if ( <Condition> ) <Statement> ( else <Statement> )? fi" << endl;
    Match("if");
    Match("(");
    Condition();
    Match(")");
    Statement();
    if (currentLexeme() == "else") {
        Match("else");
        Statement();
    }
    if (currentLexeme() == "fi") {
        Match("fi");
    } else {
        syntaxError("Expected 'fi' to close if statement");
    }
}

void ReturnStmt() {
    if (printSwitch) cout << "<Return> -> return ( <Expression> | ε ) ;" << endl;
    Match("return");
    if (currentLexeme() != ";") {
        Expression();
    }
    Match(";");
}

void PrintStmt() {
    if (printSwitch) cout << "<Print> -> put ( <Expression> ) ;" << endl;
    Match("put");
    Match("(");
    Expression();
    Match(")");
    Match(";");
}

void ScanStmt() {
    if (printSwitch) cout << "<Scan> -> get ( <IDs> ) ;" << endl;
    Match("get");
    Match("(");
    IDs();
    Match(")");
    Match(";");
}

void WhileStmt() {
    if (printSwitch) cout << "<While> -> while ( <Condition> ) <Statement>" << endl;
    Match("while");
    Match("(");
    Condition();
    Match(")");
    Statement();
}

void Condition() {
    if (printSwitch) cout << "<Condition> -> <Expression> <Relop> <Expression>" << endl;
    Expression();
    Relop();
    Expression();
}

void Relop() {
    if (printSwitch) cout << "<Relop> -> == | != | > | < | <= | >=" << endl;
    string lex = currentLexeme();
    if (isRelopLexeme(lex)) {
        Match(lex);
    } else {
        syntaxError("Expected a relational operator (==, !=, >, <, <=, >=)");
    }
}

void Expression() {
    if (printSwitch) cout << "<Expression> -> <Term> <ExpressionPrime>" << endl;
    Term();
    ExpressionPrime();
}

void ExpressionPrime() {
    string lex = currentLexeme();
    if (lex == "+" || lex == "-") {
        if (printSwitch) cout << "<ExpressionPrime> -> " << lex << " <Term> <ExpressionPrime>" << endl;
        Match(lex);
        Term();
        ExpressionPrime();
    } else {
        if (printSwitch) cout << "<ExpressionPrime> -> ε" << endl;
    }
}

void Term() {
    if (printSwitch) cout << "<Term> -> <Factor> <TermPrime>" << endl;
    Factor();
    TermPrime();
}

void TermPrime() {
    string lex = currentLexeme();
    if (lex == "*" || lex == "/") {
        if (printSwitch) cout << "<TermPrime> -> " << lex << " <Factor> <TermPrime>" << endl;
        Match(lex);
        Factor();
        TermPrime();
    } else {
        if (printSwitch) cout << "<TermPrime> -> ε" << endl;
    }
}

void Factor() {
    string lex = currentLexeme();
    if (lex == "-") {
        if (printSwitch) cout << "<Factor> -> - <Primary>" << endl;
        Match("-");
        Primary();
    } else {
        if (printSwitch) cout << "<Factor> -> <Primary>" << endl;
        Primary();
    }
}

void Primary() {
    string lex = currentLexeme();
    if (lex.empty()) syntaxError("Unexpected EOF in <Primary>");
    if (isIdentifierLexeme(lex)) {
        string idLex = lex;
        if ((indexPos + 1) < (int)globalToken.lexeme.size() && globalToken.lexeme[indexPos + 1] == "(") {
            if (printSwitch) cout << "<Primary> -> <Identifier> ( <IDs> )" << endl;
            Match(idLex); 
            Match("(");
            if (isIdentifierLexeme(currentLexeme())) {
                IDs();
            } else {
                syntaxError("Expected identifier(s) inside function call");
            }
            Match(")");
        } else {
            if (printSwitch) cout << "<Primary> -> <Identifier>" << endl;
            Match(idLex);
        }
    } else if (isNumberLexeme(lex)) {
        if (printSwitch) cout << "<Primary> -> <Integer|Real>" << endl;
        Match(lex);
    } else if (lex == "(") {
        if (printSwitch) cout << "<Primary> -> ( <Expression> )" << endl;
        Match("(");
        Expression();
        Match(")");
    } else if (lex == "true" || lex == "false") {
        if (printSwitch) cout << "<Primary> -> " << lex << endl;
        Match(lex);
    } else {
        syntaxError("Unexpected token in <Primary>");
    }
}



int main() {
    vector<string> testFiles = {"Rat25f.txt", "Rat25f2.txt", "Rat25f3.txt"};

    for (size_t t = 0; t < testFiles.size(); ++t) {
        string inputFile = testFiles[t];
        string outputFile = "output" + to_string(t + 1) + ".txt";

        ifstream myFile(inputFile);
        if (!myFile) {
            cerr << "Error opening file '" << inputFile << "'." << endl;
            continue;
        }

        ofstream outFile(outputFile);
        if (!outFile) {
            cerr << "Error creating output file '" << outputFile << "'." << endl;
            myFile.close();
            continue;
        }

        cout << "Processing " << inputFile << "..." << endl;
        outFile << "Processing " << inputFile << "..." << endl;

        cout << "Starting lexical analysis..." << endl;
        outFile << "Starting lexical analysis..." << endl;

        globalToken = lexer(myFile);

        cout << "Lexical Analysis Complete. Tokens and Lexemes will be printed:" << endl;
        outFile << "Lexical Analysis Complete. Tokens and Lexemes will be printed:" << endl;

        for (size_t i = 0; i < globalToken.lexeme.size(); ++i) {
            string tokenStr = (i < globalToken.token.size() ? globalToken.token[i] : "??");
            cout << i << ": TokenType: " << tokenStr << "  Lexeme: " << globalToken.lexeme[i] << endl;
            outFile << i << ": TokenType: " << tokenStr << "  Lexeme: " << globalToken.lexeme[i] << endl;
        }

        indexPos = 0;
        cout << "\nStarting Syntax Analysis...\n" << endl;
        outFile << "\nStarting Syntax Analysis...\n" << endl;

        streambuf* oldCoutBuf = cout.rdbuf();
        cout.rdbuf(outFile.rdbuf());

        Rat25F();

        cout.rdbuf(oldCoutBuf); 

        myFile.close();
        outFile.close();

        cout << "Finished processing " << inputFile << ". Output saved to " << outputFile << "\n" << endl;
    }

    return 0;
}
