#include "TextParserPTE.h"
#include "BaseTextParser.h"
#include "DrawingPagePTE.h"
#include "BuildComponentPTE.h"

#include <regex>

bool TextParserPTE::readComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	bool isLineNumber = false;
	if (lastDrawingPagePtr != nullptr) {
		isLineNumber = std::regex_match(componentNumberStr, lastDrawingPagePtr->getLineNumberPattern());
	}
	bool isComponentNumber = std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern());
	if (!isComponentNumber || isLineNumber)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") || isLineNumber) {
			componentsEnded = true;
		}
		return false;
	}
	else
	{
		createDrawing<DrawingPagePTE>();
		tryAddComponent<BuildComponentPTE>(componentNumberStr);
		lastComponentPtr->createEmptySplitData();
		return true;
	}
}

bool TextParserPTE::readComponent()
{
	if (componentsEnded || !readComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 3; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	std::wregex materialPattern(LR"([a-zA-Z0-9 ]+ \/( ?[а-яА-Я0-9 ]+)?|нерж\. сталь \d{1,3}L|-)");
	bool cases[]{ false, false };

	int descriptionSubStrCount{ 1 };
	int materialStrLength{ 1 };
	while (!cases[0] && !cases[1])
	{
		cases[0] = std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getNominalDiameterPattern());
		cases[0] = cases[0] && std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());

		if (!cases[0])
		{
			cases[1] = std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());
			if (subStrBuffer.size() >= 4 && cases[1] && std::regex_match(*(subStrBuffer.end() - 3), materialPattern)) {
				materialStrLength++;
				cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 4), lastComponentPtr->getNominalDiameterPattern());
			}
			else
			{
				cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 2), materialPattern);
				cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getNominalDiameterPattern());
			}
		}

		if (!cases[0] && !cases[1]) {
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
			if ((subStrBuffer.end() - 1)->starts_with(L"CUT PIPE LENGTH") || std::regex_match(*(subStrBuffer.end() - 1), lastDrawingPagePtr->getLineNumberPattern())) {
				throw std::exception("Невозможно распознать компонент!");
			}
		}
	}

	if (cases[0])
	{
		if ((subStrBuffer.end() - 2)->ends_with(L"x")) {
			subStrBuffer.emplace_back(getNextSubString());
		}
	}

	if (cases[1]) {
		descriptionSubStrCount--;
	}
	std::wstring descriptionStr;
	for (int i = 0; i < descriptionSubStrCount; i++)
	{
		descriptionStr += subStrBuffer[i];
	}
	lastComponentPtr->trySetDescription(descriptionStr);

	if (cases[0])
	{
		if ((subStrBuffer.end() - 3)->ends_with(L"x")) {
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 3) + *(subStrBuffer.end() - 2));
		}
		else {
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 2));
		}
	}
	else if (cases[1])
	{
		size_t nominalDiameterIndex{ 3 };
		if (materialStrLength > 1) {
			nominalDiameterIndex++;
			lastComponentPtr->getSplitData()->setSteelGrade(*(subStrBuffer.end() - 3) + L' ' + *(subStrBuffer.end() - 2));
		}
		else {
			lastComponentPtr->getSplitData()->setSteelGrade(*(subStrBuffer.end() - 2));
		}

		if (subStrBuffer.size() > nominalDiameterIndex && ((subStrBuffer.end() - nominalDiameterIndex)->ends_with(L"x"))) {
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - nominalDiameterIndex + 1) + *(subStrBuffer.end() - nominalDiameterIndex));
		}
		else {
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - nominalDiameterIndex));
		}
	}
	lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 1));
	return true;
}

void TextParserPTE::readTablePartData()
{
	moveToPreviouslySubString(L"Prep/Разраб.");
	if (lastDrawingPagePtr->getTotalPages() == 0)
	{
		std::wstring totalPageStr = getPreviouslySubString();
		lastDrawingPagePtr->trySetPages(getPreviouslySubString(), totalPageStr);
	}
	lastDrawingPagePtr->trySetLineNumber(getPreviouslySubString());
	
	lastDrawingPagePtr->trySetSchemeNumber(getSubString(L"Номер схемы").erase(0, 14));
	lastDrawingPagePtr->trySetIsometricDrawing(getSubString(L"Изометрический чертеж").erase(0, 22));
	lastDrawingPagePtr->trySetCipherDocument(getNextSubString());
	lastDrawingPagePtr->trySetDesignPressure(getSubString(lastDrawingPagePtr->getDesignPressurePattern()));
	lastDrawingPagePtr->trySetDesignTemperature(getNextSubString());
	lastDrawingPagePtr->trySetOperatingPressure(getNextSubString());
	lastDrawingPagePtr->trySetOperatingTemperature(getNextSubString());
	lastDrawingPagePtr->trySetTestPressure(getNextSubString());
	lastDrawingPagePtr->trySetTestEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetWeldInspection(getNextSubString());
	lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getNextSubString());
	lastDrawingPagePtr->trySetStressCalculation(getNextSubString());
	lastDrawingPagePtr->trySetPaintingSystem(getNextSubString());
	lastDrawingPagePtr->trySetTracing(getNextSubString());
	lastDrawingPagePtr->trySetIsolation(getNextSubString());
	lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getNextSubString());
	lastDrawingPagePtr->trySetGOSTPipelineCategory(getNextSubString());
	lastDrawingPagePtr->trySetDiameterPipeline(getNextSubString());
	lastDrawingPagePtr->trySetPipelineClass(getNextSubString());
}

TextParserPTE::TextParserPTE(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

void TextParserPTE::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	if (text->find(L"ERECTION MATERIALS", currentPositionInText) != std::wstring::npos) {
		moveToNextSubString(L"КОЛ-ВО");
	}
	else {
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
		readTablePartData();

		lastDrawingPagePtr->trySetFileName(fileName);

		lastDrawingPagePtr->parseSplitComponentsData();
	}
}
