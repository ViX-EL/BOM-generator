#include "TextParserSEC.h"
#include "DrawingPageSEC.h"
#include "BuildComponentSEC.h"
#include "DrawingPage.h"
#include "TextParser.h"
#include "ValuesCheker.h"
#include "StringUtilities.h"

#include <string>
#include <vector>
#include <functional>

bool TextParserSEC::readComponentNumber()
{
	if (componentsEnded) {
		return false;
	}

	std::wstring componentNumberStr(getNextSubString());
	size_t currentPosition = currentPositionInText;
	std::wstring nextSubStr = getNextSubStringFromPosition(currentPosition);
	if (lastDrawingPagePtr != nullptr) 
	{
		if (lastDrawingPagePtr->trySetPages(componentNumberStr, nextSubStr, ValuesCheker::Type::NONE) ||
			std::regex_match(componentNumberStr, lastDrawingPagePtr->getCurrentPagePattern()) &&
			std::regex_match(nextSubStr, lastDrawingPagePtr->getCipherDocumentPattern()))
		{
			componentsEnded = true;
			return false;
		}
	}

	//Если не найден номер компонента
	if (!std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern()))
	{
		if (componentNumberStr.starts_with(L"CUT PIPE") || componentNumberStr.starts_with(L"**********")) {
			componentsEnded = true;
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			moveToNextSubString(L"КОЛ-ВО");
		}
		return false;
	}
	else
	{
		createDrawing<DrawingPageSEC>();

		tryAddComponent<BuildComponentSEC>(componentNumberStr);
		return true;
	}
}

TextParserSEC::TextParserSEC(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

bool TextParserSEC::isEndOfComponent(const std::wstring& stringAfterComponent) const
{
	size_t positionInText = currentPositionInText;
	std::wstring secondStringAfterComponent = getNextSubStringFromPosition(positionInText);

	std::function checkEnd = [this](const std::wstring& subStr) {
		bool isEnd = subStr.starts_with(L"CUT PIPE LENGTH");
		if (!isEnd) {
			isEnd = subStr.starts_with(L"*******");
		}
		if (!isEnd) {
			isEnd = subStr.starts_with(L"ERECTION MATERIALS");
		}
		if (!isEnd) {
			isEnd = std::regex_match(subStr, StringUtilities::getRegex(LR"([a-zA-Z \-\.\/]+ \/ [А-Яа-я \-\.\/]+)"));
		}
		return isEnd;
	};

	bool isEnd = std::regex_match(stringAfterComponent, BuildComponent::getPositionNumberPattern());
	if (isEnd)
	{
		isEnd = std::regex_match(secondStringAfterComponent, lastComponentPtr->getDescriptionPattern()) && !checkEnd(secondStringAfterComponent);
		return isEnd;
	}
	return checkEnd(stringAfterComponent);
}

bool TextParserSEC::readComponent()
{
	if (!readComponentNumber()) {
		return false;
	}
	
	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 4; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}
	bool match[2]{ false, false };

	int descriptionSubStrCount { 1 };
	bool isPositionCodeOnTwoSubStr { false };
	bool isNominalDiameterOnTwoSubStr { false };
	while (!match[0] && !match[1])
	{
		size_t positionInText = currentPositionInText;
		std::wstring afterBufferStr = getNextSubStringFromPosition(positionInText);
		std::wstring secondAfterBufferStr = getNextSubStringFromPosition(positionInText);
		bool endOfComponent = isEndOfComponent(*(subStrBuffer.end() - 1)) || (std::regex_match(afterBufferStr, lastDrawingPagePtr->getTotalPagesPattern()) &&
			std::regex_match(*(subStrBuffer.end() - 1), lastDrawingPagePtr->getCurrentPagePattern()) && 
			std::regex_match(secondAfterBufferStr, lastDrawingPagePtr->getCipherDocumentPattern()) || 
			(std::regex_match(*(subStrBuffer.end() - 1), lastDrawingPagePtr->getCurrentPagePattern()) && 
			std::regex_match(afterBufferStr, lastDrawingPagePtr->getCipherDocumentPattern())));
		if (endOfComponent)
		{
			bool isAmount = std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getAmountPattern());

			match[0] = endOfComponent;
			match[0] = match[0] && isAmount;
			bool isPositionCode = std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getPositionCodePattern());
			if (endOfComponent && isAmount && !isPositionCode && subStrBuffer.size() >= 5)
			{
				std::wstring doubleSubStrPositionCode = *(subStrBuffer.end() - 4) + *(subStrBuffer.end() - 3);
				isPositionCode = std::regex_match(doubleSubStrPositionCode, lastComponentPtr->getPositionCodePattern());
				isPositionCodeOnTwoSubStr = isPositionCode;
				if (subStrBuffer.size() >= 6 && std::regex_match(*(subStrBuffer.end() - 6), StringUtilities::getRegex(LR"(\d{1,4} ?x)")))
				{
					isNominalDiameterOnTwoSubStr = std::regex_match(*(subStrBuffer.end() - 6) + *(subStrBuffer.end() - 5), lastComponentPtr->getNominalDiameterPattern());
					match[0] = match[0] && isNominalDiameterOnTwoSubStr;
				}
				else {
					match[0] = match[0] && std::regex_match(*(subStrBuffer.end() - 5), lastComponentPtr->getNominalDiameterPattern());
				}
			}
			else if (endOfComponent && isAmount && isPositionCode)
			{
				if (subStrBuffer.size() >= 5 && std::regex_match(*(subStrBuffer.end() - 5), StringUtilities::getRegex(LR"(\d{1,4} ?x)")))
				{
					isNominalDiameterOnTwoSubStr = std::regex_match(*(subStrBuffer.end() - 5) + *(subStrBuffer.end() - 4), lastComponentPtr->getNominalDiameterPattern());
					match[0] = match[0] && isNominalDiameterOnTwoSubStr;
				}
				else {
					match[0] = match[0] && std::regex_match(*(subStrBuffer.end() - 4), lastComponentPtr->getNominalDiameterPattern());
				}
			}
			match[0] = match[0] && isPositionCode;

			if (!match[0])
			{
				match[1] = endOfComponent;
				match[1] = match[1] && isAmount;
				if (std::regex_match(*(subStrBuffer.end() - 4), StringUtilities::getRegex(LR"(\d{1,4} ?x)")))
				{
					isNominalDiameterOnTwoSubStr = std::regex_match(*(subStrBuffer.end() - 4) + *(subStrBuffer.end() - 3), lastComponentPtr->getNominalDiameterPattern());
					match[1] = match[1] && isNominalDiameterOnTwoSubStr;
				}
				else {
					match[1] = match[1] && std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getNominalDiameterPattern());
				}
			}
		}

		if (!match[0] && !match[1]) {
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	std::wstring descriptionStr;
	for (int i = 0; i < descriptionSubStrCount; i++)
	{
		std::wstring spase = i == 0 ? L"" : L" ";
		descriptionStr += spase + subStrBuffer[i];
	}
	lastComponentPtr->trySetDescription(descriptionStr);

	if (match[0])
	{
		if (!isPositionCodeOnTwoSubStr)
		{
			if (isNominalDiameterOnTwoSubStr) {
				lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 5) + *(subStrBuffer.end() - 4));
			}
			else {
				lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 4));
			}
			lastComponentPtr->trySetPositionCode(*(subStrBuffer.end() - 3));
		}
		else
		{
			if (isNominalDiameterOnTwoSubStr) {
				lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 6) + *(subStrBuffer.end() - 5));
			}
			else {
				lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 5));
			}
			lastComponentPtr->trySetPositionCode(*(subStrBuffer.end() - 4) + *(subStrBuffer.end() - 3));
		}
		lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 2));
	}
	else if (match[1])
	{
		if (isNominalDiameterOnTwoSubStr) {
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 4) + *(subStrBuffer.end() - 3));
		}
		else {
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 3));
		}
		lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 2));
	}
	moveToPreviouslySubString();
	return true;
}

void TextParserSEC::readTablePartData()
{
	lastDrawingPagePtr->trySetPipelineClass(getSubString(lastDrawingPagePtr->getPipelineClassPattern()));
	lastDrawingPagePtr->trySetOperatingPressure(getNextSubString());
	lastDrawingPagePtr->trySetDesignPressure(getNextSubString());
	lastDrawingPagePtr->trySetOperatingTemperature(getNextSubString());
	lastDrawingPagePtr->trySetDesignTemperature(getNextSubString());
	lastDrawingPagePtr->trySetTestEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetTestPressure(getNextSubString());

	size_t currentPosition = currentPositionInText;
	moveOnCountSubStrFromPosition(currentPosition, 2);
	if (!getNextSubStringFromPosition(currentPosition).starts_with(L":"))
	{
		lastDrawingPagePtr->trySetPaintingSystem(getNextSubString());
	}

	lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getNextSubString());
	lastDrawingPagePtr->trySetWeldInspection(getNextSubString());
	moveToNextSubString();
	lastDrawingPagePtr->trySetSchemeNumber(getSubString(lastDrawingPagePtr->getSchemeNumberPattern()));
	moveToNextSubString();
	lastDrawingPagePtr->trySetGOSTPipelineCategory(getNextSubString());
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getNextSubString());
	std::wstring stressCalculation = getNextSubString();
	if (!lastDrawingPagePtr->trySetStressCalculation(stressCalculation, ValuesCheker::Type::NONE)) {
		lastDrawingPagePtr->trySetTracing(stressCalculation);
	}
	else {
		lastDrawingPagePtr->trySetTracing(getNextSubString());
	}
	lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetDiameterPipeline(getNextSubString());
	moveToNextSubString();
	lastDrawingPagePtr->trySetIsolation(getNextSubString());
	lastDrawingPagePtr->trySetIsometricDrawing(getNextSubString());
	lastDrawingPagePtr->trySetLineNumber(getSubString(lastDrawingPagePtr->getIsometricDrawingPattern()));
}

void TextParserSEC::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	bool isListStartWithContFrom = false;
	if (getFirstSubString().starts_with(L"**********"))
	{
		isListStartWithContFrom	= true;
		moveOnCountSubStr(2);
		size_t currentPosition = currentPositionInText;
		if (getNextSubStringFromPosition(currentPosition).starts_with(L"CUT PIPE")) {
			return;
		}
	}
	else if(getSubString(L"КОЛ-ВО").empty())
	{
		return;
	}

	while (true) //Чтение всех компонентов
	{
		if (!readComponent())
		{
			if (!readComponent()) {
				break;
			}
		}
	}

	if (lastDrawingPagePtr != nullptr)
	{
		lastDrawingPagePtr->trySetCipherDocument(getSubString(lastDrawingPagePtr->getCipherDocumentPattern()));
		if (!isListStartWithContFrom)
		{
			std::wstring totalPagesStr = getPreviouslySubString();
			lastDrawingPagePtr->trySetPages(getPreviouslySubString(), totalPagesStr);
		}
		else
		{
			lastDrawingPagePtr->trySetPages(getPreviouslySubString(), L"0");
		}
		lastDrawingPagePtr->trySetFileName(fileName);
		readTablePartData();

		lastDrawingPagePtr->parseSplitComponentsData();
	}
}
