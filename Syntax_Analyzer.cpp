#include "Lexical_Analyzer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int indexPos = 0;
bool printSwitch = true; // toggle verbose printing

vector<string> instructions;
int instructionIndex = 0;

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
string currentLexeme()
{
    if (indexPos < (int)globalToken.lexeme.size())
        return globalToken.lexeme[indexPos];
    return "";
}

int emitInstruction(const string &instr) //PushM add
{
    instructions.push_back(to_string(instructionIndex) + " " + instr);
    return instructionIndex++;
}

bool atEnd()
{
    return indexPos >= (int)globalToken.lexeme.size();
}

// Skip comment lexemes ("comment")
void SkipComments()
{
    while (!atEnd() && currentLexeme()[0] == '"')
    {
        if (printSwitch)
            cout << "Skipping comment: " << currentLexeme() << endl;
        indexPos++;
    }
}

// Syntax error
void syntaxError(const string &msg)
{
    string lex = atEnd() ? "EOF" : currentLexeme();
    cerr << "Syntax Error: " << msg << " at token '" << lex << "' (index " << indexPos << ")" << endl;
    exit(1);
}

// Match a specific lexeme
void Match(const string &expected)
{
    SkipComments();
    if (atEnd())
        syntaxError("Expected '" + expected + "' but found EOF");
    string cur = currentLexeme();
    if (cur == expected)
    {
        if (printSwitch)
            cout << "Token: " << cur << "   (matched '" << expected << "')" << endl;
        indexPos++;
        SkipComments();
    }
    else
        syntaxError("Expected '" + expected + "' but found '" + cur + "'");
}

// ===== Grammar Rules =====

// <Rat25F> ::= <Opt Function Definitions> # <Opt Declaration List> <Statement List> #
void Rat25F()
{
    if (printSwitch)
        cout << "<Rat25F> -> <Opt Function Definitions> # <Opt Declaration List> <Statement List> #" << endl;

    OptFunctionDefinitions();
    Match("#");
    OptDeclarationList();
    StatementList();
    Match("#");

    if (!atEnd())
    {
        cerr << "Warning: extra tokens remain starting at index " << indexPos << endl;
    }
    else
    {
        cout << "Parsing completed successfully." << endl;
    }
}

// <Opt Function Definitions> ::= <Function Definitions> | ε
void OptFunctionDefinitions()
{
    if (currentLexeme() == "function")
    {
        if (printSwitch)
            cout << "<Opt Function Definitions> -> <Function Definitions>" << endl;
        FunctionDefinitions();
    }
    else if (printSwitch)
        cout << "<Opt Function Definitions> -> ε" << endl;
}

// <Function Definitions> ::= <Function> (<Function Definitions>)*
void FunctionDefinitions()
{
    if (printSwitch)
        cout << "<Function Definitions> -> <Function> (<Function Definitions>)*" << endl;
    Function();
    while (currentLexeme() == "function")
        Function();
}

// <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void Function()
{
    if (printSwitch)
        cout << "<Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << endl;
    Match("function");
    if (isIdentifierLexeme(currentLexeme()))
        Match(currentLexeme());
    else
        syntaxError("Expected function name (identifier)");
    Match("(");
    OptParameterList();
    Match(")");
    OptDeclarationList();
    Body();
}

// <Opt Parameter List> ::= <Parameter List> | ε
void OptParameterList()
{
    if (isIdentifierLexeme(currentLexeme()))
    {
        if (printSwitch)
            cout << "<Opt Parameter List> -> <Parameter List>" << endl;
        ParameterList();
    }
    else if (printSwitch)
        cout << "<Opt Parameter List> -> ε" << endl;
}

// <Parameter List> ::= <Parameter> (, <Parameter>)*
void ParameterList()
{
    if (printSwitch)
        cout << "<Parameter List> -> <Parameter> (, <Parameter>)*" << endl;
    Parameter();
    while (currentLexeme() == ",")
    {
        Match(",");
        Parameter();
    }
}

// <Parameter> ::= <IDs> <Qualifier>
void Parameter()
{
    if (printSwitch)
        cout << "<Parameter> -> <IDs> <Qualifier>" << endl;
    IDs();
    Qualifier();
}

// <Qualifier> ::= integer | boolean | real
void Qualifier()
{
    if (printSwitch)
        cout << "<Qualifier> -> integer | boolean | real" << endl;
    string lex = currentLexeme();
    if (lex == "integer" || lex == "boolean" || lex == "real")
        Match(lex);
    else
        syntaxError("Expected type qualifier (integer, boolean, or real)");
}

// <Body> ::= { <Statement List> }
void Body()
{
    if (printSwitch)
        cout << "<Body> -> { <Statement List> }" << endl;
    Match("{");
    StatementList();
    Match("}");
}

// <Opt Declaration List> ::= <Declaration List> | ε
void OptDeclarationList()
{
    string lex = currentLexeme();
    if (lex == "integer" || lex == "boolean" || lex == "real")
    {
        if (printSwitch)
            cout << "<Opt Declaration List> -> <Declaration List>" << endl;
        DeclarationList();
    }
    else if (printSwitch)
        cout << "<Opt Declaration List> -> ε" << endl;
}

// <Declaration List> ::= <Declaration> ; (<Declaration> ;)*
void DeclarationList()
{
    if (printSwitch)
        cout << "<Declaration List> -> <Declaration> ; (<Declaration> ;)*" << endl;
    Declaration();
    Match(";");
    while (currentLexeme() == "integer" || currentLexeme() == "boolean" || currentLexeme() == "real")
    {
        Declaration();
        Match(";");
    }
}

// <Declaration> ::= <Qualifier> <IDs>
void Declaration()
{
    if (printSwitch)
        cout << "<Declaration> -> <Qualifier> <IDs>" << endl;
    Qualifier();
    IDs();
}

// <IDs> ::= <Identifier> (, <Identifier>)*
void IDs()
{
    if (printSwitch)
        cout << "<IDs> -> <Identifier> (, <Identifier>)*" << endl;
    if (isIdentifierLexeme(currentLexeme()))
        Match(currentLexeme());
    else
        syntaxError("Expected identifier in <IDs>");
    while (currentLexeme() == ",")
    {
        Match(",");
        if (isIdentifierLexeme(currentLexeme()))
            Match(currentLexeme());
        else
            syntaxError("Expected identifier after ',' in <IDs>");
    }
}

// <Statement List> ::= (<Statement>)*
void StatementList()
{
    if (printSwitch)
        cout << "<Statement List> -> (<Statement>)*" << endl;
    while (!atEnd())
    {
        string lex = currentLexeme();
        if (lex == "{" || isIdentifierLexeme(lex) || lex == "if" || lex == "return" || lex == "put" || lex == "get" || lex == "while")
        {
            Statement();
        }
        else
            break;
    }
}

// <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
void Statement()
{
    string lex = currentLexeme();
    if (lex == "{")
        Compound();
    else if (lex == "if")
        IfStmt();  //1 
    else if (lex == "return")
        ReturnStmt();
    else if (lex == "put")
        PrintStmt();//2
    else if (lex == "get")
        ScanStmt();//3
    else if (lex == "while")
        WhileStmt();//4
    else if (isIdentifierLexeme(lex))
        Assign();
    else
        syntaxError("Unrecognized start of <Statement>");
}

// <Compound> ::= { <Statement List> }
void Compound()
{
    Match("{");
    StatementList();
    Match("}");
}

// <Assign> ::= <Identifier> = <Expression> ;
void Assign()
{
    Match(currentLexeme());
    //Symbol s = lookupSymbol(currentLexeme());
    Match("=");
    Expression();
    //emitInstruction("POPM " + s.address);
    Match(";");
}

// <If> ::= if ( <Condition> ) <Statement> ( else <Statement> )? fi
void IfStmt()
{
    Match("if");
    Match("(");
    Condition();
    Match(")");
    Statement();
    if (currentLexeme() == "else")
    {
        Match("else");
        Statement();
    }
    Match("fi");
}

// <Return> ::= return ( <Expression> | ε ) ;
void ReturnStmt()
{
    Match("return");
    if (currentLexeme() != ";")
        Expression();
    Match(";");
}

// <Print> ::= put ( <Expression> ) ;
void PrintStmt()
{
    Match("put");
    Match("(");
    Expression();
    Match(")");
    Match(";");
}

// <Scan> ::= get ( <IDs> ) ;
void ScanStmt()
{
    Match("get");
    Match("(");
    IDs();
    Match(")");
    Match(";");
}

// <While> ::= while ( <Condition> ) <Statement>
void WhileStmt()
{
    Match("while");
    Match("(");
    Condition();
    Match(")");
    Statement();
}

// <Condition> ::= <Expression> <Relop> <Expression>
void Condition()
{
    Expression();
    Relop();
    Expression();
}

// <Relop> ::= == | != | > | < | <= | >=
void Relop()
{
    if (isRelopLexeme(currentLexeme()))
        Match(currentLexeme());
        
        if(currentLexeme() == "==")
            emitInstruction("EQU");
        else if(currentLexeme() == "!=")
            emitInstruction("NEQ");
        else if(currentLexeme() == ">")
            emitInstruction("GRT");
        else if(currentLexeme() == "<")
            emitInstruction("LES");
        else if(currentLexeme() == "<=")
            emitInstruction("LEQ");
        else if(currentLexeme() == ">=")
            emitInstruction("GEQ");
    else
        syntaxError("Expected relational operator (==, !=, >, <, <=, >=)");
}

// <Expression> ::= <Term> <ExpressionPrime>
void Expression()
{
    Term();
    ExpressionPrime();
}

// <ExpressionPrime> ::= (+|-) <Term> <ExpressionPrime> | ε
void ExpressionPrime()
{
    string lex = currentLexeme();
    if (lex == "+" || lex == "-")
    {
        Match(lex);
        Term();
        if(lex == "+")
            emitInstruction("ADD");
        else
            emitInstruction("SUB");
        ExpressionPrime();
    }
}

// <Term> ::= <Factor> <TermPrime>
void Term()
{
    Factor();
    TermPrime();
}

// <TermPrime> ::= (*|/) <Factor> <TermPrime> | ε
void TermPrime()
{
    string lex = currentLexeme();
    if (lex == "*" || lex == "/")
    {
        Match(lex);
        Factor();     //-x * y                              
         if(lex == "*")
            emitInstruction("MUL");
        else
            emitInstruction("DIV");

        TermPrime();
       
    }
}

// <Factor> ::= - <Primary> | <Primary>
void Factor()
{
    if (currentLexeme() == "-")  // 
    {
        Match("-");
        Primary(); // x
        emitInstruction("PUSHI -1");
        emitInstruction("MUL");
        //-x 
    }
    else
        Primary();
}

// <Primary> ::= <Identifier> | <Integer> | (<Expression>) | true | false
void Primary()
{
    string lex = currentLexeme();
    if (isIdentifierLexeme(lex) || isNumberLexeme(lex) || lex == "true" || lex == "false")
    {
        if (lex == "true")
        {
            Match("true");
            emitInstruction("PUSHI 1");
            return;
        }
        else if (lex == "false")
        {
            Match("false");
            emitInstruction("PUSHI 0");
            return;
        }
        else if(isNumberLexeme(lex))
        {
            Match(lex);
            emitInstruction("PUSHI " + lex);
        }
        else // Identifier
        {
            Match(lex);
            //Symbol s = lookupSymbol(lex);
            //
            // lookup lex here
            emitInstruction("PUSHM " + lex); // Put memory address of lex
        }
    }
    else if (lex == "(")
    {
        Match("(");
        Expression();
        Match(")");
    }
    else
        syntaxError("Unexpected primary");
}

// ===== Main =====
int main()
{
    ifstream myFile("Rat25f.txt");
    if (!myFile)
    {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    globalToken = lexer(myFile);
    myFile.close();

    Rat25F();

    return 0;
}
