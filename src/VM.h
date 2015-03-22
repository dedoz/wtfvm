/*
 * VM.h
 *
 *  Created on: Mar 22, 2015
 *      Author: miha
 */

#ifndef VM_H_
#define VM_H_

#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace std;

#include "Runtime.h"

namespace opcodes {
enum OPCODE {
	PUSH, POP, LOAD, STORE,
	ADD, SUB, MUL, DIV,
	LT, GT, EQ, NEQ,
	JMP, JE, JNE, JG, JL,
	PRE, CALL, RET,
	NEW,
	INT,
	NOP, HALT
};
}

enum INTERRUPTS {
	SCAN, PRINT, EXIT, DEBUG
};


typedef int HOLDER;

class Frame {
public:
	vector<Value*> op_stack;
	vector<Value*> locals;
	Frame(): locals(256) {}
};


template <typename T>
ostream& operator << (ostream& os, const vector<T>& vec)
{
    std::copy( vec.begin(), vec.end(), ostream_iterator<T>( os ) );
    return os;
}

class VM {
public:
	VM();
	virtual ~VM();
	void run(vector<HOLDER> program);

private:
	void interrupt(int number);

	Value* pop();
	Value* pop(Frame*);
	void push(Value* v);
	void newFrame(int num_locals);
	Frame* topFrame();
	void dropFrame();
	void aload(int index);
	void astore(int index);

	string dumpStats();
	void die(string cause) {
		throw runtime_error(cause + "\n" + dumpStats());
	}

	vector<Frame*> vm_stack;
//	vector<Value*> vm_heap;
	unsigned int ip;
	bool halt;
	Value* nval = new Value;

};

#endif /* VM_H_ */
