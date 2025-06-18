#include "TextParserOZN.h"
#include "BaseTextParser.h"
#include "DrawingPageOZN.h"
#include "BuildComponentOZN.h"

#include <regex>

bool TextParserOZN::readComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	size_t currentPosition = currentPositionInText;
	std::wstring nextSubStr(getNextSubStringFromPosition(currentPosition));
	if (lastDrawingPagePtr != nullptr && std::regex_match(componentNumberStr, lastDrawingPagePtr->getCurrentPagePattern()) && 
		std::regex_match(componentNumberStr, lastDrawingPagePtr->getTotalPagesPattern()))
	{
		componentsEnded = true;
		return false;
	}
	bool isComponentNumber = std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern());
	if (!isComponentNumber)
	{
		if (componentNumberStr.starts_with(L"ТРУБНЫЕ СБОРКИ") || componentNumberStr.starts_with(L"**********")) {
			componentsEnded = true;
		}
		if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			moveToNextSubString(L"КОЛ-ВО");
		}
		return false;
	}
	else
	{
		createDrawing<DrawingPageOZN>();
		tryAddComponent<BuildComponentOZN>(componentNumberStr);
		return true;
	}
}

bool TextParserOZN::readComponent()
{
	if (componentsEnded || !readComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 3; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}
	bool match { false };
	int descriptionSubStrCount{ 1 };
	while (!match)
	{
		match = std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getNominalDiameterPattern());
		match = match && std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());

		if (!match)
		{
			ValuesCheker::CheckCondition(!((subStrBuffer.end() - 1)->starts_with(L"ТРУБНЫЕ СБОРКИ") || (subStrBuffer.end() - 1)->starts_with(L"**********")),
				"Невозможно определить компонент!");
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	if ((subStrBuffer.end() - 2)->ends_with(L"x")) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	std::wstring descriptionStr;
	for (int i = 0; i < descriptionSubStrCount; i++)
	{
		descriptionStr += subStrBuffer[i];
	}
	lastComponentPtr->trySetDescription(descriptionStr);


	if ((subStrBuffer.end() - 3)->ends_with(L"x")) {
		lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 3) + *(subStrBuffer.end() - 2));
	}
	else {
		lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 2));
	}

	lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 1));
	return true;
}

void TextParserOZN::readTablePartData()
{
	std::wstring totalPageStr = getPreviouslySubString(L"Prep./Разраб.");
	lastDrawingPagePtr->trySetPages(getPreviouslySubString(), totalPageStr);

	lastDrawingPagePtr->trySetSchemeNumber(getValueFromSubStrWith(L"Номер схемы"));
	lastDrawingPagePtr->trySetLineNumber(getValueFromSubStrWith(L"Номер линии"));
	lastDrawingPagePtr->trySetIsometricDrawing(getValueFromSubStrWith(L"Изометрический чертеж"));
	lastDrawingPagePtr->trySetCipherDocument(getNextSubString());

	lastDrawingPagePtr->trySetDesignPressure(getSubString(lastDrawingPagePtr->getDesignPressurePattern()));
	lastDrawingPagePtr->trySetDesignTemperature(getNextSubString());
	lastDrawingPagePtr->trySetOperatingPressure(getNextSubString());
	lastDrawingPagePtr->trySetOperatingTemperature(getNextSubString());
	lastDrawingPagePtr->trySetTestPressure(getNextSubString());
	lastDrawingPagePtr->trySetTestEnvironment(getNextSubString());

	std::wstring weldInspectionStr = getNextSubString();
	if (!lastDrawingPagePtr->trySetWeldInspection(weldInspectionStr, ValuesCheker::Type::NONE)) {
		weldInspectionStr += getNextSubString();
		lastDrawingPagePtr->trySetWeldInspection(weldInspectionStr);
	}

	lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getNextSubString());
	lastDrawingPagePtr->trySetStressCalculation(getNextSubString());
	std::wstring paintingSystemStr = getNextSubString();
	if (!lastDrawingPagePtr->trySetPaintingSystem(paintingSystemStr, ValuesCheker::Type::NONE)) {
		lastDrawingPagePtr->trySetTracing(paintingSystemStr);
	}
	else {
		lastDrawingPagePtr->trySetTracing(getNextSubString());
	}
	lastDrawingPagePtr->trySetIsolation(getNextSubString());
	lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getNextSubString());
	lastDrawingPagePtr->trySetGOSTPipelineCategory(getNextSubString());
	lastDrawingPagePtr->trySetDiameterPipeline(getNextSubString());
	lastDrawingPagePtr->trySetPipelineClass(getNextSubString());
}

std::wstring TextParserOZN::getValueFromSubStrWith(const std::wstring& subStr)
{
	std::wstring valueStr = getSubString(subStr);
	return valueStr.substr(valueStr.find_last_of(L' ') + 1);
}

TextParserOZN::TextParserOZN(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

void TextParserOZN::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	if (text->find(L"КОЛ-ВО", currentPositionInText) != std::wstring::npos) {
		moveToNextSubString(L"КОЛ-ВО");
	}
	else if (getFirstSubString().starts_with(L"**********")) {
		moveOnCountSubStr(2);

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
