#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Identifier {
public:
    string identifier_name;
    int level;
    string type;
    string parameter;
    string exact_type;
    string value;
    string isGlobal;
    Identifier() : identifier_name(""), level(0), type(""), parameter(""), exact_type(""), value(""), isGlobal("") {}
    Identifier(string identifier_name, int level, string type, string parameter, string exact_type, string value, string isGlobal) {
        this->identifier_name = identifier_name;
        this->level = level;
        this->type = type;
        this->parameter = parameter;
        this->exact_type = exact_type;
        this->value = value;
        this->isGlobal = isGlobal;
    }
};
class IdentifierNode {
public:
    Identifier data;
    IdentifierNode* left;
    IdentifierNode* right;
    IdentifierNode* parent;
    IdentifierNode() : data(Identifier()), left(nullptr), right(nullptr), parent(nullptr) {}
    IdentifierNode(Identifier data) {
        this->data = data;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
};
class DataNode {
public:
    Identifier data;
    DataNode* next;
    DataNode() : data(Identifier()), next(nullptr) {}
    DataNode(Identifier data) {
        this->data = data;
        this->next = nullptr;
    }
};
class DataBlocked {
public:
    DataNode* head;
    DataNode* tail;
    DataBlocked() : head(nullptr), tail(nullptr) {}
    DataBlocked(DataNode* head, DataNode* tail) {
        this->head = head;
        this->tail = tail;
    }
    void add(Identifier id) {
        DataNode* newNode = new DataNode(id);
        if (!head) {
            head = newNode;
            tail = newNode;
            return;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    void clear() {
        if(!head) return; 
        while (head) {
            DataNode* temp = head;
            head = head->next;
            delete temp;
            temp = nullptr;
        }
        tail = nullptr;
    }
};
class SymbolTable
{
public:
    IdentifierNode* root;
    SymbolTable(): root(nullptr) {}
    SymbolTable(IdentifierNode* root) {
        this->root = root;
    }
    void run(string filename);
};
#endif