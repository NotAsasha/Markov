//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
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
	MyString(MyString&& _from) noexcept {
		myString = _from.myString;
		size = _from.size;
		_from.myString = nullptr;
		_from.size = 0;
	}
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
	MyString& operator=(MyString&& _from) noexcept {
		if (this == &_from) return *this;

		delete[] myString;
		myString = _from.myString;
		size = _from.size;

		_from.myString = nullptr;
		_from.size = 0;

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
	void TryToRemove(int _from, int _to) {
		int deletedSize = _to - _from;
		int newSize = size - deletedSize;
		char* newString = new char[newSize + 1];
		for (int i = 0; i < _from; ++i) {
			newString[i] = myString[i];
		}
		for (int i = _from; i < newSize; ++i) {
			newString[i] = myString[i + deletedSize];
		}
		newString[newSize] = '\0';
		delete[] myString;
		myString = newString;
		size = newSize;
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
	MyString left;
	MyString right;
	bool isFinal = false;
	Rule() {};
	Rule(MyString _left, MyString _right, bool _isFinal = false) : left(_left), right(_right), isFinal(_isFinal) {};
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

	MyString& ApplyRules(MyString& _input) {
		cout << "\33[32mApplying rules...\n";
		for (int i = 0; i < rulesAmount; ++i) {
			int j = 0;
			int k = 0;
			while (_input[j] != '\0') {
				while (_input[j + k] == rules[i].left[k]) {
					if (rules[i].left[k + 1] != '\0') {
						++k;
						continue;
					}
					_input.TryToRemove(j, j + rules[i].left.GetSize());
					cout << _input << '\n';
					_input.Insert(rules[i].right, j);
					cout << _input << '\n';
					i = 0;
					k = 0;
				}
				++j;
			}
			if (rules[i].isFinal) {
				cout << "\33[0m";
				return _input;
			}
		}
		cout << "\33[0m";
		return _input;
	}
};

int main() {
	MyString aa = "Hello World! Hello World!";
	Rule testRule1("Hello", "Let`s");
	Rule testRule2("Wo", "play ");
	Rule testRule3("rld", "Minecraft", true);
	Rule testRule4("!", ")");


	Markov testMarkov;
	testMarkov.AddRule(testRule1);
	testMarkov.AddRule(testRule2);
	testMarkov.AddRule(testRule3);
	testMarkov.AddRule(testRule4);
	

	cout << "\33[33mDefault Text: \33[35m" << aa << "\33[0m\n";

	testMarkov.ApplyRules(aa);

	cout << "\33[33mResult Text: \33[35m" << aa << "\33[0m\n";
}