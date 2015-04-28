#include "RegexParser.h"
#include <istream>
#include <string>

std::istream& operator>>(std::istream& Stream, char& Target) { Stream.get(Target); return Stream; }


std::shared_ptr<IRegex> RegexParser::ParseSimpleRegex(char val)
{
	if (val == '\\')
	{
		this->data->get(val);
		if (val == 'e')
		{
			return std::make_shared<EpsilonRegex>();
		}
		else if (val == 'p')
		{
			return std::make_shared<PhiRegex>();
		}
	}
	auto str = std::string(1, val);
	return std::make_shared<LiteralRegex>(str);
}

std::shared_ptr<IRegex> RegexParser::ParseRegex(char val)
{
	std::shared_ptr<IRegex> first;

	if (val == '(')
	{
		first = ParseRegex(); // RParen has been parsed, ParseRegex will take care of LParen
	}
	else
	{
		first = ParseSimpleRegex(val);
	}

	*this->data >> val;

	if (!(*this->data)) { return first; } // Nec plus ultra

	if (val == '*')
	{
		auto closure = std::make_shared<ClosureRegex>(first);
		if (!this->data->get(val)) { return closure; }
		auto next = ParseRegex(val);
		return std::make_shared<ConcatRegex>(closure, next);
	}
	else if (val == '+')
	{
		auto second = ParseRegex();
		return std::make_shared<UnionRegex>(first, second);
	}
	else if (val != ')')
	{
		auto second = ParseRegex(val);
		return std::make_shared<ConcatRegex>(first, second);
	}
	else
	{
		return first;
	}
}

std::shared_ptr<IRegex> RegexParser::ParseRegex()
{
	char val;
	this->data->get(val);
	return ParseRegex(val);
}
