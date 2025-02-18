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
			case L'�': simbol = L'C'; break;
			case L'�': simbol = L'c'; break;
			case L'�': simbol = L'E'; break;
			case L'�': simbol = L'e'; break;
			case L'�': simbol = L'T'; break;
			case L'�': simbol = L'y'; break;
			case L'�': simbol = L'O'; break;
			case L'�': simbol = L'o'; break;
			case L'�': simbol = L'P'; break;
			case L'�': simbol = L'p'; break;
			case L'�': simbol = L'A'; break;
			case L'�': simbol = L'a'; break;
			case L'�': simbol = L'X'; break;
			case L'�': simbol = L'x'; break;
			case L'�': simbol = L'B'; break;
			case L'�': simbol = L'M'; break;
			default:
				break;
			}
		}
		newStr += simbol;
	}
	return newStr;
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
	{L'�', L"a"}, {L'�', L"b"}, {L'�', L"v"}, {L'�', L"g"},
	{L'�', L"d"}, {L'�', L"e"}, {L'�', L"yo"}, {L'�', L"zh"},
	{L'�', L"z"}, {L'�', L"i"}, {L'�', L"y"}, {L'�', L"k"},
	{L'�', L"l"}, {L'�', L"m"}, {L'�', L"n"}, {L'�', L"o"},
	{L'�', L"p"}, {L'�', L"r"}, {L'�', L"s"}, {L'�', L"t"},
	{L'�', L"u"}, {L'�', L"f"}, {L'�', L"kh"}, {L'�', L"ts"},
	{L'�', L"ch"}, {L'�', L"sh"}, {L'�', L"shch"}, {L'�', L"\""},
	{L'�', L"y"}, {L'�', L"\'"}, {L'�', L"e"}, {L'�', L"yu"},
	{L'�', L"ya"},
	{L'�', L"A"}, {L'�', L"B"}, {L'�', L"V"}, {L'�', L"G"},
	{L'�', L"D"}, {L'�', L"E"}, {L'�', L"Yo"}, {L'�', L"Zh"},
	{L'�', L"Z"}, {L'�', L"I"}, {L'�', L"Y"}, {L'�', L"K"},
	{L'�', L"L"}, {L'�', L"M"}, {L'�', L"N"}, {L'�', L"O"},
	{L'�', L"P"}, {L'�', L"R"}, {L'�', L"S"}, {L'�', L"T"},
	{L'�', L"U"}, {L'�', L"F"}, {L'�', L"Kh"}, {L'�', L"Ts"},
	{L'�', L"Ch"}, {L'�', L"Sh"}, {L'�', L"Shch"}, {L'�', L"\""},
	{L'�', L"Y"}, {L'�', L"\'"}, {L'�', L"E"}, {L'�', L"Yu"},
	{L'�', L"Ya"}
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
		if (subString[i] >= '�' && subString[i] <= '�')
		{
			return true;
		}
	}
	return false;
}
