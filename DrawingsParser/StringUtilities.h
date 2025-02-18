#pragma once

#include <string>
#include <regex>

class StringUtilities
{
public:
	//Заменяет русские символы, которые похожи на английские, на английские символы
	static std::wstring replaceSimilarRuSimbols(const std::wstring& sourceStr);
	static std::wstring removeSpaces(const std::wstring& sourceStr);

	static std::wstring transliterate(const std::wstring& russianText);

	static const std::wregex& getRegex(const std::wstring& pattern);

	static bool isContainsCyrillicLetters(char* subString);
};

