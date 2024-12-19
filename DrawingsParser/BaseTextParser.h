#pragma once

#ifndef BASETEXTPARSER_H
#define BASETEXTPARSER_H

#include "DrawingPage.h"

#include <string>
#include <vector>
#include <utility>
#include <regex>
#include <memory>

struct Drawing
{
	std::vector<std::shared_ptr<DrawingPage>> pages;
};

class BaseTextParser
{
public:
	BaseTextParser(const std::wstring& text, wchar_t separator);
	virtual void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) = 0;

protected:
	const std::wstring* text{ nullptr };
	size_t currentPositionInText{ 0 };
	wchar_t separator{ '^' };
	bool componentsEnded{ false };

	std::shared_ptr<DrawingPage> lastDrawingPagePtr;
	std::vector<Drawing>* drawingsPtr{ nullptr };
	BuildComponent* lastComponentPtr{ nullptr };
	Drawing* currentDrawingPtr{ nullptr };

	bool nextComponentNumberMissing{ false };

	virtual bool readComponentNumber() = 0;
	virtual bool readComponent() = 0;

	std::pair<size_t, size_t> moveToNextSubString();
	std::pair<size_t, size_t> moveToNextSubString(size_t& positionInText) const;
	std::pair<size_t, size_t> moveToPreviouslySubString(size_t& positionInText) const;
	std::pair<size_t, size_t> moveToNextSubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToPreviouslySubString();
	std::pair<size_t, size_t> moveToPreviouslySubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToSubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToSubString(const std::wregex& pattern, bool reverseFind = false);
	std::pair<size_t, size_t> moveToLastSubString();
	std::pair<size_t, size_t> moveToFirstSubString();
	std::wstring returnSubString(std::pair<size_t, size_t> beginEndIndexes) const;
	std::wstring getSubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getSubString(const std::wregex& pattern, bool reverseFind = false);
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
	template<typename T>
	bool createDrawing();
	template<typename T>
	bool tryAddComponent(const std::wstring& componentNumberStr);
	template<typename T>
	bool tryAddPage();
private:
	size_t size_tMax{ std::numeric_limits<size_t>::max() };
};

template<typename T>
inline bool BaseTextParser::createDrawing()
{
	if (lastDrawingPagePtr == nullptr) {
		drawingsPtr->emplace_back();
		currentDrawingPtr = &drawingsPtr->back();
		tryAddPage<T>();
		return true;
	}
	return false;
}

template<typename T>
inline bool BaseTextParser::tryAddComponent(const std::wstring& componentNumberStr)
{
	bool isSuccessful = lastDrawingPagePtr->tryAddComponent<T>(componentNumberStr);
	if (isSuccessful) {
		lastComponentPtr = &lastDrawingPagePtr->getLastComponent();
	}
	return isSuccessful;
}

template<typename T>
inline bool BaseTextParser::tryAddPage()
{
	if (currentDrawingPtr != nullptr)
	{
		currentDrawingPtr->pages.emplace_back(std::make_shared<T>());
		lastDrawingPagePtr = currentDrawingPtr->pages.back();
		componentsEnded = false;
		return true;
	}
	return false;
}


#endif // BASETEXTPARSER_H
