#pragma once

#ifndef BASETEXTPARSER_H
#define BASETEXTPARSER_H

#include "DrawingPage.h"
#include "BuildComponent.h"

#include <cassert>
#include <string>
#include <vector>
#include <utility>
#include <regex>
#include <memory>
#include <initializer_list>

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
	std::pair<size_t, size_t> moveToNextSubStringFromPosition(size_t& positionInText) const;
	std::pair<size_t, size_t> moveToPreviouslySubStringFromPosition(size_t& positionInText) const;
	std::pair<size_t, size_t> moveToNextSubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToPreviouslySubString();
	std::pair<size_t, size_t> moveToPreviouslySubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToSubString(const std::wstring& subString, bool reverseFind = false);
	std::pair<size_t, size_t> moveToSubString(const std::wregex& pattern, bool reverseFind = false);
	std::pair<size_t, size_t> moveToSubString(const std::wregex& pattern, const std::wregex& stopPattern, bool reverseFind = false);
	std::pair<size_t, size_t> moveUntil(std::initializer_list<std::wregex> patterns, bool reverseFind = false);
	std::pair<size_t, size_t> moveToLastSubString();
	std::pair<size_t, size_t> moveToFirstSubString();
	std::wstring returnSubString(std::pair<size_t, size_t> beginEndIndexes) const;
	std::wstring getSubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getSubString(const std::wregex& pattern, bool reverseFind = false);
	std::wstring getSubString(const std::wregex& pattern, const std::wregex& stopPattern, bool reverseFind = false);
	std::wstring getNextSubString();
	std::wstring getNextSubStringFromPosition(size_t& positionInText) const;
	std::wstring getNextSubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getPreviouslySubString(const std::wstring& subString, bool reverseFind = false);
	std::wstring getPreviouslySubStringFromPosition(size_t& positionInText) const;
	std::wstring getPreviouslySubString();
	std::wstring getLastSubString();
	std::wstring getFirstSubString();
	std::pair<size_t, size_t> moveOnCountSubStr(size_t count, bool reverse = false);
	std::pair<size_t, size_t> moveOnCountSubStrFromPosition(size_t& positionInText, int count, bool reverse = false) const;

	bool tryMoveToNextSubStringFromPosition(size_t& positionInText) const;
	bool tryMoveToPreviouslySubStringFromPosition(size_t& positionInText) const;
	bool tryMoveOnCountSubStrFromPosition(size_t& positionInText, int count, bool reverse) const;

	bool searchForMatchesInFollowing(const std::wregex& pattern, int subStrsCount) const;

	virtual void reset();
	template<typename T>
	bool createDrawing(bool inputCheckOff = false);
	template<typename T>
	bool tryAddComponent(const std::wstring& componentNumberStr);
	template<typename T>
	bool tryAddComponent(int componentNumber);
	template<typename T>
	bool tryAddPage(bool inputCheckOff = false);
};

template<typename T>
inline bool BaseTextParser::createDrawing(bool inputCheckOff)
{
	if (lastDrawingPagePtr == nullptr) 
	{
		assert(drawingsPtr != nullptr && "drawingsPtr is nullptr!");
		drawingsPtr->emplace_back();
		currentDrawingPtr = &drawingsPtr->back();
		tryAddPage<T>(inputCheckOff);
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
inline bool BaseTextParser::tryAddComponent(int componentNumber)
{
	bool isSuccessful = lastDrawingPagePtr->tryAddComponent<T>(componentNumber);
	if (isSuccessful) {
		lastComponentPtr = &lastDrawingPagePtr->getLastComponent();
	}
	return isSuccessful;
}

template<typename T>
inline bool BaseTextParser::tryAddPage(bool inputCheckOff)
{
	if (currentDrawingPtr != nullptr)
	{
		currentDrawingPtr->pages.emplace_back(std::make_shared<T>(inputCheckOff));
		lastDrawingPagePtr = currentDrawingPtr->pages.back();
		componentsEnded = false;
		return true;
	}
	return false;
}


#endif // BASETEXTPARSER_H
