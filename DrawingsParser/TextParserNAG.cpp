#include "TextParserNAG.h"
#include "DrawingPageNAG.h"
#include "BuildComponentNAG.h"
#include "DrawingPage.h"
#include "StringUtilities.h"
#include "TextParser.h"
#include "ValuesCheker.h"

#include <regex>
#include <functional>
#include <initializer_list>
#include <string>
#include <vector>
#include <wx/log.h>

bool TextParserNAG::readComponentNumber()
{
	if (componentsEnded) {
		return false;
	}

	std::wstring componentNumberStr(getNextSubString());
	//Если не найден номер компонента
	bool isLineNumber = false;
	if (lastDrawingPagePtr != nullptr) {
		isLineNumber = std::regex_match(componentNumberStr, lastDrawingPagePtr->getLineNumberPattern());
	}
	if (!std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern()) || isLineNumber)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") || componentNumberStr.starts_with(L"UT PIPE LENGTH"))
		{
			if (lastDrawingPagePtr != nullptr)
			{
				componentsEnded = true;
				std::wstring formatStr = getPreviouslySubString(L"LEGEND");
				if (formatStr.starts_with(L"Формат")) {
					moveOnCountSubStr(2, true);
				}
				else {
					moveToPreviouslySubString();
				}

				std::wstring pagesCountStr(getPreviouslySubString());
				if (std::regex_match(pagesCountStr, lastDrawingPagePtr->getTotalPagesPattern()))
				{
					lastDrawingPagePtr->trySetPages(getPreviouslySubString(), pagesCountStr);
					lastDrawingPagePtr->trySetLineNumber(getPreviouslySubString());
					moveOnCountSubStr(2);
				}
				else {
					lastDrawingPagePtr->trySetLineNumber(pagesCountStr);
				}
			}
			else {
				currentListEmpty = true;
			}
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			moveToSubString(L"КОЛ-ВО");
		}
		else if (isLineNumber) {
			lastDrawingPagePtr->trySetLineNumber(componentNumberStr);
			std::wstring pageStr(getNextSubString());
			if (std::regex_match(pageStr, lastDrawingPagePtr->getCurrentPagePattern())) {
				lastDrawingPagePtr->trySetPages(pageStr, getNextSubString());
			}
			componentsEnded = true;
		}
		return false;
	}
	else
	{

		createDrawing<DrawingPageNAG>();
		tryAddComponent<BuildComponentNAG>(componentNumberStr);
		return true;
	}
}

void TextParserNAG::writeValueOfTwoSubStr(bool(DrawingPage::* trySetFunction)(const std::wstring&, ValuesCheker::Type), std::initializer_list<std::wstring> ifEqualStrList,
	const std::wstring& firstStr, ValuesCheker::Type checkType)
{
	std::wstring subStr;
	if (firstStr == L"") {
		subStr = getNextSubString();
	}
	else {
		subStr = firstStr;
	}
	for (const std::wstring& currentTemplateStr : ifEqualStrList)
	{
		if (subStr == currentTemplateStr) {
			subStr = subStr + getNextSubString();
			break;
		}
	}
	(*lastDrawingPagePtr.*trySetFunction)(subStr, checkType);
}

void TextParserNAG::reset()
{
	currentListEmpty = false;
	BaseTextParser::reset();
}

void TextParserNAG::readTablePartData()
{
	std::wstring schemeNumber(getNextSubString(L"ЛИСТОВ"));
	bool cipherWasFoundEarlier = false;
	if (std::regex_match(schemeNumber, lastDrawingPagePtr->getCipherDocumentPattern()))
	{
		cipherWasFoundEarlier = true;
		lastDrawingPagePtr->trySetCipherDocument(schemeNumber);
		moveToNextSubString();
		lastDrawingPagePtr->trySetIsometricDrawing(getNextSubString());
		std::wstring totalPagesStr = getPreviouslySubString(L"Формат", true);
		std::wstring currentPageStr = getPreviouslySubString();
		lastDrawingPagePtr->trySetPages(currentPageStr, totalPagesStr);
		schemeNumber = getNextSubString(L"КЛАСС ТРУБОПРОВОДА", true);
	}

	bool isFlareFarming = false;
	if (schemeNumber.starts_with(L"GCC-NAG-DDD")) {
		lastDrawingPagePtr->trySetSchemeNumber(schemeNumber);
		writeValueOfTwoSubStr(&DrawingPage::trySetDesignPressure, { L"HYDROSTATIC/" });
	}
	else
	{
		std::wregex flareFarmingPattern(L"Факельное хозяйство");
		if (std::regex_search(*text, flareFarmingPattern) && !std::regex_match(schemeNumber, lastDrawingPagePtr->getDesignPressurePattern())) {
			isFlareFarming = true;
		}
		else {
			writeValueOfTwoSubStr(&DrawingPage::trySetDesignPressure, { L"HYDROSTATIC/" }, schemeNumber);
		}
	}

	if (isFlareFarming && !std::regex_match(schemeNumber, lastDrawingPagePtr->getDesignPressurePattern()))
	{
		writeValueOfTwoSubStr(&DrawingPage::trySetTestEnvironment, { L"PLANT AIR /", L"FW /" }, schemeNumber);
		lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getNextSubString());
		lastDrawingPagePtr->trySetStressCalculation(getNextSubString());
		lastDrawingPagePtr->trySetPaintingSystem(getNextSubString());
		lastDrawingPagePtr->trySetTracing(getNextSubString());
		lastDrawingPagePtr->trySetIsolation(getNextSubString());
		lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
		lastDrawingPagePtr->trySetPipelineClass(getNextSubString());
	}
	else
	{
		lastDrawingPagePtr->trySetDesignTemperature(getNextSubString());
		writeValueOfTwoSubStr(&DrawingPage::trySetOperatingPressure, { L"HYDROSTATIC/" });
		writeValueOfTwoSubStr(&DrawingPage::trySetOperatingTemperature, { L"Не ниже 5 /" });
		lastDrawingPagePtr->trySetTestPressure(getNextSubString());
		writeValueOfTwoSubStr(&DrawingPage::trySetTestEnvironment, { L"PLANT AIR /", L"FW /" });
		writeValueOfTwoSubStr(&DrawingPage::trySetWeldInspection, { L"2 метода объем" });
		lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getNextSubString());
		lastDrawingPagePtr->trySetStressCalculation(getNextSubString());
		lastDrawingPagePtr->trySetPaintingSystem(getNextSubString());
		lastDrawingPagePtr->trySetTracing(getNextSubString());
		std::wstring isolation(getNextSubString());
		if (!lastDrawingPagePtr->trySetIsolation(isolation, ValuesCheker::Type::NONE)) {
			lastDrawingPagePtr->trySetTechnologicalEnvironment(isolation);
		}
		else {
			lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
		}
		lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getNextSubString());
		lastDrawingPagePtr->trySetGOSTPipelineCategory(getNextSubString());
		lastDrawingPagePtr->trySetDiameterPipeline(getNextSubString());
		lastDrawingPagePtr->trySetPipelineClass(getNextSubString());
	}

	if (!cipherWasFoundEarlier) 
	{
		lastDrawingPagePtr->trySetCipherDocument(getNextSubString());
		lastDrawingPagePtr->trySetIsometricDrawing(getNextSubString(L"Isometric drawing"));
	}
}

TextParserNAG::TextParserNAG(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

bool TextParserNAG::isEndOfComponent(const std::wstring& stringAfterComponent) const
{
	std::function checkEnd = [this](const std::wstring& subStr) {
		bool isEnd = subStr.starts_with(L"CUT PIPE LENGTH");
		if (!isEnd) {
			isEnd = subStr.starts_with(L"*******");
		}
		if (!isEnd) {
			isEnd = subStr.starts_with(L"ERECTION MATERIALS");
		}
		if (!isEnd) {
			isEnd = regex_match(subStr, lastDrawingPagePtr->getLineNumberPattern());
		}
		return isEnd;
	};

	bool isEnd = std::regex_match(stringAfterComponent, BuildComponent::getPositionNumberPattern());
	if (isEnd)
	{
		size_t positionInText = currentPositionInText;
		std::wstring secondStringAfterComponent = getNextSubStringFromPosition(positionInText);
		isEnd = std::regex_match(secondStringAfterComponent, lastComponentPtr->getDescriptionPattern()) && !checkEnd(secondStringAfterComponent);
		return isEnd;
	}
	return checkEnd(stringAfterComponent);
}

bool TextParserNAG::readComponent()
{
	if (!readComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 4; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	bool cases[]{ false, false };


	int descriptionSubStrCount{ 1 };
	while (!cases[0] && !cases[1])
	{
		cases[0] = std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getNominalDiameterPattern());
		cases[0] = cases[0] && std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getAmountPattern());
		cases[0] = cases[0] && isEndOfComponent(*(subStrBuffer.end() - 1));

		if (!cases[0])
		{
			cases[1] = std::regex_match(*(subStrBuffer.end() - 4), lastComponentPtr->getNominalDiameterPattern());
			cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getPositionCodePattern());
			cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getAmountPattern());
			cases[1] = cases[1] && isEndOfComponent(*(subStrBuffer.end() - 1));
		}

		if (!cases[0] && !cases[1]) {
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	if (!(subStrBuffer.end() - 2)->starts_with(L' ') && !(subStrBuffer.end() - 3)->starts_with(L' ') && !cases[1]) {
		size_t currentPos = currentPositionInText;
		std::vector<std::wstring> buffer;
		buffer.emplace_back(getNextSubStringFromPosition(currentPos));
		buffer.emplace_back(getNextSubStringFromPosition(currentPos));
		if ((buffer.end() - 1)->starts_with(L"CUT PIPE"))
		{
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	if (std::regex_match(*(subStrBuffer.end() - 4), StringUtilities::getRegex(LR"(\d{1,4} ?x)"))) {
		descriptionSubStrCount--;
	}

	if (cases[1]) {
		descriptionSubStrCount--;
	}

	std::wstring descriptionStr;
	if (!(cases[1] && subStrBuffer.size() == 4)) {
		for (int i = 0; i < descriptionSubStrCount; i++)
		{
			descriptionStr += subStrBuffer[i];
		}
		lastComponentPtr->trySetDescription(descriptionStr);
	}

	if (cases[0])
	{
		if (std::regex_match(*(subStrBuffer.end() - 4), StringUtilities::getRegex(LR"(\d{1,4} ?x)"))) {
			lastComponentPtr->trySetNominalDiameter((*(subStrBuffer.end() - 4) + L' ' + (*(subStrBuffer.end() - 3))));
		}
		else
		{
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 3));
		}
		lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 2));
	}
	else if (cases[1])
	{
		if (subStrBuffer.size() != 4 && (subStrBuffer.end() - 5)->ends_with(L"x")) {
			lastComponentPtr->trySetNominalDiameter((*(subStrBuffer.end() - 5) + L' ' + (*(subStrBuffer.end() - 4))));
		}
		else {
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 4));
		}
		lastComponentPtr->trySetPositionCode(*(subStrBuffer.end() - 3));
		lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 2));
	}
	moveToPreviouslySubString();
	return true;
}

void TextParserNAG::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	if (text->find(L"FABRICATION MATERIALS", currentPositionInText) != std::wstring::npos || text->find(L"ERECTION MATERIALS", currentPositionInText) != std::wstring::npos) {
		moveToSubString(L"КОЛ-ВО");
	}
	else if (text->find(L"*********", currentPositionInText) != std::wstring::npos)
	{
		moveToSubString(L"*********");
		moveOnCountSubStr(2);
	}
	else if (std::regex_match(getFirstSubString(), StringUtilities::getRegex(LR"(\d{5}-\w{2}-\d{4}\/\d{3}-\w{2,4}-\d{4}-[0-9A-Z]{8,9}-\d{2})"))) // Если первая строка - номер линии
	{
		componentsEnded = true;
		currentListEmpty = true;
	}
	else {
		currentPositionInText = 0;
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

	if (!currentListEmpty)
	{
		readTablePartData();

		lastDrawingPagePtr->trySetFileName(fileName);

		lastDrawingPagePtr->parseSplitComponentsData();
	}
	else {
		currentListEmpty = false;
		wxLogMessage("[Запись] Отсутствуют записываемые листы в файле %s", fileName);
	}
}
