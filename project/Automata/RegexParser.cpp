#include "RegexParser.h"

std::shared_ptr<IRegex> RegexParser::ParseSimpleRegex(char val)
{
	if (val == '\\')
	{
		*this->data >> val;
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
	if (val == '(')
	{
		return ParseRegex(); // RParen has been parsed, ParseRegex will take care of LParen
	}

	auto first = ParseSimpleRegex(val);

	*this->data >> val;

	if (!*this->data) { return first; } // Nec plus ultra

	if (val == '*')
	{
		return std::make_shared<ClosureRegex>(first);
	}
	else if (val == '+')
	{
		auto second = ParseRegex(val);
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
	*this->data >> val;
	return ParseRegex(val);
}