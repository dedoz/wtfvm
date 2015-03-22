/*
 * VM.cpp
 *
 *  Created on: Mar 22, 2015
 *      Author: miha
 */

#include "VM.h"
#include <algorithm>
#include <sstream>


VM::VM() {
	ip = 0;
	halt = false;
	newFrame(128);
}

VM::~VM() {
	delete nval;
}

void VM::run(vector<HOLDER> program) {
	while (!halt && ip < program.size()) {
		switch(program[ip]) {
		case opcodes::PUSH: {
			push(new Value(program[ip+1]));
			ip += 2;
			continue;
		}
		case opcodes::POP: {
			pop();
			ip += 1;
			continue;
		}
		case opcodes::INT: {
			interrupt(program[ip+1]);
			ip += 2;
			continue;
		}
		case opcodes::ADD: {
			auto v1 = pop();
			auto v2 = pop();
			push(v1->add(v2));
			ip += 1;
			continue;
		}
		case opcodes::GT: {
			auto v1 = pop();
			auto v2 = pop();
			push(v1->gt(v2));
			ip += 1;
			continue;
		}
		case opcodes::LT: {
			auto v1 = pop();
			auto v2 = pop();
			push(v1->lt(v2));
			ip += 1;
			continue;
		}
		case opcodes::JMP: {
			ip = program[ip+1];
			continue;
		}
		case opcodes::JE: {
			auto v1 = pop();
			auto v2 = pop();
			if (v1->data.intval == v2->data.intval)
				ip = program[ip+1];
			else
				ip += 2;
			continue;
		}
		case opcodes::STORE : {
			int index = program[ip+1];
			astore(index);
			ip += 2;
			continue;
		}
		case opcodes::LOAD: {
			int index = program[ip+1];
			aload(index);
			ip += 2;
			continue;
		}
		case opcodes::PRE: {
			push(new CallMarker);
			ip += 1;
			continue;
		}
		case opcodes::CALL: {
			auto prevFrame = topFrame();
			int addr = program[ip+1];
			newFrame(128);
			auto v = pop(prevFrame);
			int i = 0;
			while (!v->isCallMarker()) {
				topFrame()->locals[i] = v->copy();
				v = pop(prevFrame);
				++i;
			}
			push(new Value(ip+2));
			ip = addr;
			continue;
		}
		case opcodes::RET: {
			auto value = pop();
			int addr = pop()->data.intval;
			dropFrame();
			push(value);
			ip = addr;
			continue;
		}
		case opcodes::NOP: {
			ip += 1;
			continue;
		}
		case opcodes::HALT: {
			exit(0);
		}
		default: die("unknown opcode " + program[ip]);
		}
	}
}



void VM::interrupt(int number) {
	switch(number) {
	case DEBUG: {
		cout << "DEBUG:" << endl;
		cout << dumpStats();
		break;
	}
	case SCAN: {
		int v;
		cin >> v;
		push(new Value(v));
		break;
	}
	case PRINT: {
		Value* vl = pop();
		cout << vl->data.intval << endl;
		break;
	}
	case EXIT:
		cout << "exiting_interrupt";
		exit(0);
	default:
		cout << "unknown interrupt number " << number << endl;
		return;
	}
}

Frame* VM::topFrame() {
	if(vm_stack.empty())
		die("stack underrun");
	return vm_stack.back();
}

Value* VM::pop() {
	return pop(topFrame());
}

Value* VM::pop(Frame* f) {
	if(f->op_stack.empty())
		die("operand stack underrun");
	Value* v1 = f->op_stack.back();
	f->op_stack.pop_back();
	return v1;
}

void VM::push(Value* v) {
	topFrame()->op_stack.push_back(v);
}


void VM::newFrame(int num_locals) {
	vm_stack.push_back(new Frame);
}

void VM::dropFrame() {
	if(vm_stack.empty())
		die("stack underrun");
	auto frame = topFrame();
	vm_stack.pop_back();
	delete frame;
}

void VM::aload(int index) {
	if(topFrame()->locals[index] != nullptr)
		push(topFrame()->locals[index]);
	else die("loading empty variable" + index);
}

void VM::astore(int index) {
	topFrame()->locals[index] = pop()->copy();
}

string VM::dumpStats() {
	stringstream ss;
	ss << "IP = " << ip << endl;
	ss << "Stack Frames = " << vm_stack.size() << endl;
	for (auto frame: vm_stack) {
		ss << "--------" << endl << "  OPS = ";
		for (auto v: frame->op_stack) {ss << v->data.intval << ", ";}
		ss << "\n  LOCALS = ";
		for (int i = 0; i < 256 && frame->locals[i] != nullptr; ++i) {ss << frame->locals[i]->data.intval << ", ";}
		ss << "\n--------" << endl;
	}
	return ss.str();
}
