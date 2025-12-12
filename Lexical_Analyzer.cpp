#include "Lexical_Analyzer.h"
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

Token globalToken;

string IdentifierFSM(const string &input);
string NumberFSM(const string &input);
string checkKeyword(const string &input);
string checkOperator(const string &input);
string checkSeparator(const string &input);

Token lexer(ifstream &myFile) {
    Token t;
    string line;

    while (getline(myFile, line)) {
        string currentLexeme = "";
        for (size_t i = 0; i < line.size(); i++) {
            char ch = line[i];

            if (isspace(ch) || checkOperator(string(1,ch)) != "invalid" || checkSeparator(string(1,ch)) != "invalid") {

                if (!currentLexeme.empty()) {
                    if (checkKeyword(currentLexeme) != "identifier ")
                        t.lexeme.push_back(currentLexeme), t.token.push_back(checkKeyword(currentLexeme));
                    else if (isalpha(currentLexeme[0]))
                        t.lexeme.push_back(currentLexeme), t.token.push_back(IdentifierFSM(currentLexeme));
                    else if (isdigit(currentLexeme[0]))
                        t.lexeme.push_back(currentLexeme), t.token.push_back(NumberFSM(currentLexeme));
                    else
                        t.lexeme.push_back(currentLexeme), t.token.push_back("invalid");

                    currentLexeme.clear();
                }

                string s(1,ch);
                if (checkOperator(s) != "invalid")
                    t.lexeme.push_back(s), t.token.push_back(checkOperator(s));
                else if (checkSeparator(s) != "invalid")
                    t.lexeme.push_back(s), t.token.push_back(checkSeparator(s));

                continue;
            }

            currentLexeme += ch;
        }

        if (!currentLexeme.empty()) {
            if (checkKeyword(currentLexeme) != "identifier ")
                t.lexeme.push_back(currentLexeme), t.token.push_back(checkKeyword(currentLexeme));
            else if (isalpha(currentLexeme[0]))
                t.lexeme.push_back(currentLexeme), t.token.push_back(IdentifierFSM(currentLexeme));
            else if (isdigit(currentLexeme[0]))
                t.lexeme.push_back(currentLexeme), t.token.push_back(NumberFSM(currentLexeme));
            else
                t.lexeme.push_back(currentLexeme), t.token.push_back("invalid");
        }
    }

    return t;
}

string IdentifierFSM(const string &input) {
    if (input.empty() || !isalpha(input[0])) return "invalid";
    for (char c : input) if (!isalnum(c)) return "invalid";
    return "identifier";
}

string NumberFSM(const string &input) {
    bool hasDot = false;
    for (char c : input) {
        if (c == '.') {
            if (hasDot) return "invalid";
            hasDot = true;
        }
        else if (!isdigit(c)) return "invalid";
    }
    return hasDot ? "real " : "integer ";
}

string checkKeyword(const string &input) {
    string keywords[] = {"if","else","fi","while","return","put","get","integer","boolean","real","true","false","function"};
    for (string k : keywords) if (input == k) return "keyword ";
    return "identifier ";
}

string checkOperator(const string &input) {
    string ops[] = {"+","-","*","/","<=",">=","=","<",">","!="};
    for (string op : ops) if (input == op) return "operator ";
    return "invalid";
}

string checkSeparator(const string &input) {
    string seps[] = {";",",","(",")","{","}","[","]","#"};
    for (string s : seps) if (input == s) return "separator ";
    return "invalid";
}

bool isIdentifierLexeme(const string &lex) {
    return !lex.empty() && isalpha(lex[0]);
}

bool isNumberLexeme(const string &lex) {
    return !lex.empty() && isdigit(lex[0]);
}

bool isRelopLexeme(const string &lex) {
    return lex=="=="||lex=="!="||lex==">"||lex=="<"||lex=="<="||lex==">=";
}
