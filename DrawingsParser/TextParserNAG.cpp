#include "TextParserNAG.h"

#include <regex>

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

void TextParserNAG::writeValueOfTwoSubStr(const std::wstring& columnName, const std::wstring& ifEqualStr)
{
	std::wstring subStr(getNextSubString());
	if (subStr == ifEqualStr) {
		subStr = subStr + getNextSubString();
	}
	(*columns)[columnName].emplace_back(subStr);
}

void TextParserNAG::readTablePartData()
{
	std::wstring schemeNumber(getNextSubString(L"ЛИСТОВ"));
	std::wregex cipherOfDocumentPattern(LR"(GCC-NAG-DDD-\d+-\d+-\d+-TKM-ISO-\d+)");
	bool cipherWasFoundEarlier = false;
	if (std::regex_match(schemeNumber, cipherOfDocumentPattern)) 
	{
		cipherWasFoundEarlier = true;
		(*columns)[L"Шифр документа"].emplace_back(schemeNumber);
		moveToNextSubString();
		(*columns)[L"Изометрический чертеж"].emplace_back(getNextSubString());
		(*columns)[L"Листов"].emplace_back(getPreviouslySubString(L"Формат", true));
		(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
		moveToSubString(L"КЛАСС ТРУБОПРОВОДА", true);
	}

	(*columns)[L"Номер схемы"].emplace_back(getNextSubString());
	(*columns)[L"Расчет. Давление"].emplace_back(getNextSubString());
	(*columns)[L"Расчет. Темп"].emplace_back(getNextSubString());
	writeValueOfTwoSubStr(L"Рабочее давление", L"HYDROSTATIC/");
	(*columns)[L"Рабочая температура"].emplace_back(getNextSubString());
	(*columns)[L"Давление испыт"].emplace_back(getNextSubString());
	writeValueOfTwoSubStr(L"Среда испытаний", L"PLANT AIR /");
	(*columns)[L"Контроль сварных швов"].emplace_back(getNextSubString());
	(*columns)[L"Послесвар. Термообраб"].emplace_back(getNextSubString());
	(*columns)[L"Расчет напряжений"].emplace_back(getNextSubString());
	(*columns)[L"Система покраски"].emplace_back(getNextSubString());
	(*columns)[L"Спутниковый обогрев"].emplace_back(getNextSubString());
	std::wstring isolation(getNextSubString());
	std::wregex isolationPattern(LR"(H|NO|\d+|[0-9 ]+|LI|NO / НЕТ)");
	if (!std::regex_match(isolation, isolationPattern)) 
	{
		(*columns)[L"Изоляция"].emplace_back(L"-");
		(*columns)[L"Технологическая среда"].emplace_back(isolation);
	}
	else {
		(*columns)[L"Изоляция"].emplace_back(isolation);
		(*columns)[L"Технологическая среда"].emplace_back(getNextSubString());
	}
	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getNextSubString());
	(*columns)[L"Категория трубопр. Гост"].emplace_back(getNextSubString());
	(*columns)[L"Диаметр трубопровода"].emplace_back(getNextSubString());
	(*columns)[L"Класс трубопровода"].emplace_back(getNextSubString());
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

	if (!componentsCountPerList->empty())
	{
		readTablePartData();

		(*columns)[L"Имя файла"].emplace_back(fileName);
	}
}
