#include "TextParserASP.h"

#include <string>
#include <vector>
#include <regex>

bool TextParserASP::readLastComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	//Если не найден номер компонента
	std::wregex componentNumberPattern(LR"( *\d+)");
	if (!std::regex_match(componentNumberStr, componentNumberPattern)) 
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") ||
			(componentNumberStr.starts_with(L"*********") && (lastComponentNumber != 0))) {
			componentsCountPerList->push_back(lastComponentNumber); 
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			getNextSubString(L"КОЛ-ВО");
		}
		return false;
	}
	else 
	{
		lastComponentNumber++;
		return true;
	}
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
		Case1, Case2
	};
	Case parsCase;
	std::wstring designTemperatureStr = getNextSubString(L"РАСЧЕТ. ТЕМП.");\
	if (designTemperatureStr == L"") {
		designTemperatureStr = getNextSubString(L"РАСЧЕТ. ТЕМП.", true);
	}
	if (designTemperatureStr != L"SHEET" && designTemperatureStr != L"OPER. PRESSURE (MPa)") {
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

	std::wregex cipherDocumentPattern(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	std::wsmatch match;
	if (std::regex_search(*text, match, cipherDocumentPattern)) {
		(*columns)[L"Шифр документа"].emplace_back(match.str());
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

	std::wstring pipelineClassStr(getPreviouslySubString());
	if (pipelineClassStr.starts_with(L"PIPE")) {
		pipelineClassStr = L"-";
	}
	(*columns)[L"Класс трубопровода"].emplace_back(pipelineClassStr);

	(*columns)[L"Диаметр трубопровода"].emplace_back(getPreviouslySubString(L"PIPE CLASS",
		true));

	std::wstring categoryPipelinesGostStr(getPreviouslySubString(L"PIPE DIAM", true));
	if (categoryPipelinesGostStr.starts_with(L"КАТЕГОРИЯ")) {
		categoryPipelinesGostStr = L"-";
	}
	(*columns)[L"Категория трубопр. Гост"].emplace_back(categoryPipelinesGostStr);

	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getPreviouslySubString(
		L"PIPE CATEGORY", true));

	std::wstring techEnvStr(getPreviouslySubString(L"PIPE CATEGORY SN", true));
	if (techEnvStr.starts_with(L"ТЕХНОЛОГИЧЕСКАЯ")) {
		techEnvStr = L"-";
	}
	(*columns)[L"Технологическая среда"].emplace_back(techEnvStr);

	(*columns)[L"Изоляция"].emplace_back(getPreviouslySubString(L"FLUID SERVICE", true));
	(*columns)[L"Спутниковый обогрев"].emplace_back(getPreviouslySubString(L"INSULATION",
		true));

	std::wstring paintingSystemStr(getPreviouslySubString(L"TRACING", true));
	if (paintingSystemStr.starts_with(L"СИСТЕМА")) {
		paintingSystemStr = L"-";
	}
	(*columns)[L"Система покраски"].emplace_back(paintingSystemStr);

	(*columns)[L"Расчет напряжений"].emplace_back(getPreviouslySubString(L"PAINTING SYSTEM",
		true));

	std::wstring postWeldingHeatTreatmentStr(getPreviouslySubString(L"CRITICAL LINE", true));
	if (postWeldingHeatTreatmentStr.starts_with(L"ПОСЛЕСВАР")) {
		postWeldingHeatTreatmentStr = L"-";
	}
	(*columns)[L"Послесвар. Термообраб"].emplace_back(postWeldingHeatTreatmentStr);

	(*columns)[L"Контроль сварных швов"].emplace_back(getPreviouslySubString(L"POST WELD",
		true));
	(*columns)[L"Среда испытаний"].emplace_back(getPreviouslySubString(L"WELD INSPECTION",
		true));
	(*columns)[L"Давление испыт"].emplace_back(getPreviouslySubString(L"TEST MEDIUM", true));

	std::wstring schemeNumber = getPreviouslySubString(L"For general", true);
	bool schemeNumberNotFound = schemeNumber.ends_with(L"Номер схемы");
	if (!schemeNumberNotFound) {
		(*columns)[L"Номер схемы"].emplace_back(schemeNumber);
	}

	std::wstring listCountStr = getPreviouslySubString(L"LEGEND", true);
	std::wstring currentListStr = getPreviouslySubString();
	std::wregex listsPattern(LR"(\d{1,2})");
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
}

void TextParserASP::parseCase2()
{
	(*columns)[L"Расчет. Давление"].emplace_back(getPreviouslySubString(L"Pipe surface", true));
	(*columns)[L"Расчет. Темп"].emplace_back(getPreviouslySubString());
	(*columns)[L"Рабочее давление"].emplace_back(getPreviouslySubString());
	(*columns)[L"Рабочая температура"].emplace_back(getPreviouslySubString());
	(*columns)[L"Номер схемы"].emplace_back(getPreviouslySubString());
	(*columns)[L"Номер линии"].emplace_back(getPreviouslySubString());
	(*columns)[L"Давление испыт"].emplace_back(getPreviouslySubString());
	(*columns)[L"Среда испытаний"].emplace_back(getPreviouslySubString());
	(*columns)[L"Контроль сварных швов"].emplace_back(getPreviouslySubString());
	(*columns)[L"Послесвар. Термообраб"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(3, true);

	std::wstring columnsNames[] = { L"Расчет напряжений", L"Система покраски",
		L"Спутниковый обогрев", L"Изоляция" };
	std::vector<std::wstring> columnsValues;
	std::wregex generalNotes(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-\w+-\d+)");
	size_t columnCount = 0;
	do
	{
		columnsValues.emplace_back(getPreviouslySubString());
		columnCount++;
	} while (!std::regex_match(columnsValues[columnCount - 1], generalNotes));
	for (size_t i = 0; i < 4; i++)
	{
		if (columnCount < 4 && i == 1)
		{
			(*columns)[columnsNames[i]].emplace_back(L"-");
			continue;
		}
		(*columns)[columnsNames[i]].emplace_back(columnsValues[i]);
	}

	(*columns)[L"Технологическая среда"].emplace_back(getPreviouslySubString());
	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getPreviouslySubString());
	(*columns)[L"Категория трубопр. Гост"].emplace_back(getPreviouslySubString());
	(*columns)[L"Диаметр трубопровода"].emplace_back(getPreviouslySubString());
	(*columns)[L"Класс трубопровода"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(2, true);
	(*columns)[L"Шифр документа"].emplace_back(getPreviouslySubString());
	(*columns)[L"Изометрический чертеж"].emplace_back(getSubString(L"Изометрический чертеж",
		true).erase(0, 22));
	moveOnCountSubStr(2, true);
	(*columns)[L"Листов"].emplace_back(getPreviouslySubString());
	(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
}

TextParserASP::TextParserASP(const std::wstring& text, Columns& columns,
	std::vector<int>& componentsCountPerList, wchar_t separator) :
	BaseTextParser(text, columns, componentsCountPerList, separator)
{

}

void TextParserASP::parse(const std::wstring& fileName)
{
	reset();

	std::wstring transferToPagePattern(L"*********");
	if (text->substr(1, transferToPagePattern.size()) == transferToPagePattern) {
		moveOnCountSubStr(3);
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

	if (*(componentsCountPerList->end() - 1) != 0) 
	{
		readTablePartData();

		(*columns)[L"Имя файла"].emplace_back(fileName);
	} 
	else {
		componentsCountPerList->erase(componentsCountPerList->end() - 1);
	}
}
