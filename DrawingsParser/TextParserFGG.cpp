#include "TextParserFGG.h"
#include "DrawingPageFGG.h"
#include "BuildComponentFGG.h"
#include "ValuesCheker.h"

#include <regex>
#include <string>

TextParserFGG::TextParserFGG(const std::wstring& text, wchar_t separator) : BaseTextParser(text, separator)
{

}

bool TextParserFGG::readComponent()
{
	if (!readComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 3; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}
	bool match{ false };

	int descriptionSubStrCount{ 1 };
	while (!match)
	{
		match = std::regex_match(*(subStrBuffer.end() - 2), lastComponentPtr->getNominalDiameterPattern());
		match = match && std::regex_match(*(subStrBuffer.end() - 1), lastComponentPtr->getAmountPattern());

		if (!match)
		{
			descriptionSubStrCount++;
			if (descriptionSubStrCount > 4) {
				return false;
			}
			subStrBuffer.emplace_back(getNextSubString());
		}
	}

	std::wstring descriptionStr;
	for (int i = 0; i < descriptionSubStrCount; i++)
	{
		std::wstring spase = i == 0 ? L"" : L" ";
		descriptionStr += spase + subStrBuffer[i];
	}
	lastComponentPtr->trySetDescription(descriptionStr);

	lastComponentPtr->trySetNominalDiameter(*(subStrBuffer.end() - 2));
	lastComponentPtr->trySetAmount(*(subStrBuffer.end() - 1));
	return true;
}

bool TextParserFGG::readComponentNumber()
{
	if (componentsEnded) {
		return false;
	}

	std::wstring componentNumberStr(getNextSubString());

	size_t currentPosition = currentPositionInText;
	std::wstring nextSubStr = getNextSubStringFromPosition(currentPosition);
	if (lastDrawingPagePtr != nullptr && lastDrawingPagePtr->trySetPages(componentNumberStr, nextSubStr, ValuesCheker::Type::NONE))
	{
		componentsEnded = true;
		return false;
	}

	//Если не найден номер компонента
	if (!std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern()))
	{
		if (componentNumberStr.starts_with(L"FABRICATION MATERIALS "))
		{
			moveToNextSubString(L"КОЛ-В");
			moveToNextSubString();
		}
		else if (componentNumberStr.starts_with(L"CUT PIPE") || componentNumberStr.starts_with(L"**********"))
		{
			componentsEnded = true;
			if (componentNumberStr.starts_with(L"**********") && continueListName.empty())
			{
				continueListName = getNextSubString();
				size_t subStrBegin = continueListName.find_last_of(L' ');
				continueListName = continueListName.substr(subStrBegin, continueListName.length() - subStrBegin);
			}
		}
		return false;
	}
	else
	{
		createDrawing<DrawingPageFGG>();

		tryAddComponent<BuildComponentFGG>(componentNumberStr);
		return true;
	}
}

void TextParserFGG::readTablePartData()
{
	moveToPreviouslySubString(L"All flanges and");
	moveOnCountSubStr(3, true);
	bool skipOneSubStr = false;
	if (!lastDrawingPagePtr->trySetDiameterPipeline(getPreviouslySubString(), ValuesCheker::Type::NONE))
	{
		lastDrawingPagePtr->trySetDiameterPipeline(getPreviouslySubString());
		skipOneSubStr = true;
	}


	std::wstring gostCategory[2] = { getPreviouslySubString(), getPreviouslySubString() };
	//if (gostCategory[0] == L"B" || gostCategory[0] == L"В")
	//{
		lastDrawingPagePtr->trySetGOSTPipelineCategory(gostCategory[1] + L" / " + gostCategory[0]);
		lastDrawingPagePtr->trySetCategoryPipelinesTRCU(getPreviouslySubString());
	//} 
	//else
	//{
	//	lastDrawingPagePtr->trySetGOSTPipelineCategory(gostCategory[0]);
	//	lastDrawingPagePtr->trySetCategoryPipelinesTRCU(gostCategory[1]);
	//}

	lastDrawingPagePtr->trySetTechnologicalEnvironment(getPreviouslySubString());
	lastDrawingPagePtr->trySetIsolation(getPreviouslySubString());
	lastDrawingPagePtr->trySetTracing(getPreviouslySubString());
	lastDrawingPagePtr->trySetPaintingSystem(getPreviouslySubString());
	lastDrawingPagePtr->trySetStressCalculation(getPreviouslySubString());
	lastDrawingPagePtr->trySetPostWeldingHeatTreatment(getPreviouslySubString());
	std::wstring testEnvironmentStr = getPreviouslySubString();
	if (lastDrawingPagePtr->trySetTestEnvironment(testEnvironmentStr, ValuesCheker::Type::NONE)) {
		lastDrawingPagePtr->trySetPipelineClass(getPreviouslySubString());
	}
	else
	{
		if (!lastDrawingPagePtr->trySetPipelineClass(testEnvironmentStr, ValuesCheker::Type::NONE))
		{
			if (lastDrawingPagePtr->trySetWeldInspection(testEnvironmentStr, ValuesCheker::Type::NONE))
			{
				lastDrawingPagePtr->trySetTestEnvironment(getPreviouslySubString());
				lastDrawingPagePtr->trySetPipelineClass(getPreviouslySubString());
			}
			else
			{
				lastDrawingPagePtr->trySetPipelineClass(getPreviouslySubString());
			}
		}
	}

	if (skipOneSubStr) {
		moveToPreviouslySubString();
	}
	std::wstring listsStr = getPreviouslySubString();
	lastDrawingPagePtr->trySetPages(getPreviouslySubString(), listsStr);

	lastDrawingPagePtr->trySetSchemeNumber(getNextSubString(L"WELD DIAMETER"));
	lastDrawingPagePtr->trySetDesignTemperature(getNextSubString());
	lastDrawingPagePtr->trySetOperatingTemperature(getNextSubString());
	lastDrawingPagePtr->trySetDesignPressure(getNextSubString());
	lastDrawingPagePtr->trySetOperatingPressure(getNextSubString());
	lastDrawingPagePtr->trySetTestPressure(getNextSubString(), ValuesCheker::Type::NONE);
	std::wstring weldInspectionStr = getNextSubString();
	if (lastDrawingPagePtr->trySetWeldInspection(weldInspectionStr, ValuesCheker::Type::NONE)) {
		lastDrawingPagePtr->trySetLineNumber(getNextSubString());
	}
	else {
		lastDrawingPagePtr->trySetLineNumber(weldInspectionStr);
	}
}

void TextParserFGG::parse(const std::wstring& fileName, std::vector<Drawing>& drawings)
{
	drawingsPtr = &drawings;
	reset();

	while (getNextSubString(L"КОЛ-В") != L"") 
	{
		tryAddPage<DrawingPageFGG>();
		componentsEnded = false;
		moveToNextSubString();
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

	/*std::wstring parsedSubStr = getPreviouslySubString(L"===== begin block");
	parsedSubStr = parsedSubStr.erase(parsedSubStr.size() - 2);

	while (parsedSubStr.starts_with(L"Pipe supports are") || parsedSubStr.starts_with(L"10mm hole")) {
		parsedSubStr = getPreviouslySubString();
	}*/

	moveOnCountSubStr(3);
	std::wstring parsedSubStr = L"";
	while (!std::regex_match(parsedSubStr, lastDrawingPagePtr->getTotalPagesPattern()))
	{
		parsedSubStr = getNextSubString();
	}

	if (lastDrawingPagePtr != nullptr)
	{
		while (lastDrawingPagePtr->getTestEnvironment() == L"-" || lastDrawingPagePtr->getTestPressure() == L"-" || lastDrawingPagePtr->getWeldInspection() == L"-")
		{
			bool isTestEnvironment = false, isTestPressure = false, isWeldInspection = false;
			do 
			{
				parsedSubStr = getNextSubString();
				isTestEnvironment = currentDrawingPtr->pages[0]->trySetTestEnvironment(parsedSubStr, ValuesCheker::Type::NONE);
				isTestPressure = currentDrawingPtr->pages[0]->trySetTestPressure(parsedSubStr, ValuesCheker::Type::NONE);
				isWeldInspection = currentDrawingPtr->pages[0]->trySetWeldInspection(parsedSubStr, ValuesCheker::Type::NONE);
			} while(!isTestEnvironment && !isTestPressure && !isWeldInspection && !parsedSubStr.starts_with(L"===== end block"));

			for (size_t i = 1; i < currentDrawingPtr->pages.size(); i++)
			{
				if (isTestEnvironment) {
					currentDrawingPtr->pages[i]->trySetTestEnvironment(parsedSubStr);
				}
				else if (isTestPressure) {
					currentDrawingPtr->pages[i]->trySetTestPressure(parsedSubStr);
				}
				else if (isWeldInspection)
				{
					currentDrawingPtr->pages[i]->trySetWeldInspection(parsedSubStr);
				}
			}

			//bool isTestEnvironment = currentDrawingPtr->pages[0]->trySetTestEnvironment(parsedSubStr, ValuesCheker::Type::NONE);
			//bool isTestPressure = currentDrawingPtr->pages[0]->trySetTestPressure(parsedSubStr, ValuesCheker::Type::NONE);
			//bool isWeldInspection = currentDrawingPtr->pages[0]->trySetWeldInspection(parsedSubStr, ValuesCheker::Type::NONE);
			//
			//for (size_t i = 1; i < currentDrawingPtr->pages.size(); i++)
			//{
			//	if (isTestEnvironment) { 
			//		currentDrawingPtr->pages[i]->trySetTestEnvironment(parsedSubStr);
			//	}
			//	else if (isTestPressure) { 
			//		currentDrawingPtr->pages[i]->trySetTestPressure(parsedSubStr);
			//	}
			//	else if (isWeldInspection) 
			//	{
			//		bool success = currentDrawingPtr->pages[i]->trySetWeldInspection(parsedSubStr, ValuesCheker::Type::NONE);
			//		if (!success)
			//		{
			//			parsedSubStr = getPreviouslySubString(L"===== end block");
			//			parsedSubStr = parsedSubStr.erase(parsedSubStr.size() - 2);
			//			while (!currentDrawingPtr->pages[i]->trySetWeldInspection(parsedSubStr, ValuesCheker::Type::NONE))
			//			{
			//				parsedSubStr = getPreviouslySubString();
			//			}
			//		}
			//	}
			//	else { 
			//		break; 
			//	};
			//}

			//if (!isTestEnvironment && !isTestPressure && !isWeldInspection) {
			//	break; 
			//}

			//if (isWeldInspection) {
			//	moveToSubString(L"===== begin block", true);
			//}
			//parsedSubStr = getPreviouslySubString();
		}
	}


	//if (lastDrawingPagePtr != nullptr && lastDrawingPagePtr->getTestEnvironment() == L"-")
	//{
	//	for (size_t i = 0; i < currentDrawingPtr->pages.size(); i++)
	//	{
	//		currentDrawingPtr->pages[i]->trySetTestEnvironment(parsedSubStr);
	//	}
	//	parsedSubStr = getPreviouslySubString();
	//}

	//if (lastDrawingPagePtr != nullptr && lastDrawingPagePtr->getTestPressure() == L"-")
	//{
	//	for (size_t i = 0; i < currentDrawingPtr->pages.size(); i++)
	//	{
	//		currentDrawingPtr->pages[i]->trySetTestPressure(parsedSubStr);
	//	}
	//	parsedSubStr = getPreviouslySubString();
	//}

	//if (lastDrawingPagePtr != nullptr && lastDrawingPagePtr->getWeldInspection() == L"-")
	//{
	//	bool success = false;
	//	for (size_t i = 0; i < currentDrawingPtr->pages.size(); i++)
	//	{
	//		success = currentDrawingPtr->pages[i]->trySetWeldInspection(parsedSubStr, ValuesCheker::Type::NONE);
	//		if (!success) {
	//			break;
	//		}
	//	}

	//	if (!success)
	//	{
	//		parsedSubStr = getPreviouslySubString(L"===== end block");
	//		parsedSubStr = parsedSubStr.erase(parsedSubStr.size() - 2);
	//		for (size_t i = 0; i < currentDrawingPtr->pages.size(); i++)
	//		{
	//			success = currentDrawingPtr->pages[i]->trySetWeldInspection(parsedSubStr, ValuesCheker::Type::NONE);
	//			if (!success) {
	//				break;
	//			}
	//		}
	//		moveToSubString(L"===== begin block", true);
	//	}
	//}

	std::wstring fileNameStr = getNextSubString(L"Формат");
	std::wstring cipherDocumentStr = getNextSubString();
	for (size_t i = 0; i < currentDrawingPtr->pages.size(); i++)
	{
		currentDrawingPtr->pages[i]->trySetFileName(fileNameStr);
		currentDrawingPtr->pages[i]->trySetCipherDocument(cipherDocumentStr);
	}
}