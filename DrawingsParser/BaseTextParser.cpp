#include "BaseTextParser.h"

#include<limits>
#include <utility>
#include <regex>

std::pair<size_t, size_t> BaseTextParser::moveToNextSubString(const std::wstring& subString, bool reverseFind)
{
	size_t beginIdx;
	if (!reverseFind) {
		beginIdx = text->find(subString, currentPositionInText);
	}
	else {
		beginIdx = text->rfind(subString, currentPositionInText);
	}
	if (beginIdx == std::string::npos) {
		return std::pair(size_tMax, size_tMax);
	}
	size_t endIdx = text->find_first_of(separator, beginIdx) + 1;
	beginIdx = endIdx;
	endIdx = text->find_first_of(separator, beginIdx);
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToPreviouslySubString(const std::wstring& subString, bool reverseFind)
{
	size_t endIdx;
	if (!reverseFind) {
		endIdx = text->find(subString, currentPositionInText);
	}
	else {
		endIdx = text->rfind(subString, currentPositionInText);
	}
	if (endIdx == std::string::npos) {
		return std::pair(size_tMax, size_tMax);
	}
	endIdx = text->rfind(separator, endIdx);
	size_t beginIdx = text->rfind(separator, endIdx - 1) + 1;
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToSubString(const std::wstring& subString, bool reverseFind)
{
	size_t beginIdx;
	if (!reverseFind) {
		beginIdx = text->find(subString, currentPositionInText);
	}
	else {
		beginIdx = text->rfind(subString, currentPositionInText);
	}
	if (beginIdx == std::string::npos) {
		return std::pair(size_tMax, size_tMax);
	}
	size_t endIdx = text->find_first_of(separator, beginIdx);
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

// ищет предыдущую подстроку для строки, которая ищется далее по тексту от
// текущей позиции в тексте
std::wstring BaseTextParser::getPreviouslySubString(const std::wstring& subString, bool reverseFind)
{
	std::pair<size_t, size_t> beginEndIndexes = moveToPreviouslySubString(subString, reverseFind);
	return returnSubString(beginEndIndexes);
}


// возвращает следующую подстроку для подстроки, которая ищется дальше по тексту или назад
// по тексту, в зависимости от reverseFind, относительно текущей позиции в тексте
std::wstring BaseTextParser::getNextSubString(const std::wstring& subString, bool reverseFind)
{
	std::pair<size_t, size_t> beginEndIndexes = moveToNextSubString(subString, reverseFind);
	return returnSubString(beginEndIndexes);
}

std::wstring BaseTextParser::returnSubString(std::pair<size_t, size_t> beginEndIndexes)
{
	if (beginEndIndexes.first != size_tMax && beginEndIndexes.second != size_tMax) {
		return std::wstring(text->begin() + beginEndIndexes.first, text->begin() + beginEndIndexes.second);
	}
	else {
		return std::wstring();
	}
}

std::wstring BaseTextParser::getSubString(const std::wstring& subString, bool reverseFind)
{
	std::pair<size_t, size_t> beginEndIndexes = moveToSubString(subString, reverseFind);
	return returnSubString(beginEndIndexes);
}

std::wstring BaseTextParser::getNextSubString()
{
	std::pair<size_t, size_t> beginEndIndxes = moveToNextSubString();
	return std::wstring(text->begin() + beginEndIndxes.first, text->begin() + beginEndIndxes.second);
}

std::pair<size_t, size_t> BaseTextParser::moveToNextSubString()
{
	size_t beginIdx = currentPositionInText + 2;
	size_t endIdx = text->find_first_of(separator, beginIdx);
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToNextSubString(size_t& positionInText) const
{
	size_t beginIdx = text->find_first_of(separator, positionInText) + 1;
	size_t endIdx = text->find_first_of(separator, beginIdx);
	positionInText = endIdx;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToPreviouslySubString()
{
	size_t endIdx = text->rfind(separator, currentPositionInText);
	size_t beginIdx = text->rfind(separator, endIdx - 1) + 1;
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::wstring BaseTextParser::getNextSubString(size_t& positionInText) const
{
	std::pair<size_t, size_t> beginEndIndxes = moveToNextSubString(positionInText);
	return std::wstring(text->begin() + beginEndIndxes.first, text->begin() + beginEndIndxes.second);
}

std::wstring BaseTextParser::getPreviouslySubString()
{
	std::pair<size_t, size_t> beginEndIndxes = moveToPreviouslySubString();
	
	return std::wstring(text->begin() + beginEndIndxes.first, text->begin() + beginEndIndxes.second);
}

void BaseTextParser::moveOnCountSubStr(int count, bool reverse)
{
	if (reverse)
	{
		for (int i = 0; i < count; i++) {
			moveToPreviouslySubString();
		}
	}
	else
	{
		for (int i = 0; i < count; i++) {
			moveToNextSubString();
		}
	}
}

void BaseTextParser::reset()
{
	currentPositionInText = 0;
	lastComponentNumber = 0;
}

BaseTextParser::BaseTextParser(const std::wstring& text, Columns& columns,
	std::vector<int>& componentsCountPerList, wchar_t separator) : text(&text), columns(&columns),
	componentsCountPerList(&componentsCountPerList), separator(separator)
{

}

bool BaseTextParser::readComponent(Columns* columns)
{
	if (!readLastComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 3; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	std::wregex diameterPattern(LR"(\d{1,4} x( \d+)?|\d{1,4})");
	std::wregex itemCodePattern(LR"([a-zA-Z0-9()]+)");
	std::wregex countPattern(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
	bool cases[]{ false, false };

	int descriptionSubStrCount{ 1 };
	while (!cases[0] && !cases[1])
	{
		cases[0] = std::regex_match(*(subStrBuffer.end() - 2), diameterPattern);
		cases[0] = cases[0] && std::regex_match(*(subStrBuffer.end() - 1), countPattern);

		if (!cases[0])
		{
			cases[1] = std::regex_match(*(subStrBuffer.end() - 3), diameterPattern);
			cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 2), itemCodePattern);
			cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 1), countPattern);
		}

		if (!cases[0] && !cases[1]) {
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	if (cases[0])
	{
		if ((subStrBuffer.end() - 2)->ends_with(L"x")) {
			subStrBuffer.emplace_back(getNextSubString());
		}
	}

	if (cases[1] && (subStrBuffer.end() - 3)->ends_with(L"x")) {
		descriptionSubStrCount--;
	}
	std::wstring descriptionStr;
	for (int i = 0; i < descriptionSubStrCount; i++)
	{
		descriptionStr += subStrBuffer[i];
	}
	(*columns)[L"Описание компонента"].emplace_back(descriptionStr);

	if (cases[0])
	{
		if ((subStrBuffer.end() - 3)->ends_with(L"x")) {
			(*columns)[L"Условный диаметр"].emplace_back((*(subStrBuffer.end() - 3) 
				+ L' ' + (*(subStrBuffer.end() - 2))));
		}
		else {
			(*columns)[L"Условный диаметр"].emplace_back(*(subStrBuffer.end() - 2));
		}
		(*columns)[L"Код позиции"].emplace_back(L"-");
		(*columns)[L"Кол-во"].emplace_back(*(subStrBuffer.end() - 1));
	}
	else if (cases[1])
	{
		if ((subStrBuffer.end() - 4)->ends_with(L"x")) {
			(*columns)[L"Условный диаметр"].emplace_back((*(subStrBuffer.end() - 4)
				+ L' ' + (*(subStrBuffer.end() - 3))));
		}
		else {
			(*columns)[L"Условный диаметр"].emplace_back(*(subStrBuffer.end() - 3));
		}
		(*columns)[L"Код позиции"].emplace_back(*(subStrBuffer.end() - 2));
		(*columns)[L"Кол-во"].emplace_back(*(subStrBuffer.end() - 1));
	}
	(*columns)[L"Документ"].emplace_back(L"-");
	return true;
}