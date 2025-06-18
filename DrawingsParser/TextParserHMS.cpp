#include "TextParserHMS.h"
#include "BaseTextParser.h"
#include "DrawingPageHMS.h"
#include "BuildComponentHMS.h"

#include <regex>

bool TextParserHMS::readComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	bool isComponentNumber = std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern());
	if (!isComponentNumber)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") || componentNumberStr.starts_with(L"**********") || isLineNumber(componentNumberStr)) {
			componentsEnded = true;
		}
		if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			moveToSubString(L"КОЛ-ВО");
		}
		return false;
	}
	else
	{
		createDrawing<DrawingPageHMS>();
		tryAddComponent<BuildComponentHMS>(componentNumberStr);
		return true;
	}
}

bool TextParserHMS::readComponent()
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
			ValuesCheker::CheckCondition(!((subStrBuffer.end() - 1)->starts_with(L"CUT PIPE LENGTH") || (subStrBuffer.end() - 1)->starts_with(L"**********")
				|| isLineNumber(*(subStrBuffer.end() - 1))), "Невозможно определить компонент!");
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

void TextParserHMS::readTablePartData()
{
	moveToPreviouslySubString(L"Формат");
	moveToPreviouslySubString();
	std::wstring totalPageStr = getPreviouslySubString();
	lastDrawingPagePtr->trySetPages(getPreviouslySubString(), totalPageStr);
	lastDrawingPagePtr->trySetLineNumber(getPreviouslySubString());

	lastDrawingPagePtr->trySetSchemeNumber(getNextSubString(L"ЛИСТОВ"));
	lastDrawingPagePtr->trySetDesignPressure(getNextSubString());
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

	std::wstring technologicalEnvironmentStr = getNextSubString();
	if(!lastDrawingPagePtr->trySetTechnologicalEnvironment(technologicalEnvironmentStr, ValuesCheker::Type::NONE)){
		technologicalEnvironmentStr += getNextSubString();
	}
	lastDrawingPagePtr->trySetTechnologicalEnvironment(technologicalEnvironmentStr);

	std::wstring categoryPipelinesTRCUStr = getNextSubString();
	std::wstring GOSTPipelineCategoryStr;
	if (!lastDrawingPagePtr->trySetCategoryPipelinesTRCU(categoryPipelinesTRCUStr, ValuesCheker::Type::NONE)) {
		GOSTPipelineCategoryStr = categoryPipelinesTRCUStr;
	}
	else {
		GOSTPipelineCategoryStr = getNextSubString();
	}
	if(!lastDrawingPagePtr->trySetGOSTPipelineCategory(GOSTPipelineCategoryStr, ValuesCheker::Type::NONE)){
		lastDrawingPagePtr->trySetDiameterPipeline(GOSTPipelineCategoryStr);
	}
	else {
		lastDrawingPagePtr->trySetDiameterPipeline(getNextSubString());
	}
	lastDrawingPagePtr->trySetPipelineClass(getNextSubString());
	lastDrawingPagePtr->trySetCipherDocument(getNextSubString());

	lastDrawingPagePtr->trySetIsometricDrawing(getNextSubString(L"Изометрический чертеж"));
}

bool TextParserHMS::isLineNumber(const std::wstring& sourceStr) const
{
	if (lastDrawingPagePtr != nullptr) {
		return std::regex_match(sourceStr, lastDrawingPagePtr->getLineNumberPattern());
	}
	return false;
}

TextParserHMS::TextParserHMS(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

void TextParserHMS::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	if (text->find(L"КОЛ-ВО", currentPositionInText) != std::wstring::npos) {
		moveToSubString(L"КОЛ-ВО");
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
