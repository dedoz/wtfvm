//============================================================================
// Name        : wtf.cpp
// Author      : me
// Version     :
// Copyright   : wtf
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

#include "VM.h"

int main() {
	vector<int> scantest = {
			opcodes::PUSH, 100,
			opcodes::INT,  SCAN,
			opcodes::JE,   12,
			opcodes::PUSH, 0,
			opcodes::INT,  PRINT,
			opcodes::JMP,  16,
			opcodes::PUSH, 1,
			opcodes::INT,  PRINT,
			opcodes::INT,  EXIT
	};
	vector<int> vars = {
			opcodes::INT, SCAN,
			opcodes::STORE, 0,
			opcodes::INT, SCAN,
			opcodes::STORE, 1,
			opcodes::LOAD, 0,
			opcodes::LOAD, 1,
			opcodes::ADD,
			opcodes::STORE, 0,
			opcodes::LOAD, 0,
			opcodes::INT, PRINT
	};

	vector<int> func = {
			opcodes::JMP, 8,
			opcodes::LOAD, 0,
			opcodes::LOAD, 1,
			opcodes::ADD,
			opcodes::RET,
			opcodes::PRE,
			opcodes::INT, SCAN,
			opcodes::INT, SCAN,
			opcodes::CALL, 2,
			opcodes::INT,  PRINT,
			opcodes::INT,  EXIT
	};
	VM vm;
	vm.run(func);
	return 0;
}
