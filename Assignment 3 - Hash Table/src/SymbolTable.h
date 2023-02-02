#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Parameter {
public:
    string para;
    Parameter() {
        para = "";
    }
};
class Identifier {
public:
    string identifier_name;
    int level;
    string numOfParameter;
    Parameter* paraType;
    string parameter;
    string value;
    string type;
    Identifier() {
        this->identifier_name = "";
        this->parameter = "";
        this->value = "";
        this->type = "unknow";
        this->numOfParameter = "-1";
        this->paraType = nullptr;
        this->level = 0;
    }
    Identifier(string identifier_name, string parameter, string value, string type, int level, string numOfParameter) {
        this->identifier_name = identifier_name;
        this->parameter = parameter;
        this->value = value;
        this->type = type;
        this->level = level;
        this->numOfParameter = numOfParameter;
    }
    void clear() {
        if(!paraType) return;
        delete[] paraType;
        paraType = nullptr;
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
        if (!head) return;
        while (head) {
            DataNode* temp = head;
            head = head->next;
            temp->data.clear();
            delete temp;
            temp = nullptr;
        }
        tail = nullptr;
    }
};
class Constant {
public:
    string c_string, c1_string, c2_string, m_string;
    int c, c1, c2, m;
    Constant() {
        c = 0;
        c1 = 0;
        c2 = 0;
        m = 0;
        c_string = "0";
        c1_string = "0";
        c2_string = "0";
        m_string = "0";
    }
};
class SymbolTable {
public:
    int (*hp)(string, int, Constant);
    Identifier* data;
    bool* status;
    int size;
    SymbolTable() {
        this->hp = nullptr;
        this->data = nullptr;
        this->status = nullptr;
        this->size = 0;
    }
    void run(string filename);
};
#endif