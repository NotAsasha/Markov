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
	bool operator==(const MyString& other) const {
		if (size != other.size) {
			return false;
		}
		for (int i = 0; i < size; ++i) {
			if (myString[i] != other.myString[i]) {
				return false;
			}
		}
		return true;
	}
	bool operator!=(const MyString& other) const {
		return !(*this == other);
	}
	friend ostream& operator<<(ostream& _os, const MyString& _toPrint) {
		_toPrint.Print(_os);
		return _os;
	}
	friend istream& operator>>(istream& _is, MyString& _toEnter) {
		char buffer[1024];

		_is.getline(buffer, sizeof(buffer)); // I had to use it :(((

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
	Markov(int _maxRules = 40) : maxRules(_maxRules) { rules = new Rule[maxRules]; }
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
		cout << "\33[32mApplying rules...\33[0m\n";
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
					_input.Insert(rules[i].right, j);
					i = 0;
					k = 0;
				}
				++j;
			}
			if (rules[i].isFinal) {
				return _input;
			}
		}
		return _input;
	}
};

//int main() {
//	cout << "\33[33mText to Leet Text! Because why not? :)\33[0m" << endl;
//	MyString toChange;
//	cin >> toChange;
//
//	Markov testMarkov;
//#pragma region RulesIGuess
//	testMarkov.AddRule(Rule(MyString("a"), MyString("4")));
//	testMarkov.AddRule(Rule(MyString("A"), MyString("4")));
//	testMarkov.AddRule(Rule(MyString("b"), MyString("8")));
//	testMarkov.AddRule(Rule(MyString("B"), MyString("8")));
//	testMarkov.AddRule(Rule(MyString("c"), MyString("(")));
//	testMarkov.AddRule(Rule(MyString("C"), MyString("(")));
//	testMarkov.AddRule(Rule(MyString("d"), MyString("|)")));
//	testMarkov.AddRule(Rule(MyString("D"), MyString("|)")));
//	testMarkov.AddRule(Rule(MyString("f"), MyString("ph")));
//	testMarkov.AddRule(Rule(MyString("H"), MyString("|-|")));
//	testMarkov.AddRule(Rule(MyString("o"), MyString("0")));
//	testMarkov.AddRule(Rule(MyString("l"), MyString("|")));
//	testMarkov.AddRule(Rule(MyString("k"), MyString("|<")));
//	testMarkov.AddRule(Rule(MyString("F"), MyString("Ph")));
//	testMarkov.AddRule(Rule(MyString("m"), MyString("/\\/\\")));
//	testMarkov.AddRule(Rule(MyString("M"), MyString("/\\/\\")));
//	testMarkov.AddRule(Rule(MyString("n"), MyString("|\\|")));
//	testMarkov.AddRule(Rule(MyString("N"), MyString("|\\|")));
//	testMarkov.AddRule(Rule(MyString("p"), MyString("|*")));
//	testMarkov.AddRule(Rule(MyString("P"), MyString("|*")));
//	testMarkov.AddRule(Rule(MyString("r"), MyString("|2")));
//	testMarkov.AddRule(Rule(MyString("R"), MyString("|2")));
//	testMarkov.AddRule(Rule(MyString("t"), MyString("7")));
//	testMarkov.AddRule(Rule(MyString("T"), MyString("7")));
//	testMarkov.AddRule(Rule(MyString("u"), MyString("(_)")));
//	testMarkov.AddRule(Rule(MyString("U"), MyString("(_)")));
//	testMarkov.AddRule(Rule(MyString("w"), MyString("\\/\\/")));
//	testMarkov.AddRule(Rule(MyString("W"), MyString("\\/\\/")));
//	testMarkov.AddRule(Rule(MyString("x"), MyString("><")));
//	testMarkov.AddRule(Rule(MyString("X"), MyString("><")));
//	testMarkov.AddRule(Rule(MyString("y"), MyString("`/")));
//	testMarkov.AddRule(Rule(MyString("Y"), MyString("`/")));
//	testMarkov.AddRule(Rule(MyString("z"), MyString("2")));
//	testMarkov.AddRule(Rule(MyString("Z"), MyString("2")));
//#pragma endregion
//
//	testMarkov.ApplyRules(toChange);
//	cout << "\33[33mResult Text: \33[35m" << toChange << "\33[0m\n";
//}