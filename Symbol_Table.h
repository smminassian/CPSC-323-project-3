#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
using namespace std;

struct Symbol {
    string name;
    int memory;
    string type;   // "integer" or "boolean"
};

class SymbolTable {
private:
    vector<Symbol> table;
    int nextMemory = 10000;

public:
    bool exists(const string& name);
    void insert(const string& name, const string& type);
    Symbol lookup(const string& name);
    void print();
};

#endif
