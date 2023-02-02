#include "SymbolTable.h"

string decode(Identifier id) {
	string id_level = to_string(id.level);
	string res = "";
	res += id_level;
	int length = id.identifier_name.length();
	for (int i = 0; i < length; i++) {
		int temp = (int)id.identifier_name[i] - 48;
		res += to_string(temp);
	}
	return res;
}
int countParameter(string s) {
	if (s == "()") return 0;
	int length = s.length(), count = 0;
	for (int i = 0; i < length; i++) {
		if (s[i] == ',') count++;
	}
	return count + 1;
}
void initializeTable(SymbolTable& table, int size, int (*hp)(string, int, Constant)) {
	table.size = size;
	table.hp = hp;
	table.data = new Identifier[table.size];
	table.status = new bool[table.size];
	for (int i = 0; i < table.size; i++) {
		table.data[i] = Identifier();
		table.status[i] = false;
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
bool checkStringType(string s) {
	if (s == "") return false;
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
	if (s == "") return false;
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
		if (curr_para == "") return true;
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
bool checkValidConstant(string s) {
	int length = s.length();
	if (length > 6 || s == "") return false;
	for (int i = 0; i < length; i++) {
		if (!checkNumeric(s[i])) return false;
	}
	return true;
}
void initializeParaType(Identifier& id){
	int numPara = stoi(id.numOfParameter);
	if (numPara > 0) {
		id.paraType = new Parameter[numPara];
		for (int i = 0; i < numPara; i++) {
			id.paraType[i].para = "";
		}
	}
}
void tokenize(string s, string& method, Identifier& id, Constant& cont, int numberOfComponent) {
	string key = " ";
	int start = 0, end = 0, s_length = s.length(), key_length = key.length();
	if (numberOfComponent == 1) {
		method = s.substr(start, s_length);
		id.identifier_name = "", id.type = "unknow", id.parameter = "", id.value = "", id.numOfParameter = "-1", id.paraType = nullptr;
	}
	else if (numberOfComponent == 2) {
		bool flag = true;
		for (; end < s_length; end++) {
			if ((int)s[end] == 39) flag = false;
			if (s[end] == ' ' && flag) {
				method = s.substr(start, end - start);
				start = end + key_length;
			}
		}
		id.identifier_name = s.substr(start, end - start);
		id.type = "unknow", id.parameter = "", id.value = "", id.numOfParameter = "-1", id.paraType = nullptr;
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
					if (method == "LINEAR" || method == "DOUBLE") {
						cont.m_string = s.substr(start, end - start);
						if (checkValidConstant(cont.m_string)) cont.m = stoi(cont.m_string);
					}
					else {
						id.identifier_name = s.substr(start, end - start);
					}
					start = end + key_length;
				}
			}
		}
		id.type = "unknow", id.parameter = "", id.value = "", id.numOfParameter = "-1", id.paraType = nullptr;
		if (method == "LINEAR" || method == "DOUBLE") {
			cont.c_string = s.substr(start, end - start);
			if (checkValidConstant(cont.c_string)) cont.c = stoi(cont.c_string);
		}
		if (method == "INSERT") {
			id.numOfParameter = s.substr(start, end - start);
		}
		if (method == "ASSIGN") {
			id.value = s.substr(start, end - start);
		}
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
					cont.m_string = s.substr(start, end - start);
					if (checkValidConstant(cont.m_string)) cont.m = stoi(cont.m_string);
					flag2 = false;
					flag3 = true;
					start = end + key_length;
					continue;
				}
				if (flag3) {
					cont.c1_string = s.substr(start, end - start);
					if (checkValidConstant(cont.c1_string)) cont.c1 = stoi(cont.c1_string);
					start = end + key_length;
				}
			}
		}
		cont.c2_string = s.substr(start, end - start);
		if (checkValidConstant(cont.c2_string)) cont.c2 = stoi(cont.c2_string);
	}
}
bool isEqual(Identifier id1, Identifier id2) {
	return id1.identifier_name == id2.identifier_name && id1.level == id2.level;
}
int lookup(SymbolTable table, string found, Identifier id, Constant cont) {
	id.identifier_name = found;
	while (id.level >= 0) {
		int i = 0;
		while (i < table.size) {
			string key = decode(id);
			int slot = table.hp(key, i, cont);
			if (isEqual(table.data[slot], id)) {
				return slot;
			}
			i++;
		}
		id.level -= 1;
	}
	return -1;
}
bool isFunctionName(SymbolTable table, Identifier id, Constant cont, string identifier_name) {
	if(!checkVariableName(identifier_name)) return false;
	id.identifier_name = identifier_name;
	int found = lookup(table, identifier_name, id, cont);
	if(found == -1) return false;
	if (table.data[found].numOfParameter != "-1") return true;
	return false;
}
bool checkKeyWord(string s) {
	if(s == "number" || s == "string" || s == "void") return true;
	return false;
}
bool checkInvalidInstruction(SymbolTable table, string method, Identifier id, Constant cont, int numberOfComponent) {
	if (numberOfComponent > 4) return false;
	if (method != "LINEAR" && method != "QUADRATIC" && method != "DOUBLE"
		&& method != "INSERT" && method != "ASSIGN" && method != "CALL"
		&& method != "BEGIN" && method != "END" && method != "LOOKUP" && method != "PRINT") {
		return false;
	}
	if (method == "LINEAR" || method == "DOUBLE") {
		if (numberOfComponent != 3) return false;
		if (!checkValidConstant(cont.m_string) || !checkValidConstant(cont.c_string)) return false;
		return true;
	}
	else if (method == "QUADRATIC") {
		if (numberOfComponent != 4) return false;
		if (cont.m_string == "0") return false;
		if (!checkValidConstant(cont.m_string) || !checkValidConstant(cont.c1_string) || !checkValidConstant(cont.c2_string)) return false;
		return true;
	}
	else if (method == "INSERT") {
		if (numberOfComponent != 2 && numberOfComponent != 3) return false;
		if (!checkVariableName(id.identifier_name)) return false;
		if (id.numOfParameter != "-1" && !checkNumberType(id.numOfParameter)) return false;
		if(checkKeyWord(id.identifier_name)) return false;
	}
	else if (method == "ASSIGN") {
		if (numberOfComponent != 3) return false;
		if (!checkVariableName(id.identifier_name)) return false;
		if (!checkFunctionValue(id.value) && !checkValue(id.value)) return false;
		if (checkVariableName(id.value) && isFunctionName(table, id, cont, id.value)) return false;
		if (checkKeyWord(id.identifier_name) || checkKeyWord(id.value)) return false;
	}
	else if (method == "BEGIN" || method == "END" || method == "PRINT") {
		if (numberOfComponent != 1) return false;
	}
	else if (method == "LOOKUP") {
		if (numberOfComponent != 2) return false;
		if (!checkVariableName(id.identifier_name)) return false;
		if(checkKeyWord(id.identifier_name)) return false;
	}
	else if (method == "CALL") {
		if (numberOfComponent != 2) return false;
		if (!checkFunctionValue(id.identifier_name)) return false;
		if (checkVariableName(id.value) && isFunctionName(table, id, cont, id.value)) return false;
		if (checkKeyWord(id.identifier_name) || checkKeyWord(id.value)) return false;
	} 
	return true;
}
int mod(string key, int m) {
	long long int data = stoll(key, nullptr, 10);
	int res = data % (long long int)m;
	if (res < 0) res += m;
	return res;
}
int linearProbing(string key, int i, Constant cont) {
	int h = mod(key, cont.m);
	return (h + cont.c * i) % cont.m;
}
int quadraticProbing(string key, int i, Constant cont) {
	int h = mod(key, cont.m);
	return (h + cont.c1 * i + cont.c2 * i * i) % cont.m;
}
int doubleHashing(string key, int i, Constant cont) {
	int h1 = mod(key, cont.m);
	int h2 = 1 + mod(key, cont.m - 2);
	return (h1 + cont.c * i * h2) % cont.m;
}
int insert(SymbolTable& table, Identifier id, Constant cont) {
	initializeParaType(id);
	int i = 0;
	while (i < table.size) {
		string key = decode(id);
		int slot = table.hp(key, i, cont);
		if (table.status[slot] == false) {
			table.data[slot] = id;
			table.status[slot] = true;
			return i;
		}
		i++;
	}
	return i;
}
bool isExist(SymbolTable table, string found, string method, Identifier id, Constant cont) {
	id.identifier_name = found;
	while (id.level >= 0) {
		int i = 0;
		while (i < table.size) {
			string key = decode(id);
			int slot = table.hp(key, i, cont);
			if (table.data[slot].identifier_name == id.identifier_name) {
				if (method == "ASSIGN" || method == "CALL") return true;
				if (method == "INSERT") return table.data[slot].level == id.level;
			}
			i++;
		}
		id.level -= 1;
	}
	return false;
}
int numOfFoundIndex(SymbolTable table, string found, Identifier id, Constant cont) {
	id.identifier_name = found;
	while (id.level >= 0) {
		int i = 0;
		while (i < table.size) {
			string key = decode(id);
			int slot = table.hp(key, i, cont);
			if (isEqual(table.data[slot], id)) {
				return i;
			}
			i++;
		}
		id.level -= 1;
	}
	return -1;
}
string getTypeOf(SymbolTable table, string s, Identifier id, Constant cont) {
	if (checkNumberType(s)) return "number";
	else if (checkStringType(s)) return "string";
	else if (checkVariableName(s)) {
		int found = lookup(table, s, id, cont);
		if(found != -1) return table.data[found].type;
	}
	else if (checkFunctionValue(s)) {
		string function_name = "", parameter = "";
		seperateFunctionValue(s, function_name, parameter);
		int found = lookup(table, function_name, id, cont);
		if(found != -1) return table.data[found].type;
	}
	return "unknow";
}
bool checkNumberOfParameter(SymbolTable table, Identifier id, Constant cont, string function_name, string value) {
	int numberOfValueParamater = countParameter(value);
	id.identifier_name = function_name;
	int found = lookup(table, function_name, id, cont);
	if(found == -1) return false;
	return stoi(table.data[found].numOfParameter) == numberOfValueParamater;
}
string getStringParameter(SymbolTable table, string parameter_val, Identifier id, Constant cont) {
	string res = "";
	int length = parameter_val.length();
	int start = 0, end = 0;
	for (; end < length; end++) {
		if (parameter_val[end] == ',') {
			res += getTypeOf(table, parameter_val.substr(start, end - start), id, cont);
			start = end + 1;
		}
	}
	res += getTypeOf(table, parameter_val.substr(start, end - start), id, cont);
	return res;
}
int assign(SymbolTable& table, Identifier id, Constant cont) {
	int num_found = 0;
	int found = lookup(table, id.identifier_name, id, cont);
	table.data[found].value = id.value;
	num_found += numOfFoundIndex(table, id.identifier_name, id, cont);
	if (checkVariableName(id.value)) {
		num_found += numOfFoundIndex(table, id.value, id, cont);
	}
	if (checkFunctionValue(id.value)) {
		string function_name = "", parameter_val = "";
		seperateFunctionValue(id.value, function_name, parameter_val);
		num_found += numOfFoundIndex(table, function_name, id, cont);
		int length = parameter_val.length();
		parameter_val = parameter_val.substr(1, length - 2);
		length = parameter_val.length();
		int start = 0, end = 0;
		for (; end < length; end++) {
			if (parameter_val[end] == ',') {
				string temp = parameter_val.substr(start, end - start);
				if (checkVariableName(temp)) {
					num_found += numOfFoundIndex(table, temp, id, cont);
				}
				start = end + 1;
			}
		}
		string temp = parameter_val.substr(start, end - start);
		if (checkVariableName(temp)) {
			num_found += numOfFoundIndex(table, temp, id, cont);
		}
	}
	return num_found;
}
void remove(SymbolTable& table, Identifier id, Constant cont) {
	int i = 0;
	while (i < table.size) {
		string key = decode(id);
		int slot = table.hp(key, i, cont);
		if (isEqual(table.data[slot], id)) {
			table.status[slot] = false;
			table.data[slot] = Identifier();
			return;
		}
		i++;
	}
}
void removeScope(SymbolTable& table, DataBlocked block, Constant cont, int max_level) {
	DataNode* curr = block.head;
	while (curr) {
		if (curr->data.level == max_level) {
			remove(table, curr->data, cont);
		}
		curr = curr->next;
	}
}
string printTable(SymbolTable table) {
	string res = "";
	if (table.size == 0) return res;
	int size = table.size;
	for (int i = 0; i < size; i++) {
		if (table.status[i]) {
			res += to_string(i) + " " + table.data[i].identifier_name + "//" + to_string(table.data[i].level) + ";";
		}
	}
	if (res == "") return res;
	int length = res.length();
	if (res[length - 1] == ';') res.erase(res.end() - 1);
	return res;
}
void clearTable(SymbolTable& table) {
	for(int i = 0; i < table.size; i++){
		table.data[i].clear();
	}
	table.hp = nullptr;
	delete[] table.data;
	table.data = nullptr;
	delete[] table.status;
	table.status = nullptr;
	table.size = 0;
}
void SymbolTable::run(string filename) {
	ifstream ifs(filename);
	string line, method;
	Identifier id;
	SymbolTable table;
	Constant cont;
	DataBlocked block;
	bool flag = true;
	while (!ifs.eof()) {
		getline(ifs, line);
		int numberOfComponent = countNumberOfComponent(line);
		tokenize(line, method, id, cont, numberOfComponent);
		if (!checkInvalidInstruction(table, method, id, cont, numberOfComponent)) {
			clearTable(table);
			block.clear();
			InvalidInstruction error(line);
			throw(error);
		}
		if (method == "LINEAR") {
			initializeTable(table, cont.m, linearProbing);
			if (!flag) {
				clearTable(table);
				block.clear();
				InvalidInstruction error(line);
				throw(error);
			}
		}
		else if (method == "QUADRATIC") {
			initializeTable(table, cont.m, quadraticProbing);
			if (!flag) {
				clearTable(table);
				block.clear();
				InvalidInstruction error(line);
				throw(error);
			}
		}
		else if (method == "DOUBLE") {
			initializeTable(table, cont.m, doubleHashing);
			if (!flag) {
				clearTable(table);
				block.clear();
				InvalidInstruction error(line);
				throw(error);
			}
		}
		if (table.hp == nullptr) {
			clearTable(table);
			block.clear();
			InvalidInstruction error(line);
			throw(error);
		}
		if (method == "INSERT") {
			if (id.numOfParameter != "-1" && id.level != 0) {
				clearTable(table);
				block.clear();
				InvalidDeclaration error(id.identifier_name);
				throw(error);
			}
			if (isExist(table, id.identifier_name, method, id, cont)) {
				clearTable(table);
				block.clear();
				Redeclared error(id.identifier_name);
				throw(error);
			}
			if (table.size == 0) {
				clearTable(table);
				block.clear();
				Overflow error(line);
				throw(error);
			}
			int res = insert(table, id, cont);
			if (res == table.size) {
				clearTable(table);
				block.clear();
				Overflow error(line);
				throw(error);
			}
			cout << res << endl;
			if (id.level > 0) block.add(id);
		}
		else if (method == "ASSIGN") {
			string function_name = "", parameter_val = "", parameter_string = "";
			if (checkFunctionValue(id.value)) {
				seperateFunctionValue(id.value, function_name, parameter_val);
			}
			if ((checkVariableName(id.value) && !isExist(table, id.value, method, id, cont))) {
				clearTable(table);
				block.clear();
				Undeclared error(id.value);
				throw(error);
			}
			if((checkFunctionValue(id.value) && !isExist(table, function_name, method, id, cont))){
				clearTable(table);
				block.clear();
				Undeclared error(function_name);
				throw(error);
			}
			if (checkFunctionValue(id.value) && !checkNumberOfParameter(table, id, cont, function_name, parameter_val)) {
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			if(checkVariableName(id.value) && isFunctionName(table, id, cont, id.value)){
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			if (parameter_val != "" && function_name != "") {
				int found = lookup(table, function_name, id, cont);
				int numOfPara = stoi(table.data[found].numOfParameter);
				int length = parameter_val.length();
				parameter_val = parameter_val.substr(1, length - 2);
				length = parameter_val.length();
				string parameterString = getStringParameter(table, parameter_val, id, cont);
				int start = 0;
				for (int i = 0; i < numOfPara; i++) {
					if (table.data[found].paraType[i].para == "") {
						table.data[found].paraType[i].para = parameterString.substr(start, 6);
						start += 6;
					}
				}
				start = 0;
				int end = 0, curr = 0;
				for (; end < length; end++) {
					if (parameter_val[end] == ',') {
						string temp = parameter_val.substr(start, end - start);
						if (checkVariableName(temp) && !isExist(table, temp, method, id, cont)) {
							clearTable(table);
							block.clear();
							Undeclared error(temp);
							throw(error);
						}
						if(isFunctionName(table, id, cont, temp)){
							clearTable(table);
							block.clear();
							TypeMismatch error(line);
							throw(error);
						}
						string temp_type = getTypeOf(table, temp, id, cont);
						if (temp_type == "unknow" && table.data[found].paraType[curr].para == "unknow") {
							clearTable(table);
							block.clear();
							TypeCannotBeInferred error(line);
							throw(error);
						}
						if(checkVariableName(temp) && temp_type == "unknow" && table.data[found].paraType[curr].para != "unknow") {
							temp_type = table.data[found].paraType[curr].para;
							int found_temp_type = lookup(table, temp, id, cont);
							table.data[found_temp_type].type = table.data[found].paraType[curr].para;
						}
						if (temp_type != table.data[found].paraType[curr].para) {
							clearTable(table);
							block.clear();
							TypeMismatch error(line);
							throw(error);
						}
						start = end + 1;
						curr++;
					}
				}
				string temp = parameter_val.substr(start, end - start);
				if (checkVariableName(temp) && !isExist(table, temp, method, id, cont)) {
					clearTable(table);
					block.clear();
					Undeclared error(temp);
					throw(error);
				}
				if(isFunctionName(table, id, cont, temp)){
					clearTable(table);
					block.clear();
					TypeMismatch error(line);
					throw(error);
				}
				string temp_type = getTypeOf(table, temp, id, cont);
				if (temp_type == "unknow" && table.data[found].paraType[curr].para == "unknow") {
					clearTable(table);
					block.clear();
					TypeCannotBeInferred error(line);
					throw(error);
				}
				if(checkVariableName(temp) && temp_type == "unknow" && table.data[found].paraType[curr].para != "unknow") {
					temp_type = table.data[found].paraType[curr].para;
					int found_temp_type = lookup(table, temp, id, cont);
					table.data[found_temp_type].type = table.data[found].paraType[curr].para;
				}
				if (temp_type != table.data[found].paraType[curr].para) {
					clearTable(table);
					block.clear();
					TypeMismatch error(line);
					throw(error);
				}
			}
			if(checkFunctionValue(id.value) && getTypeOf(table, id.value, id, cont) == "voided") {
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			if (!isExist(table, id.identifier_name, method, id, cont)) {
				clearTable(table);
				block.clear();
				Undeclared error(id.identifier_name);
				throw(error);
			}
			if (isFunctionName(table, id, cont, id.identifier_name)) {
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			string value_type = getTypeOf(table, id.value, id, cont);
			string identifier_type = getTypeOf(table, id.identifier_name, id, cont);
			if (value_type != "unknow" && value_type != "voided" && identifier_type == "unknow") {
				int found = lookup(table, id.identifier_name, id, cont);
				table.data[found].type = value_type;
				identifier_type = value_type;
			}
			if (identifier_type != "unknow" && value_type == "unknow") {
				int found = -1;
				if (checkVariableName(id.value)) {
					found = lookup(table, id.value, id, cont);
				}
				if (checkFunctionValue(id.value)) {
					found = lookup(table, function_name, id, cont);
				}
				table.data[found].type = identifier_type;
				value_type = identifier_type;
			}
			if (identifier_type == "unknow" && value_type == "unknow") {
				clearTable(table);
				block.clear();
				TypeCannotBeInferred error(line);
				throw(error);
			}
			if (identifier_type != value_type) {
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			cout << assign(table, id, cont) << endl;
		}
		else if (method == "CALL") {
			string function_name = "", parameter_val = "";
			seperateFunctionValue(id.identifier_name, function_name, parameter_val);
			if (!isExist(table, function_name, method, id, cont)) {
				clearTable(table);
				block.clear();
				Undeclared error(function_name);
				throw(error);
			}
			if (!isFunctionName(table, id, cont, function_name)) {
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			int found = lookup(table, function_name, id, cont);
			if(table.data[found].type == "unknow"){
				table.data[found].type = "voided";
			}
			int numOfPara = stoi(table.data[found].numOfParameter);
			int length = parameter_val.length();
			parameter_val = parameter_val.substr(1, length - 2);
			length = parameter_val.length();
			string parameterString = getStringParameter(table, parameter_val, id, cont);
			int start = 0;
			for (int i = 0; i < numOfPara; i++) {
				if (table.data[found].paraType[i].para == "") {
					table.data[found].paraType[i].para = parameterString.substr(start, 6);
					start += 6;
				}
			}
			start = 0;
			int end = 0, curr = 0;
			for (; end < length; end++) {
				if (parameter_val[end] == ',') {
					string temp = parameter_val.substr(start, end - start);
					if (checkVariableName(temp) && !isExist(table, temp, method, id, cont)) {
						clearTable(table);
						block.clear();
						Undeclared error(temp);
						throw(error);
					}
					if(isFunctionName(table, id, cont, temp)){
						clearTable(table);
						block.clear();
						TypeMismatch error(line);
						throw(error);
					}
					string temp_type = getTypeOf(table, temp, id, cont);
					if (temp_type == "unknow" && table.data[found].paraType[curr].para == "unknow") {
						clearTable(table);
						block.clear();
						TypeCannotBeInferred error(line);
						throw(error);
					}
					if(checkVariableName(temp) && temp_type == "unknow" && table.data[found].paraType[curr].para != "unknow") {
						temp_type = table.data[found].paraType[curr].para;
						int found_temp_type = lookup(table, temp, id, cont);
						table.data[found_temp_type].type = table.data[found].paraType[curr].para;
					}
					if (temp_type != table.data[found].paraType[curr].para) {
						clearTable(table);
						block.clear();
						TypeMismatch error(line);
						throw(error);
					}
					start = end + 1;
					curr++;
				}
			}
			string temp = parameter_val.substr(start, end - start);
			if (checkVariableName(temp) && !isExist(table, temp, method, id, cont)) {
				clearTable(table);
				block.clear();
				Undeclared error(temp);
				throw(error);
			}
			if(isFunctionName(table, id, cont, temp)) {
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			string temp_type = getTypeOf(table, temp, id, cont);
			if (temp_type == "unknow" && table.data[found].paraType[curr].para == "unknow") {
				clearTable(table);
				block.clear();
				TypeCannotBeInferred error(line);
				throw(error);
			}
			if(checkVariableName(temp) && temp_type == "unknow" && table.data[found].paraType[curr].para != "unknow") {
				temp_type = table.data[found].paraType[curr].para;
				int found_temp_type = lookup(table, temp, id, cont);
				table.data[found_temp_type].type = table.data[found].paraType[curr].para;
			}
			if (temp_type != table.data[found].paraType[curr].para) {
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			if (table.data[found].type != "voided") {
				clearTable(table);
				block.clear();
				TypeMismatch error(line);
				throw(error);
			}
			cout << numOfFoundIndex(table, function_name, id, cont) << endl;
		}
		else if (method == "BEGIN") {
			id.level += 1;
		}
		else if (method == "END") {
			int max_level = id.level;
			id.level -= 1;
			if (id.level < 0) {
				clearTable(table);
				block.clear();
				UnknownBlock error;
				throw(error);
			}
			removeScope(table, block, cont, max_level);
		}
		else if (method == "LOOKUP") {
			int res = lookup(table, id.identifier_name, id, cont);
			if (res == -1) {
				clearTable(table);
				block.clear();
				Undeclared error(id.identifier_name);
				throw(error);
			}
			else {
				cout << res << endl;
			}
		}
		else if (method == "PRINT") {
			string res = printTable(table);
			if (res != "") { 
				cout << res << endl; 
			}
		}
		flag = false;
	}
	if (id.level > 0) {
		clearTable(table);
		block.clear();
		UnclosedBlock error(id.level);
		throw(error);
	}
	clearTable(table);
	block.clear();
	ifs.close();
}