#pragma once

#ifndef BASETEXTPARSER_H
#define BASETEXTPARSER_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <regex>

class BaseTextParser
{
public:
	typedef std::map<std::wstring, std::vector<std::wstring>> Columns;

	BaseTextParser(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList, wchar_t separator);
	virtual void parse(const std::wstring& fileName) = 0;

protected:
	const std::wstring* text{ nullptr };
	Columns* columns;
	std::vector<int>* componentsCountPerList;
	size_t listsCountBeforeStart{ 0 };
	size_t currentPositionInText{ 0 };
	wchar_t separator{ '^' };
	int lastComponentNumber{ 0 };
	bool componentsEnded{ false };

	bool nextComponentNumberMissing = false;

	virtual bool readLastComponentNumber() = 0;
	virtual bool readComponent(Columns* columns);

	std::pair<size_t, size_t> moveToNextSubString();
	std::pair<size_t, size_t> moveToNextSubString(size_t& positionInText) const;
	std::pair<size_t, size_t> moveToPreviouslySubString(size_t& positionInText) const;
	std::pair<size_t, size_t> moveToNextSubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToPreviouslySubString();
	std::pair<size_t, size_t> moveToPreviouslySubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToSubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToSubString(const std::wregex& pattern);
	std::pair<size_t, size_t> moveToLastSubString();
	std::pair<size_t, size_t> moveToFirstSubString();
	std::wstring returnSubString(std::pair<size_t, size_t> beginEndIndexes) const;
	std::wstring getSubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getSubString(const std::wregex& pattern);
	std::wstring getNextSubString();
	std::wstring getNextSubString(size_t& positionInText) const;
	std::wstring getNextSubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getPreviouslySubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getPreviouslySubString(size_t& positionInText) const;
	std::wstring getPreviouslySubString();
	std::wstring getLastSubString();
	std::wstring getFirstSubString();
	void moveOnCountSubStr(int count, bool reverse = false);
	std::pair<size_t, size_t> moveOnCountSubStr(size_t& positionInText, int count, bool reverse) const;

	bool tryMoveToNextSubString(size_t& positionInText) const;
	bool tryMoveToPreviouslySubString(size_t& positionInText) const;
	bool tryMoveOnCountSubStr(size_t& positionInText, int count, bool reverse) const;

	bool searchForMatchesInFollowing(const std::wregex& pattern, int subStrsCount);

	virtual void reset();
private:
	size_t size_tMax{ std::numeric_limits<size_t>::max() };
};

#endif // BASETEXTPARSER_H
