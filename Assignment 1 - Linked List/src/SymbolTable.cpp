#include "SymbolTable.h"

int countNumberOfComponent(string s) {
	int length = s.length(), count = 0;
	bool flag = true;
	for (int i = 0; i < length; i++) {
		if ((int)s[i] == 39) flag = false;
		if (s[i] == ' ' && flag) count++;
	}
	return count + 1;
}
void tokenize(string s, string& method, Identifier& id, int numberOfComponent) {
	string key = " ";
	int start = 0, end = 0, s_length = s.length(), key_length = key.length();
	if (numberOfComponent == 1) { method = s.substr(start, s_length); }
	else if (numberOfComponent == 2) {
		for (; end < s_length; end++) {
			if (s[end] == ' ') { method = s.substr(start, end - start); start = end + key_length; }
		}
		id.identifier_name = s.substr(start, end - start);
	}
	else if (numberOfComponent == 3) {
		bool flag1 = true, flag2 = false, flag = true;
		for (; end < s_length; end++) {
			if ((int)s[end] == 39) flag = false;
			if (s[end] == ' ' && flag) {
				if (flag1) {
					method = s.substr(start, end - start);
					flag1 = false;
					flag2 = true;
					start = end + key_length;
					continue;
				}
				if (flag2) {
					id.identifier_name = s.substr(start, end - start);
					start = end + key_length;
					continue;
				}
			}
		}
		if (method == "ASSIGN") { id.value = s.substr(start, end - start); }
		else {
			id.type = s.substr(start, end - start);
			id.value = "";
		}
	}
}
bool checkLowerCase(char c) { 
	if(c >= 'a' && c <= 'z') return true;
	return false;
}
bool checkUpperCase(char c) { 
	if(c >= 'A' && c <= 'Z') return true;
	return false;
}
bool checkNumeric(char c) {
	if (c >= '0' && c <= '9') return true;
	return false;
}
bool checkVariableName(string s) {
	if (!checkLowerCase(s[0])) return false;
	int length = s.length();
	for (int i = 1; i < length; i++) {
		if (!checkLowerCase(s[i]) && !checkUpperCase(s[i]) && !checkNumeric(s[i]) && s[i] != '_') return false;
	}
	return true;
}
bool checkStringType(string s) {
	int length = s.length();
	if ((int)s[0] == 39 && (int)s[length - 1] == 39) {
		for (int i = 1; i < length - 2; i++) {
			if (!checkLowerCase(s[i]) && !checkUpperCase(s[i]) && !checkNumeric(s[i]) && s[i] != ' ') return false;
		}
		return true;
	}
	else return false;
}
bool checkNumberType(string s) {
	int length = s.length();
	for (int i = 0; i < length; i++) {
		if (!checkNumeric(s[i])) return false;
	}
	return true;
}
bool checkValue(string s) {
	if (checkStringType(s) || checkVariableName(s) || checkNumberType(s)) return true;
	return false;
}
bool checkInvalidInstruction(string method, Identifier id, int numberOfComponent) {
	if (numberOfComponent > 3) return false;
	if (method != "INSERT" && method != "ASSIGN" && method != "BEGIN" &&
		method != "END" && method != "LOOKUP" && method != "PRINT" && method != "RPRINT") return false;
	if (method == "INSERT" || method == "ASSIGN") {
		if (numberOfComponent != 3) return false;
		if (!checkVariableName(id.identifier_name)) return false;
		if (method == "INSERT") {
			if (id.type != "number" && id.type != "string") return false;
		}
		else if (method == "ASSIGN") {
			if (!checkValue(id.value)) return false;
		} 
	}
	else if (method == "BEGIN" || method == "END" || method == "PRINT" || method == "RPRINT") {
		if (numberOfComponent != 1) return false;
	}
	else if (method == "LOOKUP") {
		if (numberOfComponent != 2) return false;
		if (!checkVariableName(id.identifier_name)) return false;
	}
	return true;
}
bool isExist(SymbolTable table, string found, string method, int scope) {
	IdentifierNode* p = table.tail;
	while (p) {
		if (p->data.identifier_name == found) {
			if(method == "ASSIGN") return true;
			if (method == "INSERT") return p->data.scope == scope;
		}
		p = p->prev;
	}
	return false;
}
void insert(SymbolTable& table, Identifier id) {
	IdentifierNode* newNode = new IdentifierNode(id, nullptr, nullptr);
	if (table.head == nullptr) {
		table.head = newNode;
		table.tail = newNode;
		return;
	}
	else {
		table.tail->next = newNode;
		newNode->prev = table.tail;
		table.tail = newNode;
	}
}
bool checkTypeMismatchForVariable(SymbolTable table, string identifier_name, string value) {
	string identifier_type = "", value_type = "";
	IdentifierNode* p = table.tail;
	bool flag_name = false, flag_value = false;
	while (p) {
		if (identifier_type != "" && value_type != "") break;
		if (p->data.identifier_name == identifier_name && !flag_name) { 
			identifier_type = p->data.type; 
			flag_name = true;	
		}
		if (p->data.identifier_name == value && !flag_value) { 
			value_type = p->data.type; 
			flag_value = true;	
		}
		p = p->prev;
	}
	if (identifier_type == value_type) return true;
	return false;
}
bool checkTypeMismatchForValue(SymbolTable table, string identifier_name, string type) {
	IdentifierNode* p = table.tail;
	while (p) {
		if (p->data.identifier_name == identifier_name) {
			return p->data.type == type;
		}
		p = p->prev;
	}
	return false;
}
void removeTail(SymbolTable& table) {
	if (table.head == nullptr) return;
	else {
		if (table.head != table.tail) {
			IdentifierNode* del = table.tail;
			table.tail = table.tail->prev;
			del->prev = nullptr;
			table.tail->next = nullptr;
			delete del;
			del = nullptr;
		}
		else {
			IdentifierNode* del = table.head;
			table.head = nullptr;
			table.tail = nullptr;
			delete del;
			del = nullptr;
		}
	}
}
void removeScope(SymbolTable& table, int max_scope) {
	IdentifierNode* p = table.tail;
	int count = 0;
	while (p) {
		if (p->data.scope < max_scope) break;
		p = p->prev;
		count++;
	}
	for (int i = 0; i < count; i++) { removeTail(table); }
}
int lookup(SymbolTable table, string identifier_name) {
	IdentifierNode* p = table.tail;
	while (p) {
		if (p->data.identifier_name == identifier_name) {
			return p->data.scope;
		}
		p = p->prev;
	}
	return -1;
}
bool checkExistInLargerScope(SymbolTable table, string identifier_name, int curr_scope) {
	IdentifierNode* p = table.tail;
	while (p && p->data.scope > curr_scope) {
		if (p->data.identifier_name == identifier_name) return true;
		p = p->prev;
	}
	return false;
}
string printScope(SymbolTable table) {
	string res = "";
	if (table.head == nullptr) return res;
	int max_scope = table.tail->data.scope;
	IdentifierNode* p = table.tail;
	while (p) {
		if (p->data.scope == max_scope) { res = p->data.identifier_name + "//" + to_string(p->data.scope) + " " + res; }
		else if (p->data.scope < max_scope) {
			if (!checkExistInLargerScope(table, p->data.identifier_name, p->data.scope)) {
				res = p->data.identifier_name + "//" + to_string(p->data.scope) + " " + res;
			}
		}
		p = p->prev;
	}
	if (res == "") return res;
	int length = res.length();
	if (res[length - 1] == ' ') res.erase(res.end() - 1);
	return res;
}
string rprintScope(SymbolTable table) {
	string res = "";
	if (table.head == nullptr) return res;
	int max_scope = table.tail->data.scope;
	IdentifierNode* p = table.tail;
	while (p) {
		if (p->data.scope == max_scope) { res = res + p->data.identifier_name + "//" + to_string(p->data.scope) + " "; }
		else if (p->data.scope < max_scope) {
			if (!checkExistInLargerScope(table, p->data.identifier_name, p->data.scope)) {
				res = res + p->data.identifier_name + "//" + to_string(p->data.scope) + " ";
			}
		}
		p = p->prev;
	}
	if(res == "") return res;
	int length = res.length();
	if (res[length - 1] == ' ') res.erase(res.end() - 1);
	return res;
}
void clearTable(SymbolTable& table) {
	if(table.head == nullptr) return;
	while (table.head) {
		IdentifierNode* del = table.head;
		table.head = table.head->next;
		delete del;
		del = nullptr;
	}
	table.tail = nullptr;
}
void SymbolTable::run(string filename)
{
	ifstream ifs(filename);
	string line, method;
	Identifier id;
	SymbolTable table;
	while(!ifs.eof()){
		getline(ifs, line);
		int numberOfComponent = countNumberOfComponent(line);
		tokenize(line, method, id, numberOfComponent);
		if(!checkInvalidInstruction(method, id, numberOfComponent)){
			clearTable(table);
			InvalidInstruction error(line);
			throw(error);
		}
		if (method == "INSERT") {
			if (isExist(table, id.identifier_name, method, id.scope)) {
				clearTable(table);
				Redeclared error(line);
				throw(error);
			}
			else{
				insert(table, id);
				cout << "success" << endl;
			}
		}
		else if (method == "ASSIGN") {
			if (!isExist(table, id.identifier_name, method, id.scope) 
				|| (checkVariableName(id.value) && !isExist(table, id.value, method, id.scope))){
				clearTable(table);
				Undeclared error(line);
				throw(error);
			}
			bool isStringType = checkStringType(id.value), isNumberType = checkNumberType(id.value);
			bool isVariableType = checkVariableName(id.value);
			if (isVariableType) {
				if (!checkTypeMismatchForVariable(table, id.identifier_name, id.value)) {
					clearTable(table);
					TypeMismatch error(line);
					throw(error);
				}
			}
			else{
				if (isStringType) id.type = "string";
				if (isNumberType) id.type = "number";
				if(!checkTypeMismatchForValue(table, id.identifier_name, id.type)){
					clearTable(table);
					TypeMismatch error(line);
					throw(error);
				}
			}
			cout << "success" << endl;
		}
		else if (method == "BEGIN") { id.scope += 1; }
		else if (method == "END") {
			int max_scope = id.scope;
			id.scope -= 1;
			if (id.scope < 0) {
				clearTable(table);
				UnknownBlock error;
				throw(error);
			}
			removeScope(table, max_scope);
		}
		else if (method == "LOOKUP") {
			if (lookup(table, id.identifier_name) == -1) {
				clearTable(table);
				Undeclared error(line);
				throw(error);
			}
			else {
				cout << lookup(table, id.identifier_name) << endl; 
			} 
		}
		else if(method == "PRINT"){
			string res = printScope(table);
			if(res != "") { cout << res << endl; } 
		}
		else if(method == "RPRINT"){
			string res = rprintScope(table);
			if(res != ""){ cout << res << endl; } 
		}
		else{
			clearTable(table);
			InvalidInstruction error(line);
			throw(error);
		}
	}
	if (id.scope > 0) {
		clearTable(table);
		UnclosedBlock error(id.scope);
		throw(error);
	}
	clearTable(table);
	ifs.close();
}