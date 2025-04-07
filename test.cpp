#include <gtest/gtest.h>
#include "pch.h"

TEST(MarkovTest, ApplyRules) {
    Markov markov;

    markov.AddRule(Rule(MyString("l"), MyString("|")));
    markov.AddRule(Rule(MyString("o"), MyString("0")));
    markov.AddRule(Rule(MyString("H"), MyString("4")));
    markov.AddRule(Rule(MyString("e"), MyString("3")));
    markov.AddRule(Rule(MyString("|"), MyString("1")));

    MyString input("Hello");
    MyString result = markov.ApplyRules(input);
    EXPECT_EQ(result, MyString("43110"));

    input = MyString("Hello World");
    result = markov.ApplyRules(input);
    EXPECT_EQ(result, MyString("43110 W0r1d"));

    markov.AddRule(Rule(MyString("o"), MyString("0"), true));
    input = MyString("Hello");
    result = markov.ApplyRules(input);
    EXPECT_EQ(result, MyString("43110"));
}

TEST(MarkovTest, NoChangeOnNoMatch) {
    Markov markov;

    markov.AddRule(Rule(MyString("z"), MyString("X")));
    MyString input("Hello");

    MyString result = markov.ApplyRules(input);
    EXPECT_EQ(result, MyString("Hello"));
}

TEST(MarkovTest, MultipleRuleApplication) {
    Markov markov;

    markov.AddRule(Rule(MyString("l"), MyString("|")));
    markov.AddRule(Rule(MyString("o"), MyString("0")));

    MyString input("Hello");
    MyString result = markov.ApplyRules(input);
    EXPECT_EQ(result, MyString("He||0"));

    result = markov.ApplyRules(result);
    EXPECT_EQ(result, MyString("He||0"));
}

TEST(MarkovTest, EmptyString) {
    Markov markov;

    MyString input("");
    MyString result = markov.ApplyRules(input);
    EXPECT_EQ(result, MyString(""));
}