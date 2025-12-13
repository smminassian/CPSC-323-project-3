#include "Symbol_Table.h"
#include <iostream>

bool SymbolTable::exists(const string& name) {
    for (auto& s : table)
        if (s.name == name)
            return true;
    return false;
}

void SymbolTable::insert(const string& name, const string& type) {
    if (exists(name)) {
        cerr << "Semantic Error: Identifier '" << name << "' declared twice\n";
        exit(1);
    }
    table.push_back({name, nextMemory++, type});
}

Symbol SymbolTable::lookup(const string& name) {
    for (auto& s : table)
        if (s.name == name)
            return s;

    cerr << "Semantic Error: Identifier '" << name << "' used before declaration\n";
    exit(1);
}

void SymbolTable::print() {
    cout << "\nSymbol Table\n";
    cout << "Identifier   MemoryLocation   Type\n";
    for (auto& s : table)
        cout << s.name << "          " << s.memory << "          " << s.type << endl;
}
