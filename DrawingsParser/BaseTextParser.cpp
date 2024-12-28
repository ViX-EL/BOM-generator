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
		return std::pair(std::string::npos, std::string::npos);
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
		return std::pair(std::string::npos, std::string::npos);
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
		return std::pair(std::string::npos, std::string::npos);
	}
	size_t endIdx = text->find_first_of(separator, beginIdx);
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToSubString(const std::wregex& pattern, bool reverseFind)
{
	return moveUntil({ pattern }, reverseFind);
}

std::pair<size_t, size_t> BaseTextParser::moveToSubString(const std::wregex& pattern, const std::wregex& stopPattern, bool reverseFind)
{
	std::pair<size_t, size_t> beginEndIndexes = moveUntil({ pattern, stopPattern }, reverseFind);
	if (std::regex_match(returnSubString(beginEndIndexes), stopPattern))
	{
		reverseFind ? moveToNextSubString() : moveToPreviouslySubString();
		return std::pair<size_t, size_t>(std::string::npos, std::string::npos);
	}
	return beginEndIndexes;
}

std::pair<size_t, size_t> BaseTextParser::moveUntil(std::initializer_list<std::wregex> patterns, bool reverseFind)
{
	bool goalAchieved = false;
	std::pair<size_t, size_t> beginEndIndexes;
	do
	{
		!reverseFind ? beginEndIndexes = moveToNextSubString() : beginEndIndexes = moveToPreviouslySubString();
		if (beginEndIndexes.second == std::wstring::npos || beginEndIndexes.first == std::wstring::npos) {
			break;
		}
		std::wstring subString = returnSubString(beginEndIndexes);
		for (const auto& pattern : patterns) {
			goalAchieved = goalAchieved || std::regex_match(subString, pattern);
		}
	} while (!goalAchieved);
	return beginEndIndexes;
}

std::pair<size_t, size_t> BaseTextParser::moveToLastSubString()
{
	size_t endIdx = text->size() - 1;
	size_t beginIdx = text->rfind(separator, endIdx - 1) + 1;
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToFirstSubString()
{
	size_t beginIdx = 1;
	size_t endIdx = text->find(separator, beginIdx);
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

// ищет предыдущую подстроку для строки, которая ищется далее по тексту от
// текущей позиции в тексте
std::wstring BaseTextParser::getPreviouslySubString(const std::wstring& subString, bool reverseFind)
{
	return returnSubString(moveToPreviouslySubString(subString, reverseFind));
}


// возвращает следующую подстроку для подстроки, которая ищется дальше по тексту или назад
// по тексту, в зависимости от reverseFind, относительно текущей позиции в тексте
std::wstring BaseTextParser::getNextSubString(const std::wstring& subString, bool reverseFind)
{
	return returnSubString(moveToNextSubString(subString, reverseFind));
}

std::wstring BaseTextParser::returnSubString(std::pair<size_t, size_t> beginEndIndexes) const
{
	if (beginEndIndexes.first != std::string::npos && beginEndIndexes.second != std::string::npos) {
		return std::wstring(text->begin() + beginEndIndexes.first, text->begin() + beginEndIndexes.second);
	}
	else {
		return std::wstring();
	}
}

std::wstring BaseTextParser::getSubString(const std::wstring& subString, bool reverseFind)
{
	return returnSubString(moveToSubString(subString, reverseFind));
}

std::wstring BaseTextParser::getSubString(const std::wregex& pattern, bool reverseFind)
{
	return returnSubString(moveToSubString(pattern, reverseFind));
}

std::wstring BaseTextParser::getSubString(const std::wregex& pattern, const std::wregex& stopPattern, bool reverseFind)
{
	return returnSubString(moveToSubString(pattern, stopPattern, reverseFind));
}

std::wstring BaseTextParser::getNextSubString()
{
	return returnSubString(moveToNextSubString());
}

std::pair<size_t, size_t> BaseTextParser::moveToNextSubString()
{
	if (currentPositionInText + 2 < currentPositionInText) {
		return std::pair(std::string::npos, std::string::npos); // Выход за пределы строки вправо
	}
	size_t beginIdx = currentPositionInText + 2;
	size_t endIdx = text->find_first_of(separator, beginIdx);
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToNextSubString(size_t& positionInText) const
{
	size_t beginIdx = text->find_first_of(separator, positionInText) + 1;
	size_t endIdx = text->find_first_of(separator, beginIdx);
	if (beginIdx == std::string::npos || endIdx == std::string::npos) {
		return std::pair(std::string::npos, std::string::npos);
	}
	positionInText = endIdx;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToPreviouslySubString(size_t& positionInText) const
{
	size_t endIdx = text->rfind(separator, positionInText);
	size_t beginIdx = text->rfind(separator, endIdx - 1) + 1;
	if (beginIdx == std::string::npos || endIdx == std::string::npos) {
		return std::pair(std::string::npos, std::string::npos);
	}
	positionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToPreviouslySubString()
{
	size_t endIdx = text->rfind(separator, currentPositionInText);
	if (endIdx - 1 == std::string::npos) {
		return std::pair(std::string::npos, std::string::npos); // Выход за пределы строки влево
	}
	size_t beginIdx = text->rfind(separator, endIdx - 1) + 1;
	currentPositionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

bool BaseTextParser::tryMoveToNextSubString(size_t& positionInText) const
{
	std::pair<size_t, size_t> subStrBegiEnd = moveToNextSubString(positionInText);
	if (subStrBegiEnd.second != std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}

bool BaseTextParser::tryMoveToPreviouslySubString(size_t& positionInText) const
{
	std::pair<size_t, size_t> subStrBegiEnd = moveToPreviouslySubString(positionInText);
	if (subStrBegiEnd.first != std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}

bool BaseTextParser::tryMoveOnCountSubStr(size_t& positionInText, int count, bool reverse) const
{
	bool subStrExist = false;
	if (reverse)
	{
		for (int i = 0; i < count; i++) {
			subStrExist = tryMoveToPreviouslySubString(positionInText);
			if (!subStrExist) {
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < count; i++) {
			subStrExist = tryMoveToNextSubString(positionInText);
			if (!subStrExist) {
				break;
			}
		}
	}
	return subStrExist;
}

bool BaseTextParser::searchForMatchesInFollowing(const std::wregex& pattern, int subStrsCount)
{
	size_t positionInText = currentPositionInText;
	for (int i = 0; i < subStrsCount; i++) {
		if (std::regex_search(getNextSubString(positionInText), pattern)) {
			return true;
		}
	}
	return false;
}

std::wstring BaseTextParser::getNextSubString(size_t& positionInText) const
{
	return returnSubString(moveToNextSubString(positionInText));
}

std::wstring BaseTextParser::getPreviouslySubString(size_t& positionInText) const
{
	return returnSubString(moveToPreviouslySubString(positionInText));
}

std::wstring BaseTextParser::getPreviouslySubString()
{
	return returnSubString(moveToPreviouslySubString());
}

std::wstring BaseTextParser::getLastSubString()
{
	return returnSubString(moveToLastSubString());
}

std::wstring BaseTextParser::getFirstSubString()
{
	return returnSubString(moveToFirstSubString());
}

void BaseTextParser::moveOnCountSubStr(size_t count, bool reverse)
{
	if (reverse)
	{
		for (size_t i = 0; i < count; i++) {
			moveToPreviouslySubString();
		}
	}
	else
	{
		for (size_t i = 0; i < count; i++) {
			moveToNextSubString();
		}
	}
}

std::pair<size_t, size_t> BaseTextParser::moveOnCountSubStr(size_t& positionInText, int count, bool reverse) const
{
	std::pair<size_t, size_t> subStrBegiEnd;
	if (reverse)
	{
		for (int i = 0; i < count; i++) {
			subStrBegiEnd = moveToPreviouslySubString(positionInText);
		}
	}
	else
	{
		for (int i = 0; i < count; i++) {
			subStrBegiEnd = moveToNextSubString(positionInText);
		}
	}
	return subStrBegiEnd;
}

void BaseTextParser::reset()
{
	currentPositionInText = 0;
	componentsEnded = false;
	lastDrawingPagePtr = nullptr;
	lastComponentPtr = nullptr;
}

BaseTextParser::BaseTextParser(const std::wstring& text,  wchar_t separator) : text(&text), separator(separator)
{

}