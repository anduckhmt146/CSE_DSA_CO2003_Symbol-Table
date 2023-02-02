#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Identifier {
public:
    string identifier_name, type, value;
    int scope;
    Identifier() : identifier_name(""), type(""), value(""), scope(0) {}
    Identifier(string identifier_name, string type, string value, int scope) {
        this->identifier_name = identifier_name;
        this->type = type;
        this->value = value;
        this->scope = scope;
    }
};
class IdentifierNode {
public:
    Identifier data;
    IdentifierNode* next;
    IdentifierNode* prev;
    IdentifierNode(): data(Identifier()), next(nullptr), prev(nullptr) {}
    IdentifierNode(Identifier data, IdentifierNode* next, IdentifierNode* prev) {
        this->data = data;
        this->next = next;
        this->prev = prev;
    }
};
class SymbolTable 
{
public:
    IdentifierNode* head;
    IdentifierNode* tail;
    SymbolTable() : head(nullptr), tail(nullptr) {}
    SymbolTable(IdentifierNode* head, IdentifierNode* tail) {
        this->head = head;
        this->tail = tail;
    }
    void run(string filename);
};
#endif