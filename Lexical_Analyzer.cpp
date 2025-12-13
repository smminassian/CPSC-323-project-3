#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include "Lexical_Analyzer.h"
using namespace std;

Token globalToken;

Token lexer(ifstream &myFile)
{
    Token t;
    string line;
    cout << "Starting lexical analysis..." << endl;

    string current_lexeme = "";

    while (getline(myFile, line))
    {
        int ix = 0;

        while (ix < line.size())
        {
            char ch = line[ix];

            // Skip comments enclosed in ""
            if (ch == '"')
            {
                ix++;
                while (ix < line.size() && line[ix] != '"') ix++;
                ix++;
                continue;
            }

            // Check for separators and operators
            if (checkSeparator(string(1, ch)) != "invalid" || checkOperator(string(1, ch)) != "invalid")
            {
                if (!current_lexeme.empty())
                {
                    if (checkKeyword(current_lexeme) != "identifier ")
                        t.lexeme.push_back(current_lexeme), t.token.push_back(checkKeyword(current_lexeme));
                    else if (isalpha(current_lexeme[0]))
                        t.lexeme.push_back(current_lexeme), t.token.push_back(IdentifierFSM(current_lexeme));
                    else if (isdigit(current_lexeme[0]))
                        t.lexeme.push_back(current_lexeme), t.token.push_back(NumberFSM(current_lexeme));
                    else
                        t.lexeme.push_back(current_lexeme), t.token.push_back("invalid");

                    current_lexeme.clear();
                }

                string s(1, ch);
                if (checkOperator(s) != "invalid")
                    t.lexeme.push_back(s), t.token.push_back(checkOperator(s));
                else if (checkSeparator(s) != "invalid")
                    t.lexeme.push_back(s), t.token.push_back(checkSeparator(s));

                ix++;
                continue;
            }

            // Skip whitespace
            if (isspace(ch))
            {
                if (!current_lexeme.empty())
                {
                    if (checkKeyword(current_lexeme) != "identifier ")
                        t.lexeme.push_back(current_lexeme), t.token.push_back(checkKeyword(current_lexeme));
                    else if (isalpha(current_lexeme[0]))
                        t.lexeme.push_back(current_lexeme), t.token.push_back(IdentifierFSM(current_lexeme));
                    else if (isdigit(current_lexeme[0]))
                        t.lexeme.push_back(current_lexeme), t.token.push_back(NumberFSM(current_lexeme));
                    else
                        t.lexeme.push_back(current_lexeme), t.token.push_back("invalid");

                    current_lexeme.clear();
                }

                ix++;
                continue;
            }

            current_lexeme += ch;
            ix++;
        }

        if (!current_lexeme.empty())
        {
            if (checkKeyword(current_lexeme) != "identifier ")
                t.lexeme.push_back(current_lexeme), t.token.push_back(checkKeyword(current_lexeme));
            else if (isalpha(current_lexeme[0]))
                t.lexeme.push_back(current_lexeme), t.token.push_back(IdentifierFSM(current_lexeme));
            else if (isdigit(current_lexeme[0]))
                t.lexeme.push_back(current_lexeme), t.token.push_back(NumberFSM(current_lexeme));
            else
                t.lexeme.push_back(current_lexeme), t.token.push_back("invalid");

            current_lexeme.clear();
        }
    }

    return t;
}

// ---------------------- FSM FUNCTIONS ----------------------

string IdentifierFSM(const string &input)
{
    const int IdBeginning = 0;
    const int IdValid = 1;
    const int IdInvalid = 2;

    int state = IdBeginning;

    for (size_t i = 0; i < input.size(); i++)
    {
        char ch = input[i];

        switch (state)
        {
        case IdBeginning:
            if (isalpha(ch))
                state = IdValid;
            else
                state = IdInvalid;
            break;

        case IdValid:
            if (isalnum(ch))
                state = IdValid;
            else
                state = IdInvalid;
            break;

        case IdInvalid:
            break;
        }

        if (state == IdInvalid)
            break;
    }

    if (state == IdValid)
        return "identifier";
    else
        return "invalid";
}

string NumberFSM(const string &input)
{
    const int NumBeginning = 0;
    const int NumInt = 1;
    const int NumReal = 2;
    const int NumInvalid = 3;

    int state = NumBeginning;

    for (size_t i = 0; i < input.size(); i++)
    {
        char ch = input[i];

        switch (state)
        {
        case NumBeginning:
            if (isdigit(ch))
                state = NumInt;
            else if (ch == '.')
                state = NumReal;
            else
                state = NumInvalid;
            break;

        case NumInt:
            if (isdigit(ch))
                state = NumInt;
            else if (ch == '.')
                state = NumReal;
            else
                state = NumInvalid;
            break;

        case NumReal:
            if (isdigit(ch))
                state = NumReal;
            else
                state = NumInvalid;
            break;

        case NumInvalid:
            break;
        }

        if (state == NumInvalid)
            break;
    }

    if (state == NumInt)
        return "integer ";
    else if (state == NumReal)
        return "real ";
    else
        return "invalid";
}

// ---------------------- CHECK FUNCTIONS ----------------------

string checkKeyword(const string &input)
{
    string keywords[] = {"if","else","fi","while","return","put","get","integer","boolean","real","true","false","function"};
    for (string k : keywords)
        if (input == k)
            return "keyword ";
    return "identifier ";
}

string checkOperator(const string &input)
{
    string ops[] = {"+","-","*","/","<=",">=","=","<",">","!="};
    for (string op : ops)
        if (input == op)
            return "operator ";
    return "invalid";
}

string checkSeparator(const string &input)
{
    string seps[] = {";",",","(",")","{","}","[","]","#"};
    for (string s : seps)
        if (input == s)
            return "separator ";
    return "invalid";
}

// ---------------------- LEXEME CHECKS ----------------------

bool isIdentifierLexeme(const string &lex)
{
    return !lex.empty() && isalpha(lex[0]);
}

bool isNumberLexeme(const string &lex)
{
    return !lex.empty() && isdigit(lex[0]);
}

bool isRelopLexeme(const string &lex)
{
    return lex=="==" || lex=="!=" || lex==">" || lex=="<" || lex=="<=" || lex==">=";
}



// int main()
// {
//     // Input and output filenames
//     vector<string> inputFiles = { "Rat25f.txt", "Rat25f2.txt", "Rat25f3.txt" };
//     vector<string> outputFiles = { "output.txt", "output2.txt", "output3.txt" };

//     // Making sure the vectors match in size
//     if (inputFiles.size() != outputFiles.size())
//     {
//         cerr << "Error: Input and output file lists do not match in size." << endl;
//         return 1;
//     }

//     for (size_t i = 0; i < inputFiles.size(); i++)
//     {
//         ifstream myFile(inputFiles[i]);
//         ofstream outFile(outputFiles[i]);

//         if (!myFile)
//         {
//             cerr << "Error opening input file: " << inputFiles[i] << endl;
//             continue; 
//         }

//         if (!outFile)
//         {
//             cerr << "Error opening output file: " << outputFiles[i] << endl;
//             myFile.close();
//             continue;
//         }

//         cout << "Processing " << inputFiles[i] << " -> " << outputFiles[i] << endl;

//         Token t = lexer(myFile);

//         for (size_t j = 0; j < t.lexeme.size(); j++)
//         {
//             outFile << t.token[j] << " " << t.lexeme[j] << endl;
//             cout << t.token[j] << " " << t.lexeme[j] << endl;
//         }

//         myFile.close();
//         outFile.close();
//     }

//     return 0;
// }
