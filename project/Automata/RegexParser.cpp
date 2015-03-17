#include "RegexParser.h"

int RegexParser::GetPrecedence(char Operator)
{
	switch (Operator)
	{
	case '+':
		return 2;
	case '*':
		return 1;
	default:
		return 0;
	}
}

std::unique_ptr<IRegex> RegexParser::ParseSimpleRegex()
{
	char val;
	*this->data >> val;
	if (!IsOperator(val))
	{
		return std::make_unique<LiteralRegex>(std::string(1, val));
	}
	else
	{

	}
}