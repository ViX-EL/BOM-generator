#include "TextParserIOT.h"
#include "BaseTextParser.h"
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
	bool match{ false };

	int descriptionSubStrCount{ 1 };
	while (!match)
	{
		match = std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getNominalDiameterPattern());
		match = match && std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getPositionCodePattern());
		match = match && std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());

		if (!match) {
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	std::wstring descriptionStr;
	for (int i = 0; i < descriptionSubStrCount; i++)
	{
		descriptionStr += subStrBuffer[i];
	}
	lastComponentPtr->trySetDescription(descriptionStr);

	lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 3));
	lastComponentPtr->trySetPositionCode(*(subStrBuffer.end() - 2));
	lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 1));
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
		return false;
	}
	else
	{
		createDrawing<DrawingPageIOT>();

		tryAddComponent<BuildComponentIOT>(componentNumberStr);
		return true;
	}
}

void TextParserIOT::readTablePartData()
{
	lastDrawingPagePtr->trySetPipelineClass(getPreviouslySubString(L"TOTAL FOR"));
	lastDrawingPagePtr->trySetDiameterPipeline(getPreviouslySubString());
	lastDrawingPagePtr->trySetIsolation(getPreviouslySubString());
	lastDrawingPagePtr->trySetTracing(getPreviouslySubString());
	lastDrawingPagePtr->trySetOperatingTemperature(getPreviouslySubString());
	lastDrawingPagePtr->trySetDesignTemperature(getPreviouslySubString());
	lastDrawingPagePtr->trySetOperatingPressure(getPreviouslySubString());
	lastDrawingPagePtr->trySetTestPressure(getPreviouslySubString());
	lastDrawingPagePtr->trySetDesignPressure(getPreviouslySubString());
	moveToPreviouslySubString();
	lastDrawingPagePtr->trySetSchemeNumber(getPreviouslySubString());
	lastDrawingPagePtr->trySetIsometricDrawing(getSubString(L"Isometric drawing", true).erase(0, 18));
	moveToPreviouslySubString();
	lastDrawingPagePtr->trySetFileName(getPreviouslySubString());
	moveOnCountSubStr(8, true);
	lastDrawingPagePtr->trySetLineNumber(getPreviouslySubString());
	moveOnCountSubStr(3, true);
	std::wstring totalPagesStr = getPreviouslySubString();
	lastDrawingPagePtr->trySetPages(getPreviouslySubString(), totalPagesStr);
	moveToPreviouslySubString();
	lastDrawingPagePtr->trySetCipherDocument(getPreviouslySubString());
	lastDrawingPagePtr->trySetPaintingSystem(getNextSubString(L"ТЕХНОЛОГИИ"));
	lastDrawingPagePtr->trySetTestEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetWeldInspection(getNextSubString());
	lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getNextSubString());
	lastDrawingPagePtr->trySetStressCalculation(getNextSubString());
	lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getNextSubString());
	lastDrawingPagePtr->trySetGOSTPipelineCategory(getNextSubString());
}

void TextParserIOT::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	moveToNextSubString(L"КОЛ-ВО");

	while (true) //Чтение всех компонентов
	{
		if (!readComponent())
		{
			if (!readComponent()) {
				break;
			}
		}
	}

	readTablePartData();
}