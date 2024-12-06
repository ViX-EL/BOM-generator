#include "TextParserASP.h"

#include <string>
#include <vector>
#include <regex>
#include <cassert>
#include <stack>

bool TextParserASP::readLastComponentNumber()
{
	if (nextComponentNumberMissing) {
		nextComponentNumberMissing = false;
		return true;
	}
	if (componentsEnded) {
		return false;
	}

	size_t currentTextPos = currentPositionInText;
	if (!tryMoveToNextSubString(currentTextPos)) {
		writeComponentsCount();
		return false;
	}

	currentTextPos = currentPositionInText;
	if (withoutWeldedPipe || cipherDocumentStr.contains(L"-PT-"))
	{
		std::wstring twoNextSubStrs[2] = { getNextSubString(currentTextPos), getNextSubString(currentTextPos) };
		std::wregex listAndListsPattern(LR"(\d+)");
		if (std::regex_match(twoNextSubStrs[0], listAndListsPattern) && std::regex_match(twoNextSubStrs[1], listAndListsPattern))
		{
			(*columns)[L"Лист"].emplace_back(getNextSubString());
			(*columns)[L"Листов"].emplace_back(getNextSubString());
			writeComponentsCount();
			return false;
		}
	}

	std::wstring componentNumberStr(getNextSubString());

	if (readFirstPipeInEndText(componentNumberStr)) {
		return true;
	}

	currentTextPos = currentPositionInText;
	std::wregex beginDescriptionPattern(LR"([A-Za-z0-9  -.]+\/[А-Яа-я0-9  -.]+)");
	if (!std::regex_search(getNextSubString(currentTextPos), beginDescriptionPattern))
	{
		std::wstring schemeNumberStr = getNextSubString(currentTextPos);
		std::wregex schemeNumberPattern(L"GCC-ASP-DDD");
		if (std::regex_search(schemeNumberStr, schemeNumberPattern) || schemeNumberStr.starts_with(L"          "))
		{
			writeComponentsCount();
			return false;
		}
	}

	std::wregex componentNumberPattern(LR"( *\d+)");
	bool cutPipeLengthFinded = searchForMatchesInFollowing(std::wregex(L"CUT PIPE LENGTH"), 3);
	if (!std::regex_match(componentNumberStr, componentNumberPattern) || cutPipeLengthFinded)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") || (componentNumberStr.starts_with(L"*********") && (lastComponentNumber != 0))) 
		{
			if (existComponentsInEndText) {
				readComponensFromEndText();
			}
			writeComponentsCount();
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) 
		{
			std::wstring subStr = getNextSubString(L"КОЛ-ВО");
			if (subStr.contains(L"*********")) {
				writeComponentsCount();
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
		int componentNumber = stoi(componentNumberStr);
		if (componentNumber == 1)
		{
			currentTextPos = currentPositionInText;
			std::wstring pipeStr = getPreviouslySubString(currentTextPos);
			if (!pipeStr.contains(L"PIPE")) {
				withoutWeldedPipe = true;
			}
		}
		lastComponentNumber++;
		if (!listStartWithContFrom && componentNumber != lastComponentNumber) {
			existComponentsInEndText = true;
		}
		return true;
	}
}

void TextParserASP::writeComponentsCount()
{
	if (lastComponentNumber != 0) {
		componentsCountPerList->push_back(lastComponentNumber);
	}
	componentsEnded = true;
}

void TextParserASP::readComponensFromEndText()
{
	std::wstring currentSubStr(getLastSubString());
	std::wstring currentPreviousSubStr(getPreviouslySubString());
	std::wregex numberPattern(LR"( *\d+)");
	do 
	{
		lastComponentNumber++;
		(*columns)[L"Кол-во"].emplace_back(currentSubStr);
		(*columns)[L"Условный диаметр"].emplace_back(currentPreviousSubStr);
		currentSubStr = getPreviouslySubString();
		std::stack<std::wstring> descriptionStack;
		while (!std::regex_match(currentSubStr, numberPattern))
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
		(*columns)[L"Описание компонента"].emplace_back(descriptionStr);
		(*columns)[L"Документ"].emplace_back(L"-");
		(*columns)[L"Код позиции"].emplace_back(L"-");
		currentSubStr = getPreviouslySubString();
		currentPreviousSubStr = getPreviouslySubString();
	} while (std::regex_match(currentSubStr, numberPattern) && std::regex_match(currentPreviousSubStr, numberPattern));
}

bool TextParserASP::readFirstPipeInEndText(std::wstring& currentSubStr)
{
	size_t currentTextPos = currentPositionInText;
	std::wstring prevSubStr(getPreviouslySubString(currentTextPos));
	if (currentSubStr == L"FITTINGS/ ФИТИНГИ" && prevSubStr == L"PIPE/ ТРУБА")
	{
		moveToLastSubString();
		(*columns)[L"Шифр документа"].emplace_back(cipherDocumentStr);
		std::wstring positionCodeStr;
		while (positionCodeStr.size() < 15) {
			positionCodeStr = getPreviouslySubString();
		}
		(*columns)[L"Код позиции"].emplace_back(getNextSubString());
		moveToPreviouslySubString();
		std::wstring descriptionStr = getPreviouslySubString();
		descriptionStr = descriptionStr + L" " + getNextSubString();
		(*columns)[L"Описание компонента"].emplace_back(descriptionStr);
		std::wregex countPattern(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
		std::wstring countStr;
		while (!std::regex_match(countStr, countPattern))
		{
			countStr = getPreviouslySubString();
		}
		(*columns)[L"Кол-во"].emplace_back(countStr);
		(*columns)[L"Условный диаметр"].emplace_back(getPreviouslySubString());
		(*columns)[L"Документ"].emplace_back(L"-");
		moveToNextSubString(L"FITTINGS/ ФИТИНГИ", true);
		lastComponentNumber += 2;
		return true;
	}
	return false;
}

bool TextParserASP::readListAndListsCount(const std::wstring& listCountStr, 
	const std::wstring& currentListStr)
{
	std::wregex listsPattern(LR"(\d{1,2})");
	if (std::regex_match(listCountStr, listsPattern) && std::regex_match(currentListStr,
		listsPattern)) 
	{
		(*columns)[L"Листов"].emplace_back(listCountStr);
		(*columns)[L"Лист"].emplace_back(currentListStr);
		return true;
	}
	else {
		return false;
	}
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

void TextParserASP::parseCase1(std::wstring designTemperatureStr)
{
	(*columns)[L"Расчет. Темп"].emplace_back(designTemperatureStr);
	(*columns)[L"Расчет. Давление"].emplace_back(getNextSubString(L"РАСЧЕТ. ДАВЛЕНИЕ", true));
	(*columns)[L"Рабочая температура"].emplace_back(getNextSubString(L"РАБОЧАЯ ТЕМП.", true));
	(*columns)[L"Рабочее давление"].emplace_back(getNextSubString(L"РАБОЧЕЕ ДАВЛЕНИЕ", true));

	std::wstring isometricDrawingStr = getSubString(L"Изометрический чертеж", true);
	if (isometricDrawingStr == L"") {
		isometricDrawingStr = getSubString(L"Изометрический чертеж");
	}
	if (isometricDrawingStr != L"") {
		isometricDrawingStr = isometricDrawingStr.erase(0, 22);
	} 
	else {
		isometricDrawingStr = L"-";
	}
	(*columns)[L"Изометрический чертеж"].emplace_back(isometricDrawingStr);

	if (cipherDocumentStr != L"") {
		(*columns)[L"Шифр документа"].emplace_back(cipherDocumentStr);
	}
	else {
		(*columns)[L"Шифр документа"].emplace_back(L"-");
	}

	std::wstring lineNumberStr(getNextSubString(L"Line number"));
	if (lineNumberStr.starts_with(L"Line")) {
		lineNumberStr = L"-";
	}
	else if (lineNumberStr == L"") {
		lineNumberStr = getNextSubString(L"Line number", true);
	}
	(*columns)[L"Номер линии"].emplace_back(lineNumberStr);

	moveToPreviouslySubString();
	setDashIfValueMissing(L"Line number", L"КЛАСС", L"Класс трубопровода");
	(*columns)[L"Диаметр трубопровода"].emplace_back(getPreviouslySubString(L"PIPE CLASS", true));
	setDashIfValueMissing(L"PIPE DIAM", L"КАТЕГОРИЯ", L"Категория трубопр. Гост");
	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getPreviouslySubString(L"PIPE CATEGORY", true));
	setDashIfValueMissing(L"PIPE CATEGORY SN", L"ТЕХНОЛОГИЧЕСКАЯ", L"Технологическая среда");
	(*columns)[L"Изоляция"].emplace_back(getPreviouslySubString(L"FLUID SERVICE", true));
	setDashIfValueMissing(L"INSULATION", L"СПУТНИКОВЫЙ", L"Спутниковый обогрев");
	setDashIfValueMissing(L"TRACING", L"СИСТЕМА", L"Система покраски");
	(*columns)[L"Расчет напряжений"].emplace_back(getPreviouslySubString(L"PAINTING SYSTEM", true));
	setDashIfValueMissing(L"CRITICAL LINE", L"ПОСЛЕСВАР", L"Послесвар. Термообраб");
	setDashIfValueMissing(L"POST WELD", L"КОНТРОЛЬ", L"Контроль сварных швов");
	setDashIfValueMissing(L"WELD INSPECTION", L"СРЕДА", L"Среда испытаний");
	setDashIfValueMissing(L"TEST MEDIUM", L"ДАВЛЕНИЕ", L"Давление испыт");

	std::wstring schemeNumber = getPreviouslySubString(L"For general", true);
	if (schemeNumber.starts_with(L"GCC-ASP-DDD-")) {
		(*columns)[L"Номер схемы"].emplace_back(schemeNumber);
	}

	std::wstring listCountStr = getPreviouslySubString(L"LEGEND", true);
	std::wstring currentListStr = getPreviouslySubString();
	if (!readListAndListsCount(listCountStr, currentListStr))
	{
		std::wregex schemeNumberPattern(LR"(\d+-GCC-ASP-DDD-\d+-\d+-\d+-\w+-\w+-\d+)");
		if (std::regex_search(listCountStr, schemeNumberPattern)) {
			(*columns)[L"Номер схемы"].emplace_back(listCountStr);
			listCountStr = currentListStr;
			currentListStr = getPreviouslySubString();
			if (!readListAndListsCount(listCountStr, currentListStr))
			{
				(*columns)[L"Листов"].emplace_back(L"1");
				(*columns)[L"Лист"].emplace_back(L"1");
			}
		}
		else {
			(*columns)[L"Номер схемы"].emplace_back(L"-");
			(*columns)[L"Листов"].emplace_back(L"1");
			(*columns)[L"Лист"].emplace_back(L"1");
		}
	}
	else
	{
		(*columns)[L"Номер схемы"].emplace_back(L"-");
	}

	if (cipherDocumentStr.contains(L"-PT-"))
	{
		std::wstring generalNotesStr = getSubString(std::wregex(LR"(GCC-ASP-DDD-\d+-\d+-\d+-PT-LST-\d+)"));
		std::wstring schemeNumber = getNextSubString();

		if (*((*columns)[L"Номер схемы"].end() - 1) == L"-") {
			*((*columns)[L"Номер схемы"].end() - 1) = schemeNumber;
		}

		std::wregex numberPattern(LR"(\d+)");
		moveToPreviouslySubString();
		if (*((*columns)[L"Система покраски"].end() - 1) == L"-") 
		{
			std::wstring paintingSystemStr = getPreviouslySubString();
			std::wregex paintingSystemPattern(L"A1->?A4|B1->?B2|NO / НЕТ|В1-В2|Система A, прим. п.1|Система B, прим. п.1");
			if (!std::regex_match(paintingSystemStr, numberPattern)) 
			{
				if (std::regex_match(paintingSystemStr, paintingSystemPattern)) {
					*((*columns)[L"Система покраски"].end() - 1) = paintingSystemStr;
				}
				else {
					*((*columns)[L"Технологическая среда"].end() - 1) = paintingSystemStr;
					*((*columns)[L"Система покраски"].end() - 1) = getPreviouslySubString();
				}
			}
			else {
				moveOnCountSubStr(2);
				*((*columns)[L"Система покраски"].end() - 1) = getNextSubString();
			}
		}

		std::wstring testEnvironmentStr = getSubString(std::wregex(LR"(вода/water|WATER/ВОДА|^-$)"));
		if (testEnvironmentStr != L"") 
		{
			if (*((*columns)[L"Среда испытаний"].end() - 1) == L"-") {
				*((*columns)[L"Среда испытаний"].end() - 1) = testEnvironmentStr;
			}
			if (*((*columns)[L"Контроль сварных швов"].end() - 1) == L"-") {
				*((*columns)[L"Контроль сварных швов"].end() - 1) = getNextSubString();
			}
		}
		std::wregex datePattern(LR"(\d+-\d+-\d+)");
		if (*((*columns)[L"Технологическая среда"].end() - 1) == L"-") 
		{
			std::wstring technologicalEnvironmentStr = getNextSubString();
			if (!std::regex_match(technologicalEnvironmentStr, datePattern) && !std::regex_match(technologicalEnvironmentStr, numberPattern)) {
				*((*columns)[L"Технологическая среда"].end() - 1) = technologicalEnvironmentStr;
			}
			else {
				moveOnCountSubStr(2, true);
				*((*columns)[L"Технологическая среда"].end() - 1) = getPreviouslySubString();
				moveOnCountSubStr(2);
			}
		}
		if (*((*columns)[L"Спутниковый обогрев"].end() - 1) == L"-") 
		{
			std::wstring tracingStr;
			do {
				tracingStr = getNextSubString();
			} while (std::regex_match(tracingStr, datePattern) || std::regex_match(tracingStr, numberPattern) || tracingStr.starts_with(L"Issued for"));
			*((*columns)[L"Спутниковый обогрев"].end() - 1) = tracingStr;
		}
		readDescriptionsFromEndText();
	}
}

void TextParserASP::parseCase2()
{
	std::wregex designPressurePattern(LR"(\d+|\d+.\d+)");
	std::wstring designPressure = getPreviouslySubString(L"Pipe surface", true);
	if (designPressure == L"") {
		designPressure = getPreviouslySubString(L"Rev.", true);
	}
	std::wregex testPresurePattern(LR"(\d+\.\d+|\d+|-)");
	std::wstring testPressureStr;
	if (std::regex_match(designPressure, designPressurePattern))
	{
		(*columns)[L"Расчет. Давление"].emplace_back(designPressure);
		(*columns)[L"Расчет. Темп"].emplace_back(getPreviouslySubString());
		(*columns)[L"Рабочее давление"].emplace_back(getPreviouslySubString());
		(*columns)[L"Рабочая температура"].emplace_back(getPreviouslySubString());
		(*columns)[L"Номер схемы"].emplace_back(getPreviouslySubString());
		(*columns)[L"Номер линии"].emplace_back(getPreviouslySubString());
		testPressureStr = getPreviouslySubString();
		if (std::regex_match(testPressureStr, testPresurePattern)) {
			(*columns)[L"Давление испыт"].emplace_back(testPressureStr);
		}
		else {
			(*columns)[L"Давление испыт"].emplace_back(L"-");
		}
	}

	if (std::regex_match(testPressureStr, testPresurePattern)) {
		(*columns)[L"Среда испытаний"].emplace_back(getPreviouslySubString());
	} 
	else {
		(*columns)[L"Среда испытаний"].emplace_back(testPressureStr);
	}
	(*columns)[L"Контроль сварных швов"].emplace_back(getPreviouslySubString());
	(*columns)[L"Послесвар. Термообраб"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(3, true);

	std::wstring columnsNames[] = { L"Расчет напряжений", L"Система покраски", L"Спутниковый обогрев", L"Изоляция" };
	std::vector<std::wstring> columnsValues;
	std::wregex generalNotes(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-\w+-\d+)");
	do {
		columnsValues.emplace_back(getPreviouslySubString());
	} while (!std::regex_match(columnsValues[columnsValues.size() - 1], generalNotes));

	for (size_t i = 0; i < 4; i++)
	{
		if (columnsValues.size() < 5 && i == 1)
		{
			(*columns)[columnsNames[i]].emplace_back(L"-");
			continue;
		}
		if (columnsValues.size() < 5 && i != 0) {
			(*columns)[columnsNames[i]].emplace_back(columnsValues[i - 1]);
		}
		else {
			(*columns)[columnsNames[i]].emplace_back(columnsValues[i]);
		}
	}

	(*columns)[L"Технологическая среда"].emplace_back(getPreviouslySubString());
	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getPreviouslySubString());
	(*columns)[L"Категория трубопр. Гост"].emplace_back(getPreviouslySubString());
	(*columns)[L"Диаметр трубопровода"].emplace_back(getPreviouslySubString());
	(*columns)[L"Класс трубопровода"].emplace_back(getPreviouslySubString());
	(*columns)[L"Шифр документа"].emplace_back(cipherDocumentStr);

	(*columns)[L"Изометрический чертеж"].emplace_back(getSubString(L"Изометрический чертеж", true).erase(0, 22));
	moveOnCountSubStr(2, true);
	(*columns)[L"Листов"].emplace_back(getPreviouslySubString());
	(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
}

void TextParserASP::parseCase3()
{
	(*columns)[L"Изометрический чертеж"].emplace_back(getSubString(L"Изометрический чертеж", true).erase(0, 22));
	std::wstring testEnvironmentStr(getPreviouslySubString(L"Pipe surface"));
	std::wregex lineNumberPattern(LR"(\d+-\w+-\d+-\d+-\d+-[A-Z0-9]+-\w+)");
	if (testEnvironmentStr == L"") {
		(*columns)[L"Номер линии"].emplace_back(getPreviouslySubString(L"Stress sketch"));
		(*columns)[L"Среда испытаний"].emplace_back(getPreviouslySubString());
	}
	else {
		if (!std::regex_match(testEnvironmentStr, lineNumberPattern)) {
			(*columns)[L"Среда испытаний"].emplace_back(testEnvironmentStr);
		}
		else {
			(*columns)[L"Номер линии"].emplace_back(testEnvironmentStr);
			(*columns)[L"Среда испытаний"].emplace_back(getPreviouslySubString());
		}
	}
	(*columns)[L"Контроль сварных швов"].emplace_back(getPreviouslySubString());
	(*columns)[L"Послесвар. Термообраб"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(3, true);
	(*columns)[L"Расчет напряжений"].emplace_back(getPreviouslySubString());
	(*columns)[L"Система покраски"].emplace_back(getPreviouslySubString());
	(*columns)[L"Спутниковый обогрев"].emplace_back(getPreviouslySubString());
	(*columns)[L"Изоляция"].emplace_back(getPreviouslySubString());
	moveToPreviouslySubString();
	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getPreviouslySubString());
	(*columns)[L"Категория трубопр. Гост"].emplace_back(getPreviouslySubString());
	(*columns)[L"Диаметр трубопровода"].emplace_back(getPreviouslySubString());
	(*columns)[L"Класс трубопровода"].emplace_back(getPreviouslySubString());
	(*columns)[L"Шифр документа"].emplace_back(cipherDocumentStr);

	std::wstring schemeNumber(getSubString(L"Номер схемы").erase(0, 13));
	if (schemeNumber == L"") {
		schemeNumber = L"-";
	}
	(*columns)[L"Номер схемы"].emplace_back(schemeNumber);
	if (testEnvironmentStr != L"" && !std::regex_match(testEnvironmentStr, lineNumberPattern)) {
		(*columns)[L"Номер линии"].emplace_back(getNextSubString().erase(0, 24));
	}
	moveToNextSubString(L"Issued for");
	size_t currPos = currentPositionInText;
	moveOnCountSubStr(currPos, 3, true);
	std::wstring operatingPressureStr(getPreviouslySubString(currPos));
	std::wregex operatingPressurePattern(LR"(\d+(\.\d+)?|\d+(\.\d+)?\W+-\d+|атм\.)");
	if (std::regex_match(operatingPressureStr, operatingPressurePattern) || operatingPressureStr == L"-")
	{
		if (operatingPressureStr == L"-")
		{
			(*columns)[L"Давление испыт"].emplace_back(operatingPressureStr);
			(*columns)[L"Расчет. Давление"].emplace_back(getPreviouslySubString(currPos));
			std::wstring designTemperatureStr = getPreviouslySubString(currPos);
			if (designTemperatureStr == L"-") {
				(*columns)[L"Расчет. Темп"].emplace_back(designTemperatureStr);
				(*columns)[L"Рабочее давление"].emplace_back(getPreviouslySubString(currPos));
			}
			else {
				(*columns)[L"Рабочее давление"].emplace_back(designTemperatureStr);
			}
			(*columns)[L"Рабочая температура"].emplace_back(getPreviouslySubString(currPos));
			if (designTemperatureStr != L"-") {
				(*columns)[L"Расчет. Темп"].emplace_back(getPreviouslySubString(currPos));
			}
			(*columns)[L"Технологическая среда"].emplace_back(getLastSubString());
		}
		else
		{
			(*columns)[L"Рабочее давление"].emplace_back(operatingPressureStr);
			(*columns)[L"Рабочая температура"].emplace_back(getPreviouslySubString(currPos));
			(*columns)[L"Давление испыт"].emplace_back(getPreviouslySubString(currPos));
			(*columns)[L"Расчет. Давление"].emplace_back(getPreviouslySubString(currPos));
			(*columns)[L"Расчет. Темп"].emplace_back(getPreviouslySubString(currPos));
			(*columns)[L"Технологическая среда"].emplace_back(getLastSubString());
		}

		readDescriptionsFromEndText();
		return;
	}
	moveOnCountSubStr(2);
	currPos = currentPositionInText;
	if (tryMoveOnCountSubStr(currPos, 6, false)) 
	{
		(*columns)[L"Расчет. Давление"].emplace_back(getNextSubString());
		(*columns)[L"Расчет. Темп"].emplace_back(getNextSubString());
		(*columns)[L"Давление испыт"].emplace_back(getNextSubString());
		(*columns)[L"Рабочая температура"].emplace_back(getNextSubString());
		(*columns)[L"Рабочее давление"].emplace_back(getNextSubString());
		(*columns)[L"Технологическая среда"].emplace_back(getNextSubString());
	}
	else
	{
		currPos = currentPositionInText;
		if (tryMoveToNextSubString(currPos)) 
		{
			(*columns)[L"Технологическая среда"].emplace_back(getNextSubString());
			moveOnCountSubStr(6, true);
			(*columns)[L"Расчет. Давление"].emplace_back(getPreviouslySubString());
			(*columns)[L"Расчет. Темп"].emplace_back(getPreviouslySubString());
			(*columns)[L"Рабочее давление"].emplace_back(getPreviouslySubString());
			(*columns)[L"Рабочая температура"].emplace_back(getPreviouslySubString());
			(*columns)[L"Давление испыт"].emplace_back(getPreviouslySubString());
		}
		else
		{
			moveToPreviouslySubString(L"Issued for", true);
			moveToPreviouslySubString();
			(*columns)[L"Технологическая среда"].emplace_back(getPreviouslySubString());
			moveOnCountSubStr(5, true);
			(*columns)[L"Давление испыт"].emplace_back(getPreviouslySubString());
			(*columns)[L"Расчет. Давление"].emplace_back(getPreviouslySubString());
			(*columns)[L"Расчет. Темп"].emplace_back(getPreviouslySubString());
			(*columns)[L"Рабочее давление"].emplace_back(getPreviouslySubString());
			(*columns)[L"Рабочая температура"].emplace_back(getPreviouslySubString());
		}
	}
}

void TextParserASP::readDescriptionsFromEndText()
{
	std::wstring currentSubStr(getSubString(std::wregex(LR"(вода/water|WATER/ВОДА|^-$)")));
	auto componentDescriptionIter = (*columns)[L"Описание компонента"].end() - 1;
	for (size_t i = 0; i < lastComponentNumber; i++)
	{
		if (*(componentDescriptionIter - i) == L"-") 
		{
			std::stack<std::wstring> descriptionStack;
			currentSubStr = getPreviouslySubString();
			std::wregex beginDescriptionPattern(LR"([A-Za-z0-9  -.]+\/[А-Яа-я0-9  -.]+)");
			while (!std::regex_search(currentSubStr, beginDescriptionPattern))
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
			*(componentDescriptionIter - i) = descriptionStr;
		}
	}
}

void TextParserASP::setDashIfValueMissing(const std::wstring& nextSubStr, const std::wstring& unwantedSubStr, const std::wstring& columnName)
{
	std::wstring subStr(getPreviouslySubString(nextSubStr, true));
	if (subStr.starts_with(unwantedSubStr)) {
		subStr = L"-";
	}
	(*columns)[columnName].emplace_back(subStr);
}

TextParserASP::TextParserASP(const std::wstring& text, Columns& columns,
	std::vector<int>& componentsCountPerList, wchar_t separator) :
	BaseTextParser(text, columns, componentsCountPerList, separator)
{

}

void TextParserASP::parse(const std::wstring& fileName)
{
	reset();

	//if (fileName.contains(L"GCC-ASP-DDD-26160-14-2200-TKM-ISO-00065"))
	//{
	//	int x = 2;
	//	x = x * 2;
	//}

	cipherDocumentStr = getSubString(std::wregex(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-ISO-\d+)"));

	if (getFirstSubString().starts_with(L"*********")) 
	{
		listStartWithContFrom = true;
		moveOnCountSubStr(2);
	}
	else {
		moveToNextSubString(L"КОЛ-ВО");
	}

	while (true) //Чтение всех компонентов
	{
		if (!BaseTextParser::readComponent(columns))
		{
			if (!BaseTextParser::readComponent(columns)) {
				break;
			}
		}
	}

	if (lastComponentNumber > 0) {
		assert(listsCountBeforeStart < componentsCountPerList->size() && "Ошибка: компоненты были добавлены, но колличество компонентов не было записано!");
	}

	if (lastComponentNumber > 0 && listsCountBeforeStart < componentsCountPerList->size())
	{
		readTablePartData();

		(*columns)[L"Имя файла"].emplace_back(fileName);
		if (((*columns)[L"Контроль сварных швов"].end() - 1)->ends_with(L"%%%")) 
		{
			size_t strSize = ((*columns)[L"Контроль сварных швов"].end() - 1)->size();
			((*columns)[L"Контроль сварных швов"].end() - 1)->erase(strSize - 2, strSize - 1);
		}
	}

	assert((*columns)[L"Имя файла"].size() == componentsCountPerList->size() && "Ошибка: неизвестная ошибка!");
}

void TextParserASP::reset()
{
	BaseTextParser::reset();
	withoutWeldedPipe = false;
	existComponentsInEndText = false;
	listStartWithContFrom = false;
}
