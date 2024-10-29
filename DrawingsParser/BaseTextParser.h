#pragma once

#ifndef BASETEXTPARSER_H
#define BASETEXTPARSER_H

#include <string>
#include <vector>
#include <map>
#include <utility>

class BaseTextParser
{
public:
	typedef std::map<std::wstring, std::vector<std::wstring>> Columns;

	BaseTextParser(const std::wstring& text, Columns& columns,
		std::vector<int>& componentsCountPerList, wchar_t separator);
	virtual void parse(const std::wstring& fileName) = 0;

protected:
	const std::wstring* text{ nullptr };
	Columns* columns;
	std::vector<int>* componentsCountPerList;
	size_t currentPositionInText{ 0 };
	wchar_t separator{ '^' };
	int lastComponentNumber{ 0 };

	virtual bool readLastComponentNumber() = 0;
	virtual bool readComponent(Columns* columns);

	std::pair<size_t, size_t> moveToNextSubString();
	std::pair<size_t, size_t> moveToNextSubString(size_t& positionInText) const;
	std::pair<size_t, size_t> moveToNextSubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToPreviouslySubString();
	std::pair<size_t, size_t> moveToPreviouslySubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToSubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring returnSubString(std::pair<size_t, size_t> beginEndIndexes) const;
	std::wstring getSubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getNextSubString();
	std::wstring getNextSubString(size_t& positionInText) const;
	std::wstring getNextSubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getPreviouslySubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getPreviouslySubString();
	void moveOnCountSubStr(int count, bool reverse = false);
	void reset();
private:
	size_t size_tMax{ std::numeric_limits<size_t>::max() };
};

#endif // BASETEXTPARSER_H
