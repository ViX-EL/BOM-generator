#include "TextParserNAG.h"
#include "DrawingPageNAG.h"
#include "BuildComponentNAG.h"
#include "DrawingPage.h"

#include <regex>
#include <initializer_list>
#include <string>
#include <vector>
#include <wx/log.h>

bool TextParserNAG::readComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	//���� �� ������ ����� ����������
	std::wregex lineNumberPattern(LR"(\d{5}-\w{2}-\d{4}\/\d{3}-\w{2,4}-\d{4}-[0-9A-Z]{9}-\d{2})");
	bool isLineNumber = std::regex_match(componentNumberStr, lineNumberPattern);
	if (!std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern()) || isLineNumber)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") ||
			componentNumberStr.starts_with(L"UT PIPE LENGTH")) {
			if (lastDrawingPagePtr != nullptr)
			{
				componentsEnded = true;
				std::wstring formatStr = getPreviouslySubString(L"LEGEND");
				if (formatStr.starts_with(L"������")) {
					moveOnCountSubStr(2, true);
				}
				else {
					moveToPreviouslySubString();
				}

				std::wstring pagesCountStr(getPreviouslySubString());
				if (std::regex_match(pagesCountStr, lastDrawingPagePtr->getPagesPattern()))
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
			moveToSubString(L"���-��");
		}
		else if (isLineNumber) {
			lastDrawingPagePtr->trySetLineNumber(componentNumberStr);
			std::wstring pageStr(getNextSubString());
			if (std::regex_match(pageStr, lastDrawingPagePtr->getPagesPattern())) {
				lastDrawingPagePtr->trySetPages(pageStr, getNextSubString());
			}
			componentsEnded = true;
		}
		return false;
	}
	else
	{
		if (lastDrawingPagePtr == nullptr)
		{
			createDrawing<DrawingPageNAG>();
		}
		tryAddComponent<BuildComponentNAG>(componentNumberStr);
		return true;
	}
}

void TextParserNAG::writeValueOfTwoSubStr(bool(DrawingPage::* trySetFunction)(const std::wstring&, bool), std::initializer_list<std::wstring> ifEqualStrList, const std::wstring& firstStr)
{
	std::wstring subStr;
	if (firstStr == L"") {
		subStr = getNextSubString();
	}
	else {
		subStr = firstStr;
	}
	for (std::wstring currentTemplateStr : ifEqualStrList)
	{
		if (subStr == currentTemplateStr) {
			subStr = subStr + getNextSubString();
			break;
		}
	}
	(*lastDrawingPagePtr.*trySetFunction)(subStr, true);
}

void TextParserNAG::reset()
{
	currentListEmpty = false;
	BaseTextParser::reset();
}

void TextParserNAG::readTablePartData()
{
	std::wstring schemeNumber(getNextSubString(L"������"));
	bool cipherWasFoundEarlier = false;
	if (std::regex_match(schemeNumber, lastDrawingPagePtr->getCipherDocumentPattern()))
	{
		cipherWasFoundEarlier = true;
		lastDrawingPagePtr->trySetCipherDocument(schemeNumber);
		moveToNextSubString();
		lastDrawingPagePtr->trySetIsometricDrawing(getNextSubString());
		std::wstring totalPagesStr = getPreviouslySubString(L"������", true);
		std::wstring currentPageStr = getPreviouslySubString();
		lastDrawingPagePtr->trySetPages(currentPageStr, totalPagesStr);
		schemeNumber = getNextSubString(L"����� ������������", true);
	}

	bool isFlareFarming = false;
	if (schemeNumber.starts_with(L"GCC-NAG-DDD")) {
		lastDrawingPagePtr->trySetSchemeNumber(schemeNumber);
		writeValueOfTwoSubStr(&DrawingPage::trySetStressCalculation, { L"HYDROSTATIC/" });
	}
	else
	{
		std::wregex flareFarmingPattern(L"��������� ���������");
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
		writeValueOfTwoSubStr(&DrawingPage::trySetOperatingTemperature, { L"�� ���� 5 /" });
		lastDrawingPagePtr->trySetTestPressure(getNextSubString());
		writeValueOfTwoSubStr(&DrawingPage::trySetTestEnvironment, { L"PLANT AIR /", L"FW /" });
		writeValueOfTwoSubStr(&DrawingPage::trySetWeldInspection, { L"2 ������ �����" });
		lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getNextSubString());
		lastDrawingPagePtr->trySetStressCalculation(getNextSubString());
		lastDrawingPagePtr->trySetPaintingSystem(getNextSubString());
		lastDrawingPagePtr->trySetTracing(getNextSubString());
		std::wstring isolation(getNextSubString());
		if (!std::regex_match(isolation, lastDrawingPagePtr->getIsolationPattern()))
		{
			lastDrawingPagePtr->trySetTechnologicalEnvironment(isolation);
		}
		else {
			std::wstring diameterSubStr;
			size_t subStrToDiametrCount = 0;
			while (!diameterSubStr.starts_with(L"DN"))
			{
				diameterSubStr = getNextSubString();
				subStrToDiametrCount++;
				if (diameterSubStr == "XXX") {
					break;
				}

			}
			moveOnCountSubStr(subStrToDiametrCount, true);
			if (subStrToDiametrCount != 3)
			{
				lastDrawingPagePtr->trySetIsolation(isolation);
				lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
			}
			else
			{
				lastDrawingPagePtr->trySetTechnologicalEnvironment(isolation);
			}
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

bool TextParserNAG::readComponent()
{
	if (!readComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 3; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	bool cases[]{ false, false };


	int descriptionSubStrCount{ 1 };
	while (!cases[0] && !cases[1])
	{
		cases[0] = std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getNominalDiameterPattern());
		cases[0] = cases[0] && std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());

		if (!cases[0])
		{
			cases[1] = std::regex_match(*(subStrBuffer.end() - 3), lastComponentPtr->getNominalDiameterPattern());
			cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getPositionCodePattern());
			cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());
		}

		if (!cases[0] && !cases[1]) {
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	if (!(subStrBuffer.end() - 1)->starts_with(L' ') && !(subStrBuffer.end() - 2)->starts_with(L' ') && !cases[1]) {
		size_t currentPos = currentPositionInText;
		std::vector<std::wstring> buffer;
		buffer.emplace_back(getNextSubString(currentPos));
		buffer.emplace_back(getNextSubString(currentPos));
		if ((buffer.end() - 1)->starts_with(L"CUT PIPE"))
		{
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	if (cases[0] && (subStrBuffer.end() - 2)->ends_with(L"x")) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	if (cases[1] && (subStrBuffer.end() - 3)->ends_with(L"x")) {
		descriptionSubStrCount--;
	}

	if (cases[1]) {
		descriptionSubStrCount--;
	}

	std::wstring descriptionStr;
	if (!(cases[1] && subStrBuffer.size() == 3)) {
		for (int i = 0; i < descriptionSubStrCount; i++)
		{
			descriptionStr += subStrBuffer[i];
		}
		lastComponentPtr->trySetDescription(descriptionStr);
	}

	if (cases[0])
	{
		if ((subStrBuffer.end() - 3)->ends_with(L"x")) {
			lastComponentPtr->trySetNominalDiameter((*(subStrBuffer.end() - 3) + L' ' + (*(subStrBuffer.end() - 2))));
		}
		else
		{
				lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 2));
		}
		lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 1));
	}
	else if (cases[1])
	{
		if (subStrBuffer.size() != 3 && (subStrBuffer.end() - 4)->ends_with(L"x")) {
			lastComponentPtr->trySetNominalDiameter((*(subStrBuffer.end() - 4) + L' ' + (*(subStrBuffer.end() - 3))));
		}
		else {
			lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 3));
		}
		lastComponentPtr->trySetPositionCode(*(subStrBuffer.end() - 2));
		lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 1));
	}
	return true;
}

void TextParserNAG::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	if (text->find(L"FABRICATION MATERIALS", currentPositionInText) != std::wstring::npos) {
		moveToSubString(L"���-��");
	}
	while (true) //������ ���� �����������
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
	}
	else {
		currentListEmpty = false;
		wxLogMessage("[������] ����������� ������������ ����� � ����� %s", fileName);
	}
}
