#include "SymbolTable.h"

int compareNode(Identifier a, Identifier b) {
	if (a.level > b.level) return 1;
	else if (a.level < b.level) return -1;
	else {
		return a.identifier_name.compare(b.identifier_name);
	}
}
int countNumberOfComponent(string s) {
	int length = s.length(), count = 0;
	bool flag = true;
	for (int i = 0; i < length; i++) {
		if ((int)s[i] == 39) flag = false;
		if (s[i] == ' ' && flag) count++;
	}
	return count + 1;
}
int countParameter(string s) {
	if (s == "()") return 0;
	int length = s.length(), count = 0;
	for(int i = 0; i < length; i++) {
		if(s[i] == ',') count++;
	}
	return count + 1;
}
void separateFunction(string function, string& parameter, string& pointer, string& result) {
	int start = 0, end = 0, length = function.length();
	while (end < length) {
		if (function[end] == ')') {
			parameter = function.substr(start, end - start + 1);
			start = end + 1;
			break;
		}
		end++;
	}
	while (end < length) {
		if (function[end] == '>') {
			pointer = function.substr(start, end - start + 1);
			start = end + 1;
			break;
		}
		end++;
	}
	result = function.substr(start, length - start);
}
bool checkLowerCase(char c) {
	if (c >= 'a' && c <= 'z') return true;
	return false;
}
bool checkUpperCase(char c) {
	if (c >= 'A' && c <= 'Z') return true;
	return false;
}
bool checkNumeric(char c) {
	if (c >= '0' && c <= '9') return true;
	return false;
}
bool checkVariableName(string s) {
	if (!checkLowerCase(s[0]) || s == "") return false;
	int length = s.length();
	for (int i = 1; i < length; i++) {
		if (!checkLowerCase(s[i]) && !checkUpperCase(s[i]) && !checkNumeric(s[i]) && s[i] != '_') return false;
	}
	return true;
}
bool checkFunctionType(string function) {
	string parameter = "", pointer = "", result = "";
	separateFunction(function, parameter, pointer, result);
	int para_length = parameter.length();
	if (parameter[0] != '(' || parameter[para_length - 1] != ')') return false;
	if (pointer != "->") return false;
	if (result != "number" && result != "string") return false;
	if (parameter[0] == '(' && parameter[para_length - 1] == ')') {
		string curr_para = parameter.substr(1, para_length - 2);
		if (curr_para == "") return true;
		string key = ",", temp = "";
		int curr_length = curr_para.length(), key_length = key.length();
		int start = 0, end = 0;
		for (; end < curr_length; end++) {
			if (curr_para[end] == ',') {
				temp = curr_para.substr(start, end - start);
				if (temp != "number" && temp != "string") {
					return false;
				}
				start = end + key_length;
			}
		}
		temp = curr_para.substr(start, end - start);
		if (temp != "number" && temp != "string") {
			return false;
		}
	}
	return true;
}
bool checkStringType(string s) {
	if(s == "") return false;
	int length = s.length();
	if ((int)s[0] != 39 || (int)s[length - 1] != 39) return false;
	if ((int)s[0] == 39 && (int)s[length - 1] == 39) {
		for (int i = 1; i < length - 1; i++) {
			if (!checkLowerCase(s[i]) && !checkUpperCase(s[i]) && !checkNumeric(s[i]) && s[i] != ' ') return false;
		}
	}
	return true;
}
bool checkNumberType(string s) {
	if(s == "") return false;
	int length = s.length();
	for (int i = 0; i < length; i++) {
		if (!checkNumeric(s[i])) return false;
	}
	return true;
}
bool checkValue(string s) {
	if (checkNumberType(s) || checkStringType(s) || checkVariableName(s)) return true;
	return false;
}
void tokenize(string s, string& method, Identifier& id, int numberOfComponent) {
	string key = " ";
	int start = 0, end = 0, s_length = s.length(), key_length = key.length();
	if (numberOfComponent == 1) {
		method = s.substr(start, s_length);
		id.identifier_name = "", id.type = "", id.parameter = "", id.exact_type = "", id.value = "", id.isGlobal = "";
	}
	else if (numberOfComponent == 2) {
		for (; end < s_length; end++) {
			if (s[end] == ' ') {
				method = s.substr(start, end - start);
				start = end + key_length;
			}
		}
		id.identifier_name = s.substr(start, end - start);
		id.type = "", id.parameter = "", id.exact_type = "", id.value = "", id.isGlobal = "";
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
				}
			}
		}
		id.value = s.substr(start, end - start);
		id.type = "", id.parameter = "", id.exact_type = "", id.isGlobal = "";
	}
	else if (numberOfComponent == 4) {
		bool flag1 = true, flag2 = false, flag3 = false, flag = true;
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
					flag2 = false;
					flag3 = true;
					start = end + key_length;
					continue;
				}
				if (flag3) {
					id.type = s.substr(start, end - start);
					if (checkFunctionType(id.type)) {
						string parameter = "", pointer = "", result = "";
						separateFunction(id.type, parameter, pointer, result);
						id.parameter = parameter;
						id.exact_type = result;
					}
					else {
						id.exact_type = id.type;
					}
					start = end + key_length;
				}
			}
		}
		id.isGlobal = s.substr(start, end - start);
		id.value = "";
	}
}
void seperateFunctionValue(string value, string& function_name, string& parameter) {
	int start = 0, end = 0, val_length = value.length();
	while (end < val_length) {
		if (value[end] == '(') {
			function_name = value.substr(start, end - start);
			start = end;
			break;
		}
		end++;
	}
	parameter = value.substr(start, val_length - start);
}
bool checkFunctionValue(string value) {
	string function_name = "", parameter_val = "";
	seperateFunctionValue(value, function_name, parameter_val);
	if (!checkVariableName(function_name)) return false;
	int para_length = parameter_val.length();
	if (parameter_val[0] != '(' || parameter_val[para_length - 1] != ')') return false;
	if (parameter_val[0] == '(' && parameter_val[para_length - 1] == ')') {
		string curr_para = parameter_val.substr(1, para_length - 2);
		if(curr_para == "") return true;
		string key = ",", temp = "";
		int curr_length = curr_para.length(), key_length = key.length();
		int start = 0, end = 0;
		for (; end < curr_length; end++) {
			if (curr_para[end] == ',') {
				temp = curr_para.substr(start, end - start);
				if (!checkNumberType(temp) && !checkStringType(temp) && !checkVariableName(temp)) return false;
				start = end + key_length;
			}
		}
		temp = curr_para.substr(start, end - start);
		if (!checkNumberType(temp) && !checkStringType(temp) && !checkVariableName(temp)) return false;
	}
	return true;
}
bool checkInvalidInstruction(string method, Identifier id, int numberOfComponent) {
	if (numberOfComponent > 4) return false;
	if (method != "INSERT" && method != "ASSIGN" && method != "BEGIN"
		&& method != "END" && method != "LOOKUP" && method != "PRINT") return false;
	if (method == "INSERT") {
		if (numberOfComponent != 4) return false;
		if (!checkVariableName(id.identifier_name)) return false;
		if (id.type != "number" && id.type != "string" && !checkFunctionType(id.type)) return false;
		if (id.isGlobal != "true" && id.isGlobal != "false") return false;
	}
	else if (method == "ASSIGN") {
		if (numberOfComponent != 3) return false;
		if (!checkVariableName(id.identifier_name)) return false;
		if (!checkFunctionValue(id.value) && !checkValue(id.value)) return false;
	}
	else if (method == "BEGIN" || method == "END" || method == "PRINT") {
		if (numberOfComponent != 1) return false;
	}
	else if (method == "LOOKUP") {
		if (numberOfComponent != 2) return false;
		if (!checkVariableName(id.identifier_name)) return false;
	}
	return true;
}
bool isFunctionName(SymbolTable table, Identifier id, string identifier_name) {
	id.identifier_name = identifier_name;
	while (id.level >= 0) {
		IdentifierNode* curr = table.root;
		while (curr) {
			if (curr->data.identifier_name == identifier_name) {
				return checkFunctionType(curr->data.type);
			}
			if (compareNode(id, curr->data) < 0) {
				curr = curr->left;
			}
			else if (compareNode(id, curr->data) > 0) {
				curr = curr->right;
			}
		}
		id.level -= 1;
	}
	return false;
}
bool isExist(SymbolTable table, string found, Identifier id, string method, int level) {
	id.identifier_name = found;
	while(id.level >= 0) {
		IdentifierNode* p = table.root;
		while (p) {
			if (p->data.identifier_name == found) {
				if (method == "ASSIGN") return true;
				if (method == "INSERT") return p->data.level == level;
			}
			if (compareNode(id, p->data) < 0) {
				p = p->left;
			}
			else if (compareNode(id, p->data) > 0) {
				p = p->right;
			}
		}
		id.level -= 1;	
	}
	return false;
}
void leftRotate(SymbolTable& table, IdentifierNode*& x) {
	IdentifierNode* y = x->right;
	IdentifierNode* z = y->left;
	IdentifierNode* p = x->parent;
	y->parent = p;
	if (p == nullptr) {
		table.root = y;
	}
	else if (p->left == x) {
		p->left = y;
	}
	else {
		p->right = y;
	}
	y->left = x;
	x->parent = y;
	x->right = z;
	if (z) z->parent = x;
}
void rightRotate(SymbolTable& table, IdentifierNode*& x) {
	IdentifierNode* y = x->left;
	IdentifierNode* z = y->right;
	IdentifierNode* p = x->parent;
	y->parent = p;
	if (p == nullptr) {
		table.root = y;
	}
	else if (p->left == x) {
		p->left = y;
	}
	else {
		p->right = y;
	}
	y->right = x;
	x->parent = y;
	x->left = z;
	if (z) z->parent = x;
}
void Splay(SymbolTable& table, IdentifierNode*& n) {
	if (!n->parent || !n) return;
	while (n->parent) {
		IdentifierNode* p = n->parent;
		IdentifierNode* g = p->parent;
		if (g == nullptr) {
			if (p->left == n)
				rightRotate(table, p);
			else
				leftRotate(table, p);
		}
		else {
			if (g->left == p && p->left == n) {
				rightRotate(table, g);
				rightRotate(table, p);
			}
			else if (g->right == p && p->right == n) {
				leftRotate(table, g);
				leftRotate(table, p);
			}
			else if (g->left == p && p->right == n) {
				leftRotate(table, p);
				rightRotate(table, g);
			}
			else {
				rightRotate(table, p);
				leftRotate(table, g);
			}
		}
	}
}
void insert(SymbolTable& table, Identifier id, int& num_comp, int& num_splay) {
	IdentifierNode* newNode = new IdentifierNode(id);
	if (!table.root) {
		table.root = newNode;
		num_comp = 0;
		num_splay = 0;
		return;
	}
	IdentifierNode* curr = table.root;
	IdentifierNode* prev = nullptr;
	while (curr) {
		prev = curr;
		if (compareNode(id, curr->data) < 0) {
			curr = curr->left;
			num_comp++;
		}
		else if (compareNode(id, curr->data) > 0) {
			curr = curr->right;
			num_comp++;
		}
		else {
			return;
		}
	}
	if (compareNode(id, prev->data) < 0) {
		prev->left = newNode;
		newNode->parent = prev;
	}
	else if (compareNode(id, prev->data) > 0) {
		prev->right = newNode;
		newNode->parent = prev;
	}
	Splay(table, newNode);
	num_splay += 1;
}
int lookup(SymbolTable& table, string found, Identifier id, int& num_comp, int& num_splay) {
	id.identifier_name = found;
	int temp_comp = num_comp, temp_splay = num_splay;
	while (id.level >= 0) {
		IdentifierNode* curr = table.root;
		while (curr) {
			if (curr->data.identifier_name == found) {
				num_comp++;
				if(curr != table.root) {
					Splay(table, curr);
					num_splay += 1;
				}
				return curr->data.level;
			}
			if (compareNode(id, curr->data) < 0) {
				curr = curr->left;
				num_comp++;
			}
			else if (compareNode(id, curr->data) > 0) {
				curr = curr->right;
				num_comp++;
			}
		}
		id.level -= 1;
		num_comp = temp_comp, num_splay = temp_splay;
	}
	return -1;
}
string getTypeOf(SymbolTable table, string s, Identifier id) {
	if (checkNumberType(s)) return "number";
	else if (checkStringType(s)) return "string";
	else if (checkVariableName(s)) {
		id.identifier_name = s;
		while(id.level >= 0) {
			IdentifierNode* curr = table.root;
			while (curr) {
				if (curr->data.identifier_name == s) {
					return curr->data.exact_type;
				}
				if (compareNode(id, curr->data) < 0) {
					curr = curr->left;
				}
				else if (compareNode(id, curr->data) > 0) {
					curr = curr->right;
				}
			}
			id.level -= 1;
		}
	}
	else if (checkFunctionValue(s)) {
		string function_name = "", parameter = "";
		seperateFunctionValue(s, function_name, parameter);
		id.identifier_name = function_name;
		while(id.level >= 0){
			IdentifierNode* curr = table.root;
			while (curr) {
				if (curr->data.identifier_name == function_name) {
					return curr->data.exact_type;
				}
				if (compareNode(id, curr->data) < 0) {
					curr = curr->left;
				}
				else if (compareNode(id, curr->data) > 0) {
					curr = curr->right;
				}
			}
			id.level -= 1;
		}
	}
	return "";
}
bool checkNumberOfParameter(SymbolTable table, Identifier id, string function_name, string value){
	int numberOfValueParamater = countParameter(value);
	id.identifier_name = function_name;
	while(id.level >= 0) {
		IdentifierNode* curr = table.root;
		while (curr) {
			if (curr->data.identifier_name == function_name) {
				int numberOfVariableParameter = countParameter(curr->data.parameter);
				return numberOfVariableParameter == numberOfValueParamater;
			}
			if (compareNode(id, curr->data) < 0) {
				curr = curr->left;
			}
			else if (compareNode(id, curr->data) > 0) {
				curr = curr->right;
			}
		}
		id.level -= 1;
	}
	return false;
}
string getParameterString(SymbolTable table, string function_name, Identifier id) {
	id.identifier_name = function_name;
	while (id.level >= 0) {
		IdentifierNode* curr = table.root;
		while (curr) {
			if (curr->data.identifier_name == function_name) {
				int length = curr->data.parameter.length();
				string curr_para = curr->data.parameter.substr(1, length - 2);
				length = curr_para.length();
				string res = "";
				for (int i = 0; i < length; i++) {
					if (curr_para[i] != ',') {
						res += curr_para[i];
					}
				}
				return res;
			}
			if (compareNode(id, curr->data) < 0) {
				curr = curr->left;
			}
			else if (compareNode(id, curr->data) > 0) {
				curr = curr->right;
			}
		}
		id.level -= 1;
	}
	return "";
}
void assign(SymbolTable& table, Identifier id, int& num_comp, int& num_splay) {
	string temp = "", function_name = "", parameter = ""; 
	int found_identifiername = -1, found_value = -1, found_parameter = -1;
	if (checkFunctionValue(id.value)) {
		seperateFunctionValue(id.value, function_name, parameter);
		id.value = function_name;
		int length = parameter.length();
		string temp_para = parameter.substr(1, length - 2);
		parameter = temp_para;
	}
	if(checkVariableName(id.value)) {
		found_value = lookup(table, id.value, id, num_comp, num_splay);
	}
	if(parameter != "") {
		int length = parameter.length();
		int start = 0, end = 0;
		for(; end < length; end++){
			if(parameter[end] == ','){
				temp = parameter.substr(start, end - start);
				if(checkVariableName(temp)){
					found_parameter = lookup(table, temp, id, num_comp, num_splay);
				}
				start = end + 1;
			}
		}
		temp = parameter.substr(start, end - start);
		if(checkVariableName(temp)){
			found_parameter = lookup(table, temp, id, num_comp, num_splay);
		}
	}
	found_identifiername = lookup(table, id.identifier_name, id, num_comp, num_splay);
	if (found_identifiername != -1 || found_value != -1 || found_parameter != -1) {
		IdentifierNode* curr = table.root;
		while (curr) {
			if (curr->data.identifier_name == id.identifier_name) {
				curr->data.value = id.value;
				return;
			}
			if (compareNode(id, curr->data) < 0) {
				curr = curr->left;
			}
			else if (compareNode(id, curr->data) > 0) {
				curr = curr->right;
			}
		}
	}
}
void removeNode(SymbolTable& table, Identifier id) {
	if (!table.root) return;
	int num_comp = 0, num_splay = 0;
	int found_id = lookup(table, id.identifier_name, id, num_comp, num_splay);
	if (id.identifier_name != table.root->data.identifier_name || found_id <= 0) return;
	SymbolTable leftSubTree;
	SymbolTable rightSubTree;
	IdentifierNode* temp = table.root;
	leftSubTree.root = table.root->left;
	rightSubTree.root = table.root->right;
	if (leftSubTree.root) {
		leftSubTree.root->parent = nullptr;
	}
	if (rightSubTree.root) {
		rightSubTree.root->parent = nullptr;
	}
	delete temp;
	temp = nullptr;
	if(!leftSubTree.root){
		table.root = rightSubTree.root;
	}
	if(!rightSubTree.root){
		table.root = leftSubTree.root;
	}
	if (leftSubTree.root && rightSubTree.root) {
		IdentifierNode* maxRoot = leftSubTree.root;
		while (maxRoot->right) {
			maxRoot = maxRoot->right;
		}
		Splay(leftSubTree, maxRoot);
		leftSubTree.root->right = rightSubTree.root;
		if (rightSubTree.root) rightSubTree.root->parent = leftSubTree.root;
		table.root = leftSubTree.root;
	}
}
void removeScope(SymbolTable& table, DataBlocked blocked, int max_level) {
	DataNode* temp = blocked.head;
	while (temp) {
		if (temp->data.level == max_level) {
			removeNode(table, temp->data);
		}
		temp = temp->next;
	}
}
void preOrder(IdentifierNode* root, string& res) {
	if (root) {
		res += root->data.identifier_name + "//" + to_string(root->data.level) + " ";
		preOrder(root->left, res);
		preOrder(root->right, res);
	}
}
void clearTable(IdentifierNode*& root) {
	while (root) {
		clearTable(root->left);
		clearTable(root->right);
		delete root;
		root = nullptr;
	}
}
void SymbolTable::run(string filename)
{
	ifstream ifs(filename);
	string line, method;
	Identifier id;
	SymbolTable table;
	DataBlocked blocked;
	int scope_level = 0;
	while (!ifs.eof()) {
		getline(ifs, line);
		int numberOfComponent = countNumberOfComponent(line);
		tokenize(line, method, id, numberOfComponent);
		if (id.isGlobal == "true") {
			id.level = 0;
		}
		else {
			id.level = scope_level;
		}
		if (!checkInvalidInstruction(method, id, numberOfComponent)) {
			clearTable(table.root);
			blocked.clear();
			InvalidInstruction error(line);
			throw(error);
		}
		if (method == "INSERT") {
			int num_comp = 0, num_splay = 0;
			if (checkFunctionType(id.type) && id.level > 0) {
				clearTable(table.root);
				blocked.clear();
				InvalidDeclaration error(line);
				throw(error);
			}
			if (isExist(table, id.identifier_name, id, method, id.level)) {
				clearTable(table.root);
				blocked.clear();
				Redeclared error(line);
				throw(error);
			}
			insert(table, id, num_comp, num_splay);
			cout << num_comp << " " << num_splay << endl;
			if (id.level > 0) blocked.add(id);
		}
		else if (method == "ASSIGN") {
			string function_name = "", parameter_val = "", parameter_string = "";
			int num_comp = 0, num_splay = 0;
			if (checkFunctionValue(id.value)) {
				seperateFunctionValue(id.value, function_name, parameter_val);
				parameter_string = getParameterString(table, function_name, id);
			}
			if ((checkVariableName(id.value) && !isExist(table, id.value, id, method, id.level))
				|| (checkFunctionValue(id.value) && !isExist(table, function_name, id, method, id.level))) {
				clearTable(table.root);
				blocked.clear();
				Undeclared error(line);
				throw(error);
			}
			if(checkFunctionValue(id.value) && !checkNumberOfParameter(table, id, function_name, parameter_val)){
				clearTable(table.root);
				blocked.clear();
				TypeMismatch error(line);
				throw(error);
			}
			if(parameter_val != "" && parameter_string != "") {
				int length = parameter_val.length();
				string new_parameter_val = parameter_val.substr(1, length - 2);
				length = new_parameter_val.length();
				string res = "", temp = "", temp_check = "", type_of_para = "";
				int start = 0, end = 0, start_index = 0;
				for (; end < length; end++) {
					if (new_parameter_val[end] == ',') {
						temp = new_parameter_val.substr(start, end - start);
						temp_check = parameter_string.substr(start_index, 6);
						start_index += 6;
						if(checkVariableName(temp) && !isExist(table, temp, id, method, id.level)) {
							clearTable(table.root);
							blocked.clear();
							Undeclared error(line);
							throw(error);
						}
						type_of_para = getTypeOf(table, temp, id);
						if(isFunctionName(table, id, temp) || type_of_para != temp_check){
							clearTable(table.root);
							blocked.clear();
							TypeMismatch error(line);
							throw(error);
						}
						start = end + 1;
					}
				}
				temp = new_parameter_val.substr(start, end - start);
				type_of_para = getTypeOf(table, temp, id);
				temp_check = parameter_string.substr(start_index, 6);
				if(checkVariableName(temp) && !isExist(table, temp, id, method, id.level)) {
					clearTable(table.root);
					blocked.clear();
					Undeclared error(line);
					throw(error);
				}
				if(isFunctionName(table, id, temp) || type_of_para != temp_check){
					clearTable(table.root);
					blocked.clear();
					TypeMismatch error(line);
					throw(error);
				}
			}
			if (!isExist(table, id.identifier_name, id, method, id.level)) {
				clearTable(table.root);
				blocked.clear();
				Undeclared error(line);
				throw(error);
			}
			if (isFunctionName(table, id, id.identifier_name)) {
				clearTable(table.root);
				blocked.clear();
				TypeMismatch error(line);
				throw(error);
			}
			string identifier_type = getTypeOf(table, id.identifier_name, id);
			string value_type = getTypeOf(table, id.value, id);
			if (identifier_type != value_type) {
				clearTable(table.root);
				blocked.clear();
				TypeMismatch error(line);
				throw(error);
			}
			assign(table, id, num_comp, num_splay);
			cout << num_comp << " " << num_splay << endl;
		}
		else if (method == "BEGIN") {
			scope_level += 1;
		}
		else if (method == "END") {
			int max_level = scope_level;
			scope_level -= 1;
			id.level = scope_level;
			if (scope_level < 0) {
				clearTable(table.root);
				blocked.clear();
				UnknownBlock error;
				throw(error);
			}
			removeScope(table, blocked, max_level);
		}
		else if (method == "LOOKUP") {
			int num_comp = 0, num_splay = 0;
			if (lookup(table, id.identifier_name, id, num_comp, num_splay) == -1) {
				clearTable(table.root);
				blocked.clear();
				Undeclared error(line);
				throw(error);
			}
			else {
				cout << lookup(table, id.identifier_name, id, num_comp, num_splay) << endl;
			}
		}
		else if (method == "PRINT") {
			string res = "";
			preOrder(table.root, res);
			if (res != "") {
				int length = res.length();
				if (res[length - 1] == ' ') res.erase(res.end() - 1);
				cout << res << endl;
			}
		}
	}
	if (scope_level > 0) {
		clearTable(table.root);
		blocked.clear();
		UnclosedBlock error(scope_level);
		throw(error);
	}
	clearTable(table.root);
	blocked.clear();
	ifs.close();
}