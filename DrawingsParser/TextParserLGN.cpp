#include "TextParserLGN.h"
#include "StringConvert.h"
#include "StringUtilities.h"
#include "BuildComponentLGN.h"
#include "DrawingPageLGN.h"
#include "StringUtilities.h"

#include <functional>
#include <regex>

bool TextParserLGN::readComponentNumber()
{
	bool thisDocumentStrFinded = searchForMatchesInFollowing(std::wregex (L"This document is"), 3);

	std::wstring componentNumberStr(getNextSubString());

	if (!std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern()) || thisDocumentStrFinded)
	{
		if (thisDocumentStrFinded || componentNumberStr.starts_with(L"CUT PIPE LENGTH") ||
			componentNumberStr.starts_with(L"*******")) {
			if (componentNumberStr.starts_with(L"*******")) {
				lineBreak = true;
			}
			componentsEnded = true;
		}
		return false;
	}
	else 
	{		
		tryAddComponent<BuildComponentLGN>(componentNumberStr);
		return true;
	}
}

bool TextParserLGN::readComponent()
{
	if (componentsEnded || !readComponentNumber()) {
		return false;
	}

	std::wstring descriptionStr(getNextSubString());
	std::wstring diameterStr(getNextSubString());
	while (!readDescriptionAndDiameter(descriptionStr, diameterStr))
	{
		descriptionStr += diameterStr;
		diameterStr = getNextSubString();
	}

	readDocument();
	lastComponentPtr->trySetAmount(getNextSubString());
	return true;
}

bool TextParserLGN::readDescriptionAndDiameter(const std::wstring& descriptionStr, std::wstring& diameterStr)
{
	if (std::regex_match(diameterStr, lastComponentPtr->getNominalDiameterPattern()))
	{
		lastComponentPtr->trySetDescription(descriptionStr);
		if (text->at(currentPositionInText + 3) == L'x')
		{
			diameterStr += getNextSubString();
			diameterStr += getNextSubString();
		}
		else if (text->at(currentPositionInText + 2) == L'x') {
			diameterStr += getNextSubString();
			if (diameterStr.ends_with(L"x")) {
				diameterStr += getNextSubString();
			}
		}
		else if (diameterStr.ends_with(L"x")) {
			diameterStr += getNextSubString();
		}

		lastComponentPtr->trySetNominalDiameter(diameterStr);
		return true;
	}
	else {
		return false;
	}
}

void TextParserLGN::readDocument()
{
	std::vector<std::wstring> subStringsBuffer;
	size_t position = currentPositionInText;
	for (int i{ 0 }; i < 3; i++) {
		subStringsBuffer.emplace_back(getNextSubString(position));
	}
	std::wstring documentStr = *(subStringsBuffer.begin());

	if (subStringsBuffer[0] == L"–" || std::regex_match(subStringsBuffer[0], lastComponentPtr->getAmountPattern()) || subStringsBuffer[0] == L"None") 
	{
		if (subStringsBuffer[0] == L"–" || subStringsBuffer[0] == L"None") {
			moveToNextSubString();
		}
		return;
	}

	// Подсчёт количества подстрок, которые составляют строку документа компонента
	size_t documentSubStrCount{ 1 };
	while (true)
	{
		bool matches[3]{ false };
		if ((subStringsBuffer.end() - 1)->starts_with(L"This document is"))
		{
			//documentSubStrCount -= 2;
			matches[0] = true; matches[1] = true; matches[2] = true;
		}
		else
		{
			matches[0] = std::regex_match(documentStr, lastComponentPtr->getDocumentPattern());
			matches[1] = std::regex_match(*(subStringsBuffer.end() - 2), lastComponentPtr->getAmountPattern());
			matches[2] = isEndOfComponent(*(subStringsBuffer.end() - 1), position);
		}

		if (matches[0] && matches[1] && matches[2])
		{
			lastComponentPtr->trySetDocument(documentStr);
			moveOnCountSubStr(documentSubStrCount);
			break;
		}
		else 
		{
			documentStr = documentStr + *(subStringsBuffer.begin() + documentSubStrCount);
			subStringsBuffer.push_back(getNextSubString(position));
			documentSubStrCount++;
		}
	}
}

bool TextParserLGN::isEndOfComponent(const std::wstring& stringAfterComponent, size_t positionInText) const
{
	std::function checkEnd = [](const std::wstring& subStr) 
	{
		bool isEnd = subStr.starts_with(L"CUT PIPE LENGTH");
		if (!isEnd) {
			isEnd = std::regex_match(subStr, StringUtilities::getRegex(LR"([A-Z ]+\/[А-Я ]+)"));
		}
		if (!isEnd) {
			isEnd = subStr.starts_with(L"*******");
		}
		return isEnd;
	};

	bool isEnd = std::regex_match(stringAfterComponent, BuildComponent::getPositionNumberPattern());
	if (isEnd)
	{
		std::wstring secondStringAfterComponent = getNextSubString(positionInText);
		isEnd = std::regex_match(secondStringAfterComponent, lastComponentPtr->getDescriptionPattern()) && !checkEnd(secondStringAfterComponent);
		if (!isEnd) {
			isEnd = std::regex_match(stringAfterComponent, lastDrawingPagePtr->getPagesPattern()) && std::regex_match(secondStringAfterComponent, lastDrawingPagePtr->getPagesPattern());
		}
		return isEnd;
	}
	return checkEnd(stringAfterComponent);
}

bool TextParserLGN::findCountStr()
{
	std::wstring findedCountStr;
	if (lineBreak)
	{
		for (int i = 0; i < 2; i++) {
			findedCountStr = getNextSubString(L"*******");
		}
		lineBreak = false;
		if (std::regex_match(findedCountStr, BuildComponent::getPositionNumberPattern())) {
			moveToPreviouslySubString();
		}
		else {
			findedCountStr = getNextSubString(L"КОЛ-ВО");
		}
	}
	else {
		findedCountStr = getNextSubString(L"КОЛ-ВО");
	}

	if (findedCountStr.empty()) { // Если не найдено
		return false;
	}

	if (!createDrawing<DrawingPageLGN>()) {
		tryAddPage<DrawingPageLGN>();
	}
	return true;
}

bool TextParserLGN::readList()
{
	if (!findCountStr()) {
		return false;
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

	bool haveOnlyOneList = isHaveOnlyOneList();

	if (haveOnlyOneList) {
		lastDrawingPagePtr->trySetPages(L"1", L"1");
	}
	else {
		std::wstring thisDocumentStr = getSubString(L"Настоящий документ");
		moveOnCountSubStr(2, true);
		std::wstring pagesStr = getPreviouslySubString();
		std::wstring pageStr = getPreviouslySubString();
		lastDrawingPagePtr->trySetPages(pageStr, pagesStr);
	}

	moveToSubString(L"НА ПЛОЩАДКЕ");
	std::wstring currentSubStr;
	while (!std::regex_match(currentSubStr, StringUtilities::getRegex(LR"(GCC-LGN-DDD-\d+-\d+-\d+-\w+-\w+-\d+)"))) {
		currentSubStr = getNextSubString();
	}

	lastDrawingPagePtr->trySetDesignPressure(getNextSubString());
	lastDrawingPagePtr->trySetDesignTemperature(getNextSubString());
	lastDrawingPagePtr->trySetGOSTPipelineCategory(getNextSubString());
	lastDrawingPagePtr->trySetTestPressure(getNextSubString());
	std::wstring weldInspection = getNextSubString();
	std::wstring postWeldingHeatTreatmentStr;
	if (!lastDrawingPagePtr->trySetWeldInspection(weldInspection, false)) {
		postWeldingHeatTreatmentStr = weldInspection;
	}
	else {
		postWeldingHeatTreatmentStr = getNextSubString();
	}
	if (!lastDrawingPagePtr->trySetPostWeldingHeatTreatment(postWeldingHeatTreatmentStr, false)) {
		lastDrawingPagePtr->trySetPaintingSystem(postWeldingHeatTreatmentStr);
	}
	else {
		lastDrawingPagePtr->trySetPaintingSystem(getNextSubString());
	}

	std::wstring testEnvironmentStr = getNextSubString();
	if (testEnvironmentStr == L"Воздух/Азот") {
		lastDrawingPagePtr->trySetTestEnvironment(testEnvironmentStr + L' ' + getNextSubString());
	}
	else {
		lastDrawingPagePtr->trySetTestEnvironment(testEnvironmentStr);
	}

	lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetPipelineClass(getNextSubString());
	moveOnCountSubStr(2);
	lastDrawingPagePtr->trySetTracing(getNextSubString());
	std::wstring operatingPressureStr = getNextSubString();
	if (!lastDrawingPagePtr->trySetOperatingPressure(operatingPressureStr, false)) {
		lastDrawingPagePtr->trySetOperatingTemperature(operatingPressureStr);
	}
	else {
		lastDrawingPagePtr->trySetOperatingTemperature(getNextSubString());
	}

	std::wstring cipherDocumentStr(getPreviouslySubString(L"Газохимический комплекс"));
	if (std::regex_match(cipherDocumentStr, lastDrawingPagePtr->getCipherDocumentPattern())) {
		lastDrawingPagePtr->trySetCipherDocument(getPreviouslySubString(L"Газохимический комплекс"));
	}
	else {
		lastDrawingPagePtr->trySetCipherDocument(getSubString(lastDrawingPagePtr->getCipherDocumentPattern()));
	}
	lastDrawingPagePtr->trySetDiameterPipeline(getSubString(L"DN"));
	lastDrawingPagePtr->trySetIsolation(getNextSubString());
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getNextSubString());
	moveToNextSubString();

	lastDrawingPagePtr->trySetIsometricDrawing(getNextSubString(L"Изометрический чертеж"));
	moveToPreviouslySubString();
	std::wstring stressCalculationStr = getPreviouslySubString();
	if (!lastDrawingPagePtr->trySetStressCalculation(stressCalculationStr, false)) {
		lastDrawingPagePtr->trySetLineNumber(stressCalculationStr);
	}
	else {
		lastDrawingPagePtr->trySetLineNumber(getPreviouslySubString());
	}

	std::wstring schemeNumber(getPreviouslySubString());
	if (!schemeNumber.starts_with(L"* (See note /")) {
		lastDrawingPagePtr->trySetSchemeNumber(schemeNumber);
	}

	lastDrawingPagePtr->trySetFileName(getSubString(L"Имя файла"));
	if (schemeNumber.starts_with(L"* (See note /")) {
		moveOnCountSubStr(3);
		schemeNumber = getNextSubString();
		schemeNumber.erase(0, 3);
		lastDrawingPagePtr->trySetSchemeNumber(schemeNumber, false);
	}
	return true;
}

bool TextParserLGN::isHaveOnlyOneList()
{
	size_t currPosition = 0;

	for (size_t i = 0; i < 2; i++)
	{
		currPosition = text->find(L"This document", currPosition) + 1;
	}
	if (currPosition - 1 == std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}

void TextParserLGN::reset()
{
	lineBreak = false;
	currentDrawingPtr = nullptr;
	BaseTextParser::reset();
}

TextParserLGN::TextParserLGN(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

void TextParserLGN::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	this->drawingsPtr = &drawings;
	reset();

	bool success{ false };
	do {
		success = readList();
		lastDrawingPagePtr->parseSplitComponentsData();
	} while (success);
}