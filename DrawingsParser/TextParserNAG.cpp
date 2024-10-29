#include "TextParserNAG.h"

#include <regex>
#include <initializer_list>
#include <wx/log.h>

bool TextParserNAG::readLastComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	//Если не найден номер компонента
	std::wregex componentNumberPattern(LR"( *\d+)");
	std::wregex lineNumberPattern(LR"(\d+-\d+-\d+\/\d+-\w+-\d+-[0-9A-Z]+-\d+)");
	bool isLineNumber = std::regex_match(componentNumberStr, lineNumberPattern);
	if (!std::regex_match(componentNumberStr, componentNumberPattern) || isLineNumber)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") ||
			componentNumberStr.starts_with(L"UT PIPE LENGTH")) {
			if (lastComponentNumber != 0) {
				componentsCountPerList->push_back(lastComponentNumber);
				std::wstring formatStr = getPreviouslySubString(L"LEGEND");
				if (formatStr.starts_with(L"Формат")) {
					moveOnCountSubStr(2, true);
				}
				else {
					moveToPreviouslySubString();
				}
				std::wregex listsPattern(LR"(\d+)");
				std::wstring listsCountStr(getPreviouslySubString());
				if (std::regex_match(listsCountStr, listsPattern))
				{
					(*columns)[L"Листов"].emplace_back(listsCountStr);
					(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
					(*columns)[L"Номер линии"].emplace_back(getPreviouslySubString());
					moveOnCountSubStr(2);
				}
				else {
					(*columns)[L"Номер линии"].emplace_back(listsCountStr);
				}
			}
			else {
				currentListEmpty = true;
			}
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			moveToSubString(L"КОЛ-ВО");
		}
		else if (isLineNumber) {
			(*columns)[L"Номер линии"].emplace_back(componentNumberStr);
			std::wstring listStr(getNextSubString());
			std::wregex listPattern(LR"(\d+)");
			if (std::regex_match(listStr, listPattern))
			{
				(*columns)[L"Лист"].emplace_back(listStr);
				(*columns)[L"Листов"].emplace_back(getNextSubString());
			}
			componentsCountPerList->push_back(lastComponentNumber);
		}
		return false;
	}
	else
	{
		lastComponentNumber++;
		return true;
	}
}

void TextParserNAG::writeValueOfTwoSubStr(const std::wstring& columnName, std::initializer_list<std::wstring> ifEqualStrList, const std::wstring& firstStr)
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
	(*columns)[columnName].emplace_back(subStr);
}

void TextParserNAG::readTablePartData()
{
	std::wstring schemeNumber(getNextSubString(L"ЛИСТОВ"));
	std::wregex cipherOfDocumentPattern(LR"(GCC-NAG-DDD-\d+-\d+-\d+-TKM?-ISO-\d+)");
	bool cipherWasFoundEarlier = false;
	if (std::regex_match(schemeNumber, cipherOfDocumentPattern)) 
	{
		cipherWasFoundEarlier = true;
		(*columns)[L"Шифр документа"].emplace_back(schemeNumber);
		moveToNextSubString();
		(*columns)[L"Изометрический чертеж"].emplace_back(getNextSubString());
		(*columns)[L"Листов"].emplace_back(getPreviouslySubString(L"Формат", true));
		(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
		schemeNumber = getNextSubString(L"КЛАСС ТРУБОПРОВОДА", true);
	}

	std::wregex designPressurePattern(LR"(-?\d|-|\d,\d(\/\d,\d)?|\d\.\d)");
	bool isFlareFarming = false;
	if (schemeNumber.starts_with(L"GCC-NAG-DDD")) {
		(*columns)[L"Номер схемы"].emplace_back(schemeNumber);
		writeValueOfTwoSubStr(L"Расчет. Давление", { L"HYDROSTATIC/" });
	}
	else
	{
		(*columns)[L"Номер схемы"].emplace_back(L"-");
		std::wregex flareFarmingPattern(L"Факельное хозяйство");
		if (std::regex_search(*text, flareFarmingPattern) && !std::regex_match(schemeNumber, designPressurePattern)) {
			isFlareFarming = true;
		}
		else {
			writeValueOfTwoSubStr(L"Расчет. Давление", { L"HYDROSTATIC/" }, schemeNumber);
		}
	}

	if (isFlareFarming && !std::regex_match(schemeNumber, designPressurePattern))
	{
		writeValueOfTwoSubStr(L"Среда испытаний", { L"PLANT AIR /", L"FW /" }, schemeNumber);
		(*columns)[L"Послесвар. Термообраб"].emplace_back(getNextSubString());
		(*columns)[L"Расчет напряжений"].emplace_back(getNextSubString());
		(*columns)[L"Система покраски"].emplace_back(getNextSubString());
		(*columns)[L"Спутниковый обогрев"].emplace_back(getNextSubString());
		(*columns)[L"Изоляция"].emplace_back(getNextSubString());
		(*columns)[L"Технологическая среда"].emplace_back(getNextSubString());
		(*columns)[L"Класс трубопровода"].emplace_back(getNextSubString());

		(*columns)[L"Расчет. Давление"].emplace_back(L"-");
		(*columns)[L"Расчет. Темп"].emplace_back(L"-");
		(*columns)[L"Рабочее давление"].emplace_back(L"-");
		(*columns)[L"Рабочая температура"].emplace_back(L"-");
		(*columns)[L"Давление испыт"].emplace_back(L"-");
		(*columns)[L"Контроль сварных швов"].emplace_back(L"-");
		(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(L"-");
		(*columns)[L"Категория трубопр. Гост"].emplace_back(L"-");
		(*columns)[L"Диаметр трубопровода"].emplace_back(L"-");
	}
	else
	{
		(*columns)[L"Расчет. Темп"].emplace_back(getNextSubString());
		writeValueOfTwoSubStr(L"Рабочее давление", { L"HYDROSTATIC/" });
		writeValueOfTwoSubStr(L"Рабочая температура", { L"Не ниже 5 /" });
		(*columns)[L"Давление испыт"].emplace_back(getNextSubString());
		writeValueOfTwoSubStr(L"Среда испытаний", { L"PLANT AIR /", L"FW /" });
		writeValueOfTwoSubStr(L"Контроль сварных швов", { L"2 метода объем" });
		(*columns)[L"Послесвар. Термообраб"].emplace_back(getNextSubString());
		(*columns)[L"Расчет напряжений"].emplace_back(getNextSubString());
		(*columns)[L"Система покраски"].emplace_back(getNextSubString());
		(*columns)[L"Спутниковый обогрев"].emplace_back(getNextSubString());
		std::wstring isolation(getNextSubString());
		std::wregex isolationPattern(LR"(H|NO|\d+|[0-9 ]+|LI|NO ?/ ?НЕТ|\d+ \d+)");
		if (!std::regex_match(isolation, isolationPattern))
		{
			(*columns)[L"Изоляция"].emplace_back(L"-");
			(*columns)[L"Технологическая среда"].emplace_back(isolation);
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
				(*columns)[L"Изоляция"].emplace_back(isolation);
				(*columns)[L"Технологическая среда"].emplace_back(getNextSubString());
			}
			else
			{
				(*columns)[L"Изоляция"].emplace_back(L"-");
				(*columns)[L"Технологическая среда"].emplace_back(isolation);
			}
		}
		(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getNextSubString());
		(*columns)[L"Категория трубопр. Гост"].emplace_back(getNextSubString());
		(*columns)[L"Диаметр трубопровода"].emplace_back(getNextSubString());
		(*columns)[L"Класс трубопровода"].emplace_back(getNextSubString());
	}

	if (!cipherWasFoundEarlier) 
	{
		(*columns)[L"Шифр документа"].emplace_back(getNextSubString());
		(*columns)[L"Изометрический чертеж"].emplace_back(getNextSubString(L"Isometric drawing"));
	}
}

TextParserNAG::TextParserNAG(const std::wstring& text, Columns& columns,
std::vector<int>& componentsCountPerList, wchar_t separator) : BaseTextParser(text, columns,
	componentsCountPerList, separator)
{

}

void TextParserNAG::parse(const std::wstring& fileName)
{
	reset();

	if (text->find(L"FABRICATION MATERIALS", currentPositionInText) != std::wstring::npos) {
		moveToSubString(L"КОЛ-ВО");
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

	if (!currentListEmpty)
	{
		readTablePartData();

		(*columns)[L"Имя файла"].emplace_back(fileName);
	}
	else {
		currentListEmpty = false;
		wxLogMessage("[Запись] Отсутствуют записываемые листы в файле %s", fileName);
	}
}
