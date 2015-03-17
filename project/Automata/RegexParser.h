#pragma once
#include "RegexState.h"
#include "IRegex.h"
#include "PhiRegex.h"
#include "LiteralRegex.h"
#include "EpsilonRegex.h"
#include "UnionRegex.h"
#include "ConcatRegex.h"
#include "ClosureRegex.h"
#include "HashExtensions.h"

class RegexParser
{
public:
	RegexParser(std::istream& Input) : data(&Input) { }

	std::unique_ptr<IRegex> ParseRegex();

private:
	std::istream* data;
	static int GetPrecedence(char Operator);
	static bool IsOperator(char Value) { return GetPrecedence(Value) > 0; }
	std::unique_ptr<IRegex> ParseSimpleRegex();
};