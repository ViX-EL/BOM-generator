#include "TextParserIOT.h"
#include "DrawingPageIOT.h"
#include "BuildComponentIOT.h"

#include <regex>
#include <string>

TextParserIOT::TextParserIOT(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

bool TextParserIOT::readComponent()
{
	if (!readComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 4; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}
	bool match[2]{ false, false };

	int descriptionSubStrCount{ 1 };
	while (!match[0] && !match[1])
	{
		match[0] = std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getNominalDiameterPattern());
		match[0] = match[0] && std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getPositionCodePattern());
		match[0] = match[0] && std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());

		if (!match[0])
		{
			match[1] = std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getNominalDiameterPattern());
			match[1] = match[1] && std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getAmountPattern());
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
		lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 3));
		lastComponentPtr->trySetPositionCode(*(subStrBuffer.end() - 2));
		lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 1));
	}
	else if (match[1])
	{
		lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 3));
		lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 2));
		moveToPreviouslySubString();
	}
	return true;
}

bool TextParserIOT::readComponentNumber()
{
	if (componentsEnded) {
		return false;
	}

	std::wstring componentNumberStr(getNextSubString());
	//Если не найден номер компонента
	if (!std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern())) 
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") || componentNumberStr.starts_with(L"GCC-IOT-DDD")) {
			componentsEnded = true;
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			moveToNextSubString(L"КОЛ-ВО");
		}
		return false;
	}
	else
	{
		componentsCount++;
		createDrawing<DrawingPageIOT>();

		tryAddComponent<BuildComponentIOT>(componentsCount);
		return true;
	}
}

void TextParserIOT::readTablePartData()
{
	moveToPreviouslySubString(L"Газохимический комплекс");
	lastDrawingPagePtr->trySetPipelineClass(getSubString(lastDrawingPagePtr->getPipelineClassPattern(), true));
	lastDrawingPagePtr->trySetDiameterPipeline(getPreviouslySubString());
	if (!lastDrawingPagePtr->trySetIsolation(getPreviouslySubString(), false)) {
		lastDrawingPagePtr->trySetIsolation(getSubString(lastDrawingPagePtr->getIsolationPattern(), true));
	}
	lastDrawingPagePtr->trySetTracing(getPreviouslySubString());
	lastDrawingPagePtr->trySetOperatingTemperature(getPreviouslySubString());
	lastDrawingPagePtr->trySetDesignTemperature(getPreviouslySubString());
	lastDrawingPagePtr->trySetOperatingPressure(getPreviouslySubString());
	lastDrawingPagePtr->trySetTestPressure(getPreviouslySubString());
	lastDrawingPagePtr->trySetDesignPressure(getPreviouslySubString());
	moveToPreviouslySubString();
	lastDrawingPagePtr->trySetSchemeNumber(getPreviouslySubString());

	std::wstring isometricDrawingStr = getSubString(L"Isometric drawing", true);
	if (isometricDrawingStr != L"") {
		lastDrawingPagePtr->trySetIsometricDrawing(isometricDrawingStr.erase(0, 18));
	}
	std::wstring fileNameStr = getSubString(lastDrawingPagePtr->getFileNamePattern(), lastDrawingPagePtr->getLineNumberPattern(), true);
	if (fileNameStr != L"") {
		lastDrawingPagePtr->trySetFileName(fileNameStr);
	}
	lastDrawingPagePtr->trySetLineNumber(getSubString(lastDrawingPagePtr->getLineNumberPattern(), lastDrawingPagePtr->getCipherDocumentPattern(), true));
	lastDrawingPagePtr->trySetCipherDocument(getSubString(lastDrawingPagePtr->getCipherDocumentPattern(), true));
	moveToNextSubString();
	std::wstring currentPageStr = getNextSubString();
	std::wstring totalPagesStr = getNextSubString();
	if (std::regex_match(totalPagesStr, lastDrawingPagePtr->getPagesPattern())) {
		lastDrawingPagePtr->trySetPages(currentPageStr, totalPagesStr);
	}
	else {
		lastDrawingPagePtr->trySetPages(currentPageStr, currentPageStr);
	}
	std::wstring paintingSystemStr = getNextSubString(L"ТЕХНОЛОГИИ");
	if (paintingSystemStr == L"") {
		paintingSystemStr = getNextSubString(L"Беларусь");
	}
	if (lastDrawingPagePtr->trySetPaintingSystem(paintingSystemStr, false)) {
		lastDrawingPagePtr->trySetTestEnvironment(getNextSubString());
	}
	else {
		lastDrawingPagePtr->trySetTestEnvironment(paintingSystemStr);
	}
	lastDrawingPagePtr->trySetWeldInspection(getNextSubString());
	lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getNextSubString());
	lastDrawingPagePtr->trySetStressCalculation(getNextSubString());
	lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getNextSubString());
	lastDrawingPagePtr->trySetGOSTPipelineCategory(getNextSubString());

	if (isometricDrawingStr == L"") {
		lastDrawingPagePtr->trySetIsometricDrawing(getSubString(L"Isometric drawing").erase(0, 18));
	}

	if (fileNameStr == L"") {
		lastDrawingPagePtr->trySetFileName(getSubString(lastDrawingPagePtr->getFileNamePattern()));
	}
}

void TextParserIOT::reset()
{
	componentsCount = 0;
	BaseTextParser::reset();
}

void TextParserIOT::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	if (getNextSubString(L"КОЛ-ВО") == L"") {
		componentsEnded = true;
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

	if (lastDrawingPagePtr != nullptr) {
		readTablePartData();
		lastDrawingPagePtr->parseSplitComponentsData();
	}
}