/*
 * Runtime.h
 *
 *  Created on: Mar 22, 2015
 *      Author: miha
 */

#ifndef RUNTIME_H_
#define RUNTIME_H_


union ValueData {
	int intval;
	string* strval;
};

enum datatype { NONE, INT, STRING };

class Value {
public:
	ValueData data;
	datatype type;
	Value(): type(NONE) {}
	Value(ValueData d): data(d), type(NONE) {}
	Value(int ival): type(INT) {
		data.intval = ival;
	}
	virtual ~Value() {}
	Value* add(Value* other) {
		return new Value(data.intval + other->data.intval);
	}
	Value* eq(Value* other) {
		return new Value(data.intval == other->data.intval);
	}
	Value* lt(Value* other) {
		return new Value(data.intval < other->data.intval);
	}
	Value* gt(Value* other) {
		return new Value(data.intval > other->data.intval);
	}
	Value* copy() {
		return new Value(data);
	}
	virtual bool isCallMarker() {return false;}
};


class CallMarker: public Value {
public:
	CallMarker() {}
	virtual bool isCallMarker() {return true;}
};

class ValueParser {
public:
	static Value* parse(int ival) {

	}

	static Value* parse(string strval) {

	}
};


#endif /* RUNTIME_H_ */
