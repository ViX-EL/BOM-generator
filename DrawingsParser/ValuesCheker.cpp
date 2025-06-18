#include "ValuesCheker.h"
#include "StringConvert.h"

#include <cassert>

bool ValuesCheker::trySetValue(const std::wstring& sourceValueStr, int& targetValue, const std::wregex& valuePattern, Type checkType, const char* errorMessage)
{
	bool isMatch = std::regex_match(sourceValueStr, valuePattern);
	if (checkType != Type::NONE)
	{
		if (checkType == Type::ASSERTION) {
			assert(isMatch && errorMessage);
		}
		else if (!isMatch)
		{
			throw std::logic_error(std::string(errorMessage) + ", значение: " + narrow(sourceValueStr, std::locale("Russian")));
		}
	}
	if (isMatch)
	{
		targetValue = stoi(sourceValueStr);
		return true;
	}
	return false;
}

bool ValuesCheker::trySetValue(const std::wstring& sourceValueStr, std::wstring& targetValue, const std::wregex& valuePattern, bool inputCheckOff, Type checkType, const char* errorMessage)
{
	if (inputCheckOff)
	{
		targetValue = sourceValueStr;
		return true;
	}
	else
	{
		bool isMatch = std::regex_match(sourceValueStr, valuePattern);
		if (checkType != Type::NONE)
		{
			if (checkType == Type::ASSERTION) {
				assert(isMatch && errorMessage);
			}
			else if (!isMatch)
			{
				throw std::logic_error(std::string(errorMessage) + ", значение: " + narrow(sourceValueStr, std::locale("Russian")));
				//throw std::logic_error(std::string(errorMessage) + ", значение: " + narrow(sourceValueStr, std::locale("ru_RU.utf8")));
				//throw std::logic_error(std::string(errorMessage) + ", значение: " + utf8_encode(sourceValueStr));
			}
		}
		if (isMatch)
		{
			targetValue = sourceValueStr;
			return true;
		}
		return false;
	}
}

bool ValuesCheker::CheckCondition(bool condition, const char* errorMessage, Type checkType)
{
	if (checkType != Type::NONE)
	{
		if (checkType == Type::ASSERTION) {
			assert(condition && errorMessage);
		}
		else if (!condition)
		{
			throw std::logic_error(std::string(errorMessage));
		}
	}
	else {
		return condition;
	}
	return false;
}



ValuesCheker::Type ValuesCheker::getType()
{
    return _type;
}

void ValuesCheker::setType(Type type)
{
	_type = type;
}


