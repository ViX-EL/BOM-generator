#pragma once

#ifndef ValuesCheker_h__
#define ValuesCheker_h__

#include <string>
#include <regex>

class ValuesCheker
{
public:
	enum class Type
	{
		ASSERTION,
		EXCEPTION,
		NONE
	};

	static bool trySetValue(const std::wstring& sourceValueStr, int& targetValue, const std::wregex& valuePattern, Type checkType = getType(), const char* errorMessage = "");
	static bool trySetValue(const std::wstring& sourceValueStr, std::wstring& targetValue, const std::wregex& valuePattern, bool inputCheckOff = false, Type checkType = getType(),
		const char* errorMessage = "");
	static bool CheckCondition(bool condition, const char* errorMessage = "", Type checkType = getType());
	static Type getType();
	static void setType(Type type);

private:
	static inline Type _type = Type::ASSERTION;
};

#endif // ValuesCheker_h__

