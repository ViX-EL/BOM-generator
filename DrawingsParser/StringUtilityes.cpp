#include "StringUtilities.h"

#include <string>
#include <unordered_map>
#include <regex>

std::wstring StringUtilities::replaceSimilarRuSimbols(const std::wstring& sourceStr)
{
	std::wstring newStr;
	newStr.reserve(sourceStr.size());
	for (wchar_t simbol : sourceStr)
	{
		if (!((simbol >= L'A' && simbol <= L'Z') || (simbol >= L'a' && simbol <= L'z')))
		{
			switch (simbol)
			{
			case L'С': simbol = L'C'; break;
			case L'с': simbol = L'c'; break;
			case L'Е': simbol = L'E'; break;
			case L'е': simbol = L'e'; break;
			case L'Т': simbol = L'T'; break;
			case L'у': simbol = L'y'; break;
			case L'О': simbol = L'O'; break;
			case L'о': simbol = L'o'; break;
			case L'Р': simbol = L'P'; break;
			case L'р': simbol = L'p'; break;
			case L'А': simbol = L'A'; break;
			case L'а': simbol = L'a'; break;
			case L'Х': simbol = L'X'; break;
			case L'х': simbol = L'x'; break;
			case L'В': simbol = L'B'; break;
			case L'М': simbol = L'M'; break;
			default:
				break;
			}
		}
		newStr += simbol;
	}
	return newStr;
}

std::wstring StringUtilities::truncate(const std::wstring& sourceStr)
{
	size_t startIndex = 0;
	for (wchar_t currentChar : sourceStr)
	{
		if (currentChar == L' ') {
			startIndex++;
		}
		else {
			break;
		}
	}

	size_t endIndex = sourceStr.size();
	for (size_t i = sourceStr.size() - 1; i > 0; i--)
	{
		if (sourceStr[i] == L' ') {
			endIndex--;
		}
		else {
			break;
		}
	}
	return sourceStr.substr(startIndex, endIndex);
}

std::wstring StringUtilities::removeSpaces(const std::wstring& sourceStr)
{
	std::wstring newString = sourceStr;
	for (size_t i = 0; i < newString.size(); i++)
	{
		if (newString[i] == L' ') {
			newString = newString.erase(i, 1);
		}
	}
	return newString;
}

std::wstring StringUtilities::transliterate(const std::wstring& russianText)
{
	std::unordered_map<wchar_t, std::wstring> charMap = {
	{L'а', L"a"}, {L'б', L"b"}, {L'в', L"v"}, {L'г', L"g"},
	{L'д', L"d"}, {L'е', L"e"}, {L'ё', L"yo"}, {L'ж', L"zh"},
	{L'з', L"z"}, {L'и', L"i"}, {L'й', L"y"}, {L'к', L"k"},
	{L'л', L"l"}, {L'м', L"m"}, {L'н', L"n"}, {L'о', L"o"},
	{L'п', L"p"}, {L'р', L"r"}, {L'с', L"s"}, {L'т', L"t"},
	{L'у', L"u"}, {L'ф', L"f"}, {L'х', L"kh"}, {L'ц', L"ts"},
	{L'ч', L"ch"}, {L'ш', L"sh"}, {L'щ', L"shch"}, {L'ъ', L"\""},
	{L'ы', L"y"}, {L'ь', L"\'"}, {L'э', L"e"}, {L'ю', L"yu"},
	{L'я', L"ya"},
	{L'А', L"A"}, {L'Б', L"B"}, {L'В', L"V"}, {L'Г', L"G"},
	{L'Д', L"D"}, {L'Е', L"E"}, {L'Ё', L"Yo"}, {L'Ж', L"Zh"},
	{L'З', L"Z"}, {L'И', L"I"}, {L'Й', L"Y"}, {L'К', L"K"},
	{L'Л', L"L"}, {L'М', L"M"}, {L'Н', L"N"}, {L'О', L"O"},
	{L'П', L"P"}, {L'Р', L"R"}, {L'С', L"S"}, {L'Т', L"T"},
	{L'У', L"U"}, {L'Ф', L"F"}, {L'Х', L"Kh"}, {L'Ц', L"Ts"},
	{L'Ч', L"Ch"}, {L'Ш', L"Sh"}, {L'Щ', L"Shch"}, {L'Ъ', L"\""},
	{L'Ы', L"Y"}, {L'Ь', L"\'"}, {L'Э', L"E"}, {L'Ю', L"Yu"},
	{L'Я', L"Ya"}
	};

	std::wstring result;

	for (wchar_t ch : russianText)
	{
		if (charMap.find(ch) != charMap.end()) {
			result += charMap[ch];
		}
		else {
			result += ch;
		}
	}

	return result;
}

const std::wregex& StringUtilities::getRegex(const std::wstring& pattern)
{
	static std::unordered_map<std::wstring, std::wregex> cache;
	auto it = cache.find(pattern);
	if (it != cache.end()) {
		return it->second;
	}
	return cache.emplace(pattern, std::wregex(pattern)).first->second;
}

bool StringUtilities::isContainsCyrillicLetters(char* subString)
{
	size_t size = strlen(subString);
	for (size_t i = 0; i < size; i++)
	{
		if (subString[i] >= 'А' && subString[i] <= 'я')
		{
			return true;
		}
	}
	return false;
}
