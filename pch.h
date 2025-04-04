//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
#include <string>
#include <ostream>
using namespace std;

class MyString {
private:
	char* myString;
	int size = 0;
public:
	MyString() {
		myString = new char[1];
		myString[0] = '\0';
	}
	MyString(const char* a) {
		while (a[size] != '\0') size++;
		myString = new char[size + 1];
		for (int i = 0; i < size; ++i) {
			myString[i] = a[i];
		}
		myString[size] = '\0';
	};
	MyString(const MyString& _from) : MyString(_from.myString) {};
	char operator[](int i) const { return myString[i]; };
	MyString& operator=(const MyString& _from) {
		if (this == &_from) return *this;

		delete[] myString;

		size = _from.size;
		myString = new char[size + 1];

		for (int i = 0; i <= size; ++i) {
			myString[i] = _from.myString[i];
		}

		return *this;
	}
	friend ostream& operator<<(ostream& _os, const MyString& _toPrint) {
		_toPrint.Print(_os);
		return _os;
	}
	friend istream& operator>>(istream& _is, MyString& _toEnter) {
		char buffer[1024];
		_is >> buffer;

		delete[] _toEnter.myString;

		int newSize = 0;
		while (buffer[newSize] != '\0') newSize++;

		_toEnter.size = newSize;
		_toEnter.myString = new char[newSize + 1];

		for (int i = 0; i < newSize; i++) {
			_toEnter.myString[i] = buffer[i];
		}
		_toEnter.myString[newSize] = '\0';

		return _is;
	}
	inline int GetSize() const { return size; };

	void Insert(const char* _text, int _index) {
		int textSize = 0;
		while (_text[textSize] != '\0') textSize++;
		int newSize = size + textSize;
		char* newString = new char[newSize + 1];


		for (int i = 0; i < _index; ++i) {
			newString[i] = myString[i];
		}
		for (int i = 0; i < textSize; ++i) {
			newString[i + _index] = _text[i];
		}
		for (int i = _index; i < size; ++i) {
			newString[i + textSize] = myString[i];
		}

		newString[newSize] = '\0';
		delete[] myString;
		myString = newString;
		size = newSize;
	}
	void TryToRemove() {

	}
	void Insert(MyString& _text, int _index = 0) {
		Insert(_text.myString, _index);
	}

	ostream& Print(ostream& _os) const {
		for (int i = 0; i < size; ++i) {
			_os << myString[i];
		}
		return _os;
	};
	~MyString() {
		delete[] myString;
	}
};

struct Rule {
	string left;
	string right;
	bool isFinal;
};

class Markov {
public:
	int maxRules;
	int rulesAmount = 0;
	Rule* rules;
	Markov(int _maxRules = 5) : maxRules(_maxRules) { rules = new Rule[maxRules]; }
	void AddRule(Rule _rule) {
		if (rulesAmount >= maxRules) {
			Rule* newArray = new Rule[maxRules * 2];
			for (int i = 0; i < maxRules; ++i) {
				newArray[i] = rules[i];
			}
			delete[] rules;
			rules = newArray;
		}
		rules[rulesAmount] = _rule;
		++rulesAmount;
	}

	string& ApplyRules(string& _input) {
		string _input;
		for (int i = 0; i < rulesAmount; ++i) {
			
		}
	}

};

int main() {
	MyString aa = "522r";
	string bb = "522r";
	cout << aa << '\n';
	cout << bb << '\n';
	cout << "ss5254" << '\n';
	aa.Insert("ss5254", 2);
	cout << aa;


}