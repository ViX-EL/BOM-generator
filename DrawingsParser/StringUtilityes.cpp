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
			case L'Ñ': simbol = L'C'; break;
			case L'ñ': simbol = L'c'; break;
			case L'Å': simbol = L'E'; break;
			case L'å': simbol = L'e'; break;
			case L'Ò': simbol = L'T'; break;
			case L'ó': simbol = L'y'; break;
			case L'Î': simbol = L'O'; break;
			case L'î': simbol = L'o'; break;
			case L'Ð': simbol = L'P'; break;
			case L'ð': simbol = L'p'; break;
			case L'À': simbol = L'A'; break;
			case L'à': simbol = L'a'; break;
			case L'Õ': simbol = L'X'; break;
			case L'õ': simbol = L'x'; break;
			case L'Â': simbol = L'B'; break;
			case L'Ì': simbol = L'M'; break;
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
	{L'à', L"a"}, {L'á', L"b"}, {L'â', L"v"}, {L'ã', L"g"},
	{L'ä', L"d"}, {L'å', L"e"}, {L'¸', L"yo"}, {L'æ', L"zh"},
	{L'ç', L"z"}, {L'è', L"i"}, {L'é', L"y"}, {L'ê', L"k"},
	{L'ë', L"l"}, {L'ì', L"m"}, {L'í', L"n"}, {L'î', L"o"},
	{L'ï', L"p"}, {L'ð', L"r"}, {L'ñ', L"s"}, {L'ò', L"t"},
	{L'ó', L"u"}, {L'ô', L"f"}, {L'õ', L"kh"}, {L'ö', L"ts"},
	{L'÷', L"ch"}, {L'ø', L"sh"}, {L'ù', L"shch"}, {L'ú', L"\""},
	{L'û', L"y"}, {L'ü', L"\'"}, {L'ý', L"e"}, {L'þ', L"yu"},
	{L'ÿ', L"ya"},
	{L'À', L"A"}, {L'Á', L"B"}, {L'Â', L"V"}, {L'Ã', L"G"},
	{L'Ä', L"D"}, {L'Å', L"E"}, {L'¨', L"Yo"}, {L'Æ', L"Zh"},
	{L'Ç', L"Z"}, {L'È', L"I"}, {L'É', L"Y"}, {L'Ê', L"K"},
	{L'Ë', L"L"}, {L'Ì', L"M"}, {L'Í', L"N"}, {L'Î', L"O"},
	{L'Ï', L"P"}, {L'Ð', L"R"}, {L'Ñ', L"S"}, {L'Ò', L"T"},
	{L'Ó', L"U"}, {L'Ô', L"F"}, {L'Õ', L"Kh"}, {L'Ö', L"Ts"},
	{L'×', L"Ch"}, {L'Ø', L"Sh"}, {L'Ù', L"Shch"}, {L'Ú', L"\""},
	{L'Û', L"Y"}, {L'Ü', L"\'"}, {L'Ý', L"E"}, {L'Þ', L"Yu"},
	{L'ß', L"Ya"}
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
		if (subString[i] >= 'À' && subString[i] <= 'ÿ')
		{
			return true;
		}
	}
	return false;
}
