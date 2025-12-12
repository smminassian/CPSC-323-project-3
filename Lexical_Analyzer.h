#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Token {
    vector<string> lexeme;
    vector<string> token;
};


extern Token globalToken;


Token lexer(ifstream &myFile);


string IdentifierFSM(const string &input);
string NumberFSM(const string &input);
string checkKeyword(const string &input);
string checkOperator(const string &input);
string checkSeparator(const string &input);


bool isIdentifierLexeme(const string &lex);
bool isNumberLexeme(const string &lex);
bool isRelopLexeme(const string &lex);

#endif
