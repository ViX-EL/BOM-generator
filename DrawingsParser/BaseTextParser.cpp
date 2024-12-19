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

std::pair<size_t, size_t> BaseTextParser::moveToSubString(const std::wregex& pattern, bool reverseFind)
{
	std::pair<size_t, size_t> beginEndIndexes;
	do
	{
		if (!reverseFind) {
			beginEndIndexes = moveToNextSubString();
		}
		else {
			beginEndIndexes = moveToPreviouslySubString();
		}
		if (beginEndIndexes.second == std::wstring::npos || beginEndIndexes.first == std::wstring::npos) {
			break;
		}
	} while (!std::regex_match(text->substr(beginEndIndexes.first, beginEndIndexes.second - beginEndIndexes.first), pattern));
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
	currentPositionInText = endIdx;
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

std::wstring BaseTextParser::returnSubString(std::pair<size_t, size_t> beginEndIndexes) const
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

std::wstring BaseTextParser::getSubString(const std::wregex& pattern, bool reverseFind)
{
	std::pair<size_t, size_t> beginEndIndexes = moveToSubString(pattern, reverseFind);
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
	if (beginIdx == std::string::npos || endIdx == std::string::npos) {
		return std::pair(size_tMax, size_tMax);
	}
	positionInText = endIdx;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToPreviouslySubString(size_t& positionInText) const
{
	size_t endIdx = text->rfind(separator, positionInText);
	size_t beginIdx = text->rfind(separator, endIdx - 1) + 1;
	if (beginIdx == std::string::npos || endIdx == std::string::npos) {
		return std::pair(size_tMax, size_tMax);
	}
	positionInText = endIdx - 1;
	return std::pair(beginIdx, endIdx);
}

std::pair<size_t, size_t> BaseTextParser::moveToPreviouslySubString()
{
	size_t endIdx = text->rfind(separator, currentPositionInText);
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
	std::pair<size_t, size_t> beginEndIndxes = moveToNextSubString(positionInText);
	return std::wstring(text->begin() + beginEndIndxes.first, text->begin() + beginEndIndxes.second);
}

std::wstring BaseTextParser::getPreviouslySubString(size_t& positionInText) const
{
	std::pair<size_t, size_t> beginEndIndxes = moveToPreviouslySubString(positionInText);
	return std::wstring(text->begin() + beginEndIndxes.first, text->begin() + beginEndIndxes.second);
}

std::wstring BaseTextParser::getPreviouslySubString()
{
	std::pair<size_t, size_t> beginEndIndxes = moveToPreviouslySubString();
	return std::wstring(text->begin() + beginEndIndxes.first, text->begin() + beginEndIndxes.second);
}

std::wstring BaseTextParser::getLastSubString()
{
	std::pair<size_t, size_t> beginEndIndxes = moveToLastSubString();
	return std::wstring(text->begin() + beginEndIndxes.first, text->begin() + beginEndIndxes.second);
}

std::wstring BaseTextParser::getFirstSubString()
{
	std::pair<size_t, size_t> beginEndIndxes = moveToFirstSubString();
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