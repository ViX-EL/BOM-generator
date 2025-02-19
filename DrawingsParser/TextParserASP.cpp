#include "TextParserASP.h"
#include "DrawingPageASP.h"
#include "BuildComponentASP.h"
#include "StringUtilities.h"

#include <string>
#include <vector>
#include <regex>
#include <cassert>
#include <stack>
#include <memory>

bool TextParserASP::readComponentNumber()
{
	if (nextComponentNumberMissing) {
		nextComponentNumberMissing = false;
		return true;
	}
	if (componentsEnded) {
		return false;
	}

	size_t currentTextPos = currentPositionInText;
	if (!tryMoveToNextSubString(currentTextPos))
	{
		componentsEnded = true;
		return false;
	}

	currentTextPos = currentPositionInText;
	if (withoutWeldedPipe || cipherDocumentStr.contains(L"-PT-") && lastDrawingPagePtr != nullptr)
	{
		std::wstring currentPageStr = getNextSubString(currentTextPos);
		std::wstring totalPagesStr = getNextSubString(currentTextPos);
		if (lastDrawingPagePtr->trySetPages(currentPageStr, totalPagesStr, false))
		{
			std::wstring schemeNumberStr = getNextSubString(currentTextPos);
			if (std::regex_match(schemeNumberStr, lastDrawingPagePtr->getSchemeNumberPattern()))
			{
				lastDrawingPagePtr->trySetSchemeNumber(schemeNumberStr);
			}
			componentsEnded = true;
			return false;
		}
	}

	std::wstring componentNumberStr(getNextSubString());

	if (readFirstPipeInEndText(componentNumberStr)) {
		return true;
	}

	currentTextPos = currentPositionInText;
	if (!std::regex_search(getNextSubString(currentTextPos), StringUtilities::getRegex(LR"([A-Za-z0-9  -.]+\/[А-Яа-я0-9  -.]+)")))
	{
		std::wstring schemeNumberStr = getNextSubString(currentTextPos);
		std::wregex schemeNumberPattern(L"GCC-ASP-DDD");
		if (std::regex_search(schemeNumberStr, schemeNumberPattern) || schemeNumberStr.starts_with(L"          "))
		{
			componentsEnded = true;
			return false;
		}
	}

	bool cutPipeLengthFinded = false;
	if (!componentNumberStr.starts_with(L"CUT PIPE LENGTH")) {
		cutPipeLengthFinded = searchForMatchesInFollowing(std::wregex(L"CUT PIPE LENGTH"), 3);
	}
	else {
		cutPipeLengthFinded = true;
	}
	if (cutPipeLengthFinded || !std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern()))
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") || (componentNumberStr.starts_with(L"*********") && (lastDrawingPagePtr != nullptr)))
		{
			if (existComponentsInEndText) {
				readComponensFromEndText();
			}
			componentsEnded = true;
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) 
		{
			std::wstring subStr = getNextSubString(L"КОЛ-ВО");
			if (subStr.contains(L"*********")) {
				componentsEnded = true;
			}
		}
		else if (cutPipeLengthFinded) 
		{
			if (getSubString(L"ERECTION MATERIALS") != L"") {
				moveToNextSubString(L"КОЛ-ВО");
			}
		}
		return false;
	}
	else 
	{
		createDrawing<DrawingPageASP>();

		tryAddComponent<BuildComponentASP>(componentNumberStr);

		int componentNumber = stoi(componentNumberStr);
		if (componentNumber == 1)
		{
			currentTextPos = currentPositionInText;
			std::wstring pipeStr = getPreviouslySubString(currentTextPos);
			if (!pipeStr.contains(L"PIPE")) {
				withoutWeldedPipe = true;
			}
		}

		if (!pageStartWithContFrom && componentNumber != lastDrawingPagePtr->getComponentsCount()) {
			existComponentsInEndText = true;
		}
		return true;
	}
}

bool TextParserASP::readComponent()
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
		cases[0] = std::regex_match(*(subStrBuffer.begin()), lastComponentPtr->getNominalDiameterPattern());
		cases[0] = cases[0] && std::regex_match(*(subStrBuffer.begin() + 1), lastComponentPtr->getAmountPattern());
		if (cases[0]) {
			descriptionSubStrCount = 0;
		}
		else {
			cases[0] = std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getNominalDiameterPattern());
			cases[0] = cases[0] && std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());
		}

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

	if (!cases[1] && std::regex_match(*(subStrBuffer.begin()), BuildComponent::getPositionNumberPattern()) && !std::regex_match(*(subStrBuffer.begin() + 1), 
		lastComponentPtr->getAmountPattern()))
	{
		nextComponentNumberMissing = true;
		descriptionSubStrCount--;
		subStrBuffer.erase(subStrBuffer.begin());
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
		if (descriptionSubStrCount != 0) {
			lastComponentPtr->trySetDescription(descriptionStr);
		}
	}

	if (cases[0])
	{
		if ((subStrBuffer.end() - 3)->ends_with(L"x")) {
			lastComponentPtr->trySetNominalDiameter((*(subStrBuffer.end() - 3) + L' ' + (*(subStrBuffer.end() - 2))));
		}
		else
		{
			if (descriptionSubStrCount != 0) {
				lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 2));
			}
			else {
				lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.begin()));
			}
		}
		if (descriptionSubStrCount != 0) {
			lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 1));
		}
		else
		{
			lastComponentPtr->trySetAmount(*(subStrBuffer.begin() + 1));
			moveToPreviouslySubString();
		}

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

void TextParserASP::readComponensFromEndText()
{
	std::wstring amountStr = getLastSubString();
	if (!std::regex_match(amountStr, lastComponentPtr->getAmountPattern())) {
		amountStr = getSubString(lastComponentPtr->getAmountPattern(), true);
	}
	std::wstring nominalDiameterStr(getPreviouslySubString());
	std::wstring positionCodeStr;
	if (std::regex_match(nominalDiameterStr, lastComponentPtr->getPositionCodePattern())) {
		positionCodeStr = nominalDiameterStr;
		nominalDiameterStr = getPreviouslySubString();
	}
	else if (!std::regex_match(nominalDiameterStr, lastComponentPtr->getNominalDiameterPattern())) {
		return;
	}

	std::wstring currentSubStr = getPreviouslySubString();
	bool anotherComponentExist = false;
	do 
	{
		std::stack<std::wstring> descriptionStack;
		while (!std::regex_match(currentSubStr, BuildComponent::getPositionNumberPattern()))
		{
			descriptionStack.push(currentSubStr);
			currentSubStr = getPreviouslySubString();
		}

		std::wstring descriptionStr;
		while (!descriptionStack.empty())
		{
			descriptionStr += descriptionStack.top();
			descriptionStack.pop();
		}

		if (descriptionStr == L"")
		{
			moveToSubString(lastComponentPtr->getDescriptionPattern(), true);
			std::wstring positionNumberStr = getSubString(lastComponentPtr->getPositionNumberPattern(), true);
			do {
				currentSubStr = getNextSubString();
				descriptionStr += currentSubStr;
			} while (!std::regex_match(currentSubStr, lastComponentPtr->getNominalDiameterPattern()));
			nominalDiameterStr = currentSubStr;
			currentSubStr = getNextSubString();
			if (std::regex_match(currentSubStr, lastComponentPtr->getPositionCodePattern())) {

				positionCodeStr = currentSubStr;
				amountStr = getNextSubString();
			}
			else {
				amountStr = currentSubStr;
			}
			currentSubStr = positionNumberStr;
		}

		tryAddComponent<BuildComponentASP>(currentSubStr);
		if (positionCodeStr != L"") {
			lastComponentPtr->trySetPositionCode(positionCodeStr);
		}
		lastComponentPtr->trySetDescription(descriptionStr);
		lastComponentPtr->trySetAmount(amountStr);
		lastComponentPtr->trySetNominalDiameter(nominalDiameterStr);

		amountStr = getPreviouslySubString();
		nominalDiameterStr = getPreviouslySubString();
		positionCodeStr = L"";
		if (std::regex_match(nominalDiameterStr, lastComponentPtr->getPositionCodePattern())) {
			positionCodeStr = nominalDiameterStr;
			nominalDiameterStr = getPreviouslySubString();
		}
		currentSubStr = getPreviouslySubString();

		anotherComponentExist = std::regex_match(amountStr, lastComponentPtr->getAmountPattern());
		if (positionCodeStr != L"") {
			anotherComponentExist = anotherComponentExist && std::regex_match(positionCodeStr, lastComponentPtr->getPositionCodePattern());
		}
		anotherComponentExist = anotherComponentExist && std::regex_match(nominalDiameterStr, lastComponentPtr->getNominalDiameterPattern());
		anotherComponentExist = anotherComponentExist && std::regex_match(currentSubStr, lastComponentPtr->getDescriptionPattern());
	} while (anotherComponentExist);
}

bool TextParserASP::readFirstPipeInEndText(std::wstring& currentSubStr)
{
	size_t currentTextPos = currentPositionInText;
	std::wstring prevSubStr(getPreviouslySubString(currentTextPos));
	if (currentSubStr == L"FITTINGS/ ФИТИНГИ" && prevSubStr == L"PIPE/ ТРУБА")
	{
		if (lastDrawingPagePtr == nullptr) {
			createDrawing<DrawingPageASP>();
		}
		tryAddComponent<BuildComponentASP>(L"1");
		moveToLastSubString();
		std::wstring positionCodeStr;
		while (positionCodeStr.size() < 15) {
			positionCodeStr = getPreviouslySubString();
		}
		lastComponentPtr->trySetPositionCode(getNextSubString());
		moveToPreviouslySubString();
		std::wstring descriptionStr = getPreviouslySubString();
		descriptionStr = descriptionStr + L" " + getNextSubString();
		lastComponentPtr->trySetDescription(descriptionStr);
		std::wstring countStr;
		while (!std::regex_match(countStr, lastComponentPtr->getAmountPattern()))
		{
			countStr = getPreviouslySubString();
		}
		lastComponentPtr->trySetAmount(countStr);
		lastComponentPtr->trySetNominalDiameter(getPreviouslySubString());
		moveToSubString(L"FITTINGS/ ФИТИНГИ", true);
		return true;
	}
	return false;
}

void TextParserASP::readTablePartData()
{
	enum class Case {
		Case1, Case2, Case3
	};
	Case parsCase;
	std::wstring designTemperatureStr = getNextSubString(L"РАСЧЕТ. ТЕМП.");
	if (designTemperatureStr == L"") {
		designTemperatureStr = getNextSubString(L"РАСЧЕТ. ТЕМП.", true);
	}
	if (withoutWeldedPipe) {
		parsCase = Case::Case3;
	}
	else if (designTemperatureStr != L"SHEET" && designTemperatureStr != L"OPER. PRESSURE (MPa)") {
		parsCase = Case::Case1;
	}
	else {
		parsCase = Case::Case2;
	}

	switch (parsCase)
	{
	case Case::Case1:
		parseCase1(designTemperatureStr);
		break;
	case Case::Case2:
		parseCase2();
		break;
	case Case::Case3:
		parseCase3();
		break;
	default:
		break;
	}
}

void TextParserASP::trySet(const std::wstring& nextSubStr, const std::wstring& unwantedSubStr, bool(DrawingPage::* trySetFunction)(const std::wstring&, bool))
{
	std::wstring subStr(getPreviouslySubString(nextSubStr, true));
	if (!subStr.starts_with(unwantedSubStr)) {
		(*lastDrawingPagePtr.*trySetFunction)(subStr, true);
	}
}

void TextParserASP::parseCase1(std::wstring designTemperatureStr)
{
	lastDrawingPagePtr->trySetDesignTemperature(designTemperatureStr);
	lastDrawingPagePtr->trySetDesignPressure(getNextSubString(L"РАСЧЕТ. ДАВЛЕНИЕ", true));
	lastDrawingPagePtr->trySetOperatingTemperature(getNextSubString(L"РАБОЧАЯ ТЕМП.", true));
	lastDrawingPagePtr->trySetOperatingPressure(getNextSubString(L"РАБОЧЕЕ ДАВЛЕНИЕ", true));

	std::wstring isometricDrawingStr = getSubString(L"Изометрический чертеж", true);
	if (isometricDrawingStr == L"") {
		isometricDrawingStr = getSubString(L"Изометрический чертеж");
	}
	if (isometricDrawingStr != L"") {
		isometricDrawingStr = isometricDrawingStr.erase(0, 22);
	}
	if (isometricDrawingStr != L"") {
		lastDrawingPagePtr->trySetIsometricDrawing(isometricDrawingStr);
	}

	std::wstring lineNumberStr(getNextSubString(L"Line number"));
	if (lineNumberStr == L"") {
		lineNumberStr = getNextSubString(L"Line number", true);
	}
	lastDrawingPagePtr->trySetLineNumber(lineNumberStr);

	moveToPreviouslySubString();
	trySet(L"Line number", L"КЛАСС", &DrawingPage::trySetPipelineClass);
	lastDrawingPagePtr->trySetDiameterPipeline(getPreviouslySubString(L"PIPE CLASS", true));
	trySet(L"PIPE DIAM", L"КАТЕГОРИЯ", &DrawingPage::trySetGOSTPipelineCategory);
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getPreviouslySubString(L"PIPE CATEGORY", true));
	trySet(L"PIPE CATEGORY SN", L"ТЕХНОЛОГИЧЕСКАЯ", &DrawingPage::trySetTechnologicalEnvironment);
	lastDrawingPagePtr->trySetIsolation(getPreviouslySubString(L"FLUID SERVICE", true));
	trySet(L"INSULATION", L"СПУТНИКОВЫЙ", &DrawingPage::trySetTracing);
	trySet(L"TRACING", L"СИСТЕМА", &DrawingPage::trySetPaintingSystem);
	lastDrawingPagePtr->trySetStressCalculation(getPreviouslySubString(L"PAINTING SYSTEM", true));
	trySet(L"CRITICAL LINE", L"ПОСЛЕСВАР", &DrawingPage::trySetPostWeldingHeatTreatment);
	trySet(L"POST WELD", L"КОНТРОЛЬ", &DrawingPage::trySetWeldInspection);
	trySet(L"WELD INSPECTION", L"СРЕДА", &DrawingPage::trySetTestEnvironment);
	trySet(L"TEST MEDIUM", L"ДАВЛЕНИЕ", &DrawingPage::trySetTestPressure);

	std::wstring schemeNumber = getPreviouslySubString(L"For general", true);
	if (schemeNumber.starts_with(L"GCC-ASP-DDD-")) {
		lastDrawingPagePtr->trySetSchemeNumber(schemeNumber);
	}

	std::wstring PageTotalStr = getPreviouslySubString(L"LEGEND", true);
	std::wstring currentPageStr = getPreviouslySubString();
	if (!lastDrawingPagePtr->trySetPages(currentPageStr, PageTotalStr, false))
	{
		if (std::regex_search(PageTotalStr, lastDrawingPagePtr->getSchemeNumberPattern()))
		{
			lastDrawingPagePtr->trySetSchemeNumber(PageTotalStr);
			PageTotalStr = currentPageStr;
			currentPageStr = getPreviouslySubString();
			if (!lastDrawingPagePtr->trySetPages(currentPageStr, PageTotalStr, false)) {
				lastDrawingPagePtr->trySetPages(L"1", L"1");
			}
		}
		else {
			lastDrawingPagePtr->trySetPages(L"1", L"1");
		}
	}

	if (cipherDocumentStr.contains(L"-PT-"))
	{
		std::wstring generalNotesStr = getSubString(StringUtilities::getRegex(LR"(GCC-ASP-DDD-\d+-\d+-\d+-PT-LST-\d+)"));
		std::wstring schemeNumber = getNextSubString();

		if (lastDrawingPagePtr->getSchemeNumber() == L"-") {
			lastDrawingPagePtr->trySetSchemeNumber(schemeNumber);
		}

		std::wregex numberPattern(LR"(\d+)");
		moveToPreviouslySubString();
		if (lastDrawingPagePtr->getPaintingSystem() == L"-")
		{
			std::wstring paintingSystemStr = getPreviouslySubString();
			if (!std::regex_match(paintingSystemStr, numberPattern)) 
			{
				if (std::regex_match(paintingSystemStr, lastDrawingPagePtr->getPaintingSystemPattern())) {
					lastDrawingPagePtr->trySetPaintingSystem(paintingSystemStr);
				}
				else {
					lastDrawingPagePtr->trySetTechnologicalEnvironment(paintingSystemStr);
					lastDrawingPagePtr->trySetPaintingSystem(getPreviouslySubString());
				}
			}
			else {
				moveOnCountSubStr(2);
				lastDrawingPagePtr->trySetPaintingSystem(getNextSubString());
			}
		}

		std::wstring testEnvironmentStr = getSubString(lastDrawingPagePtr->getTestEnvironmentPattern());
		if (testEnvironmentStr != L"") 
		{
			if (lastDrawingPagePtr->getTestEnvironment() == L"-") {
				lastDrawingPagePtr->trySetTestEnvironment(testEnvironmentStr);
			}
			if (lastDrawingPagePtr->getWeldInspection() == L"-") {
				lastDrawingPagePtr->trySetWeldInspection(getNextSubString());
			}
		}
		std::wregex datePattern(LR"(\d+-\d+-\d+)");
		if (lastDrawingPagePtr->getTechnologicalEnvironment() == L"-")
		{
			std::wstring technologicalEnvironmentStr = getNextSubString();
			if (!std::regex_match(technologicalEnvironmentStr, datePattern) && !std::regex_match(technologicalEnvironmentStr, numberPattern)) {
				lastDrawingPagePtr->trySetTechnologicalEnvironment(technologicalEnvironmentStr);
			}
			else {
				moveOnCountSubStr(2, true);
				lastDrawingPagePtr->trySetTechnologicalEnvironment(getPreviouslySubString());
				moveOnCountSubStr(2);
			}
		}
		if (lastDrawingPagePtr->getTracing() == L"-")
		{
			std::wstring tracingStr;
			do {
				tracingStr = getNextSubString();
			} while (std::regex_match(tracingStr, datePattern) || std::regex_match(tracingStr, numberPattern) || tracingStr.starts_with(L"Issued for"));
			lastDrawingPagePtr->trySetTracing(tracingStr);
		}
		readDescriptionsFromEndText();
	}
}

void TextParserASP::parseCase2()
{
	std::wstring designPressure = getPreviouslySubString(L"Pipe surface", true);
	if (designPressure == L"") {
		designPressure = getPreviouslySubString(L"Rev.", true);
	}
	std::wstring testPressureStr;
	if (std::regex_match(designPressure, lastDrawingPagePtr->getDesignPressurePattern()))
	{
		lastDrawingPagePtr->trySetDesignPressure(designPressure);
		lastDrawingPagePtr->trySetDesignTemperature(getPreviouslySubString());
		lastDrawingPagePtr->trySetOperatingPressure(getPreviouslySubString());
		lastDrawingPagePtr->trySetOperatingTemperature(getPreviouslySubString());
		lastDrawingPagePtr->trySetSchemeNumber(getPreviouslySubString());
		lastDrawingPagePtr->trySetLineNumber(getPreviouslySubString());
		testPressureStr = getPreviouslySubString();
		lastDrawingPagePtr->trySetTestPressure(testPressureStr, false);
	}

	if (std::regex_match(testPressureStr, lastDrawingPagePtr->getTestPressurePattern())) {
		lastDrawingPagePtr->trySetTestEnvironment(getPreviouslySubString());
	} 
	else {
		lastDrawingPagePtr->trySetTestEnvironment(testPressureStr);
	}
	lastDrawingPagePtr->trySetWeldInspection(getPreviouslySubString());
	lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getPreviouslySubString());
	moveOnCountSubStr(3, true);

	lastDrawingPagePtr->trySetStressCalculation(getPreviouslySubString());
	std::wstring paintingSystemStr{ getPreviouslySubString() };
	if (!lastDrawingPagePtr->trySetPaintingSystem(paintingSystemStr, false)) {
		lastDrawingPagePtr->trySetTracing(paintingSystemStr);
	}
	else {
		lastDrawingPagePtr->trySetTracing(getPreviouslySubString());
	}
	lastDrawingPagePtr->trySetIsolation(getPreviouslySubString());

	moveToPreviouslySubString();
	lastDrawingPagePtr->trySetTechnologicalEnvironment(getPreviouslySubString());
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getPreviouslySubString());
	lastDrawingPagePtr->trySetGOSTPipelineCategory(getPreviouslySubString());
	lastDrawingPagePtr->trySetDiameterPipeline(getPreviouslySubString());
	lastDrawingPagePtr->trySetPipelineClass(getPreviouslySubString());

	lastDrawingPagePtr->trySetIsometricDrawing(getSubString(L"Изометрический чертеж", true).erase(0, 22));
	moveOnCountSubStr(2, true);
	std::wstring totalPagesStr = getPreviouslySubString();
	std::wstring currentPageStr = getPreviouslySubString();
	lastDrawingPagePtr->trySetPages(currentPageStr, totalPagesStr);
}

void TextParserASP::parseCase3()
{
	lastDrawingPagePtr->trySetIsometricDrawing(getSubString(L"Изометрический чертеж", true).erase(0, 22));
	std::wstring testEnvironmentStr(getPreviouslySubString(L"Pipe surface"));
	if (testEnvironmentStr == L"") {
		std::wstring lineNumberStr = getPreviouslySubString(L"Stress sketch");
		if (lineNumberStr == L"") 
		{
			withoutWeldedPipe = false;
			readTablePartData();
			return;
		}
		lastDrawingPagePtr->trySetLineNumber(lineNumberStr);
		lastDrawingPagePtr->trySetTestEnvironment(getPreviouslySubString());
	}
	else {
		if (!std::regex_match(testEnvironmentStr, lastDrawingPagePtr->getLineNumberPattern())) 
		{
			if (!lastDrawingPagePtr->trySetTestEnvironment(testEnvironmentStr, false))
			{
				withoutWeldedPipe = false;
				readTablePartData();
				return;
			}
		}
		else {
			lastDrawingPagePtr->trySetLineNumber(testEnvironmentStr);
			lastDrawingPagePtr->trySetTestEnvironment(getPreviouslySubString());
		}
	}
	lastDrawingPagePtr->trySetWeldInspection(getPreviouslySubString());
	lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getPreviouslySubString());
	moveOnCountSubStr(3, true);
	lastDrawingPagePtr->trySetStressCalculation(getPreviouslySubString());
	lastDrawingPagePtr->trySetPaintingSystem(getPreviouslySubString());
	lastDrawingPagePtr->trySetTracing(getPreviouslySubString());
	lastDrawingPagePtr->trySetIsolation(getPreviouslySubString());
	moveToPreviouslySubString();
	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getPreviouslySubString());
	lastDrawingPagePtr->trySetGOSTPipelineCategory(getPreviouslySubString());
	lastDrawingPagePtr->trySetDiameterPipeline(getPreviouslySubString());
	lastDrawingPagePtr->trySetPipelineClass(getPreviouslySubString());

	std::wstring schemeNumberStr(getSubString(L"Номер схемы"));
	if (schemeNumberStr != L"") 
	{
		size_t charGIndex = schemeNumberStr.find_first_of(L'G');
		if (charGIndex != std::wstring::npos)
		{
			schemeNumberStr = schemeNumberStr.erase(0, charGIndex);
			lastDrawingPagePtr->trySetSchemeNumber(schemeNumberStr);
		}
	}
	if (testEnvironmentStr != L"" && !std::regex_match(testEnvironmentStr, lastDrawingPagePtr->getLineNumberPattern())) {
		lastDrawingPagePtr->trySetLineNumber(getNextSubString().erase(0, 24));
	}
	moveToNextSubString(L"Issued for");
	size_t currPos = currentPositionInText;
	moveOnCountSubStr(currPos, 3, true);
	std::wstring operatingPressureStr(getPreviouslySubString(currPos));
	if (std::regex_match(operatingPressureStr, lastDrawingPagePtr->getOperatingPressurePattern()) || operatingPressureStr == L"-")
	{
		if (operatingPressureStr == L"-")
		{
			lastDrawingPagePtr->trySetTestPressure(operatingPressureStr);
			lastDrawingPagePtr->trySetDesignPressure(getPreviouslySubString(currPos));
			std::wstring designTemperatureStr = getPreviouslySubString(currPos);
			if (designTemperatureStr == L"-") {
				lastDrawingPagePtr->trySetDesignTemperature(designTemperatureStr);
				lastDrawingPagePtr->trySetOperatingPressure(getPreviouslySubString(currPos));
			}
			else {
				lastDrawingPagePtr->trySetOperatingPressure(designTemperatureStr);
			}
			lastDrawingPagePtr->trySetOperatingTemperature(getPreviouslySubString(currPos));
			if (designTemperatureStr != L"-") {
				lastDrawingPagePtr->trySetDesignTemperature(getPreviouslySubString(currPos));
			}
			std::wstring technologicalEnvironmentStr;
			while (tryMoveToNextSubString(currPos) && !std::regex_match(technologicalEnvironmentStr, lastDrawingPagePtr->getTechnologicalEnvironmentPattern()))
			{
				technologicalEnvironmentStr = getNextSubString();
			}
			if (std::regex_match(technologicalEnvironmentStr, lastDrawingPagePtr->getTechnologicalEnvironmentPattern())) {
				lastDrawingPagePtr->trySetTechnologicalEnvironment(technologicalEnvironmentStr);
			}
		}
		else
		{
			lastDrawingPagePtr->trySetOperatingPressure(operatingPressureStr);
			lastDrawingPagePtr->trySetOperatingTemperature(getPreviouslySubString(currPos));
			lastDrawingPagePtr->trySetTestPressure(getPreviouslySubString(currPos));
			lastDrawingPagePtr->trySetDesignPressure(getPreviouslySubString(currPos));
			lastDrawingPagePtr->trySetDesignTemperature(getPreviouslySubString(currPos));
			lastDrawingPagePtr->trySetTechnologicalEnvironment(getLastSubString());
		}

		readDescriptionsFromEndText();
		return;
	}
	moveOnCountSubStr(2);
	currPos = currentPositionInText;
	if (tryMoveOnCountSubStr(currPos, 6, false)) 
	{
		lastDrawingPagePtr->trySetDesignPressure(getNextSubString());
		lastDrawingPagePtr->trySetDesignTemperature(getNextSubString());
		lastDrawingPagePtr->trySetTestPressure(getNextSubString());
		lastDrawingPagePtr->trySetOperatingTemperature(getNextSubString());
		lastDrawingPagePtr->trySetOperatingPressure(getNextSubString());
		lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
	}
	else
	{
		currPos = currentPositionInText;
		if (tryMoveToNextSubString(currPos)) 
		{
			lastDrawingPagePtr->trySetTechnologicalEnvironment(getNextSubString());
			moveOnCountSubStr(6, true);
			lastDrawingPagePtr->trySetDesignPressure(getPreviouslySubString());
			lastDrawingPagePtr->trySetDesignTemperature(getPreviouslySubString());
			lastDrawingPagePtr->trySetOperatingPressure(getPreviouslySubString());
			lastDrawingPagePtr->trySetOperatingTemperature(getPreviouslySubString());
			lastDrawingPagePtr->trySetTestPressure(getPreviouslySubString());
		}
		else
		{
			moveToPreviouslySubString(L"Issued for", true);
			moveToPreviouslySubString();
			lastDrawingPagePtr->trySetTechnologicalEnvironment(getPreviouslySubString());
			moveOnCountSubStr(5, true);
			lastDrawingPagePtr->trySetTestPressure(getPreviouslySubString());
			lastDrawingPagePtr->trySetDesignPressure(getPreviouslySubString());
			lastDrawingPagePtr->trySetDesignTemperature(getPreviouslySubString());
			lastDrawingPagePtr->trySetOperatingPressure(getPreviouslySubString());
			lastDrawingPagePtr->trySetOperatingTemperature(getPreviouslySubString());
		}
	}
}

void TextParserASP::readDescriptionsFromEndText()
{
	std::wstring currentSubStr(getSubString(lastDrawingPagePtr->getTestEnvironmentPattern()));
	if (currentSubStr == L"") {
		currentSubStr = getSubString(lastDrawingPagePtr->getTestEnvironmentPattern(), true);
	}
	size_t currPos = currentPositionInText;
	std::wstring weldInspectionStr = getPreviouslySubString(currPos);
	if (std::regex_match(weldInspectionStr, lastDrawingPagePtr->getWeldInspectionPattern()) || weldInspectionStr == L"-") {
		currentSubStr = getLastSubString();
	}
	for (size_t i = 0; i < lastDrawingPagePtr->getComponentsCount(); i++)
	{
		if (lastDrawingPagePtr->getComponent(i).getDescription() == L"-")
		{
			std::stack<std::wstring> descriptionStack;
			currentSubStr = getPreviouslySubString();
			while (!std::regex_search(currentSubStr, StringUtilities::getRegex(LR"([A-Za-z0-9 -.]+\/[А-Яа-я0-9 -.]+)")))
			{
				descriptionStack.push(currentSubStr);
				currentSubStr = getPreviouslySubString();
			}
			descriptionStack.push(currentSubStr);

			std::wstring descriptionStr;
			while (!descriptionStack.empty())
			{
				descriptionStr += descriptionStack.top();
				descriptionStack.pop();
			}
			lastDrawingPagePtr->getComponent(i).trySetDescription(descriptionStr);
		}
	}
}

TextParserASP::TextParserASP(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

void TextParserASP::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	//if (fileName.contains(L"16150-11-2200_005-IA-0711-GCB2B01BN-02_Sht_1"))
	//{
	//	int x = 2;
	//	x = x * 2;
	//}

	cipherDocumentStr = getSubString(StringUtilities::getRegex(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-ISO-\d+)"));

	if (getFirstSubString().starts_with(L"*********")) 
	{
		pageStartWithContFrom = true;
		moveOnCountSubStr(2);
	}
	else {
		moveToNextSubString(L"КОЛ-ВО");
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
		lastDrawingPagePtr->trySetCipherDocument(cipherDocumentStr);
		readTablePartData();

		lastDrawingPagePtr->trySetFileName(fileName);

		lastDrawingPagePtr->parseSplitComponentsData();
	}
}

void TextParserASP::reset()
{
	BaseTextParser::reset();
	withoutWeldedPipe = false;
	existComponentsInEndText = false;
	pageStartWithContFrom = false;
	cipherDocumentStr = L"";
}
