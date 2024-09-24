#include "TextParserLGN.h"

#include <regex>

bool TextParserLGN::readComponent()
{
	if (!readLastComponentNumber()) {
		return false;
	}

	std::wstring descriptionStr(getNextSubString());
	std::wstring diameterStr(getNextSubString());
	if (!readDescriptionAndDiameter(descriptionStr, diameterStr))
	{
		descriptionStr += diameterStr;
		diameterStr = getNextSubString();
		if (!readDescriptionAndDiameter(descriptionStr, diameterStr)) {
			descriptionStr += diameterStr;
			diameterStr = getNextSubString();
			readDescriptionAndDiameter(descriptionStr, diameterStr);
		}
	}

	readDocument();
	(*columns)[L"Кол-во"].emplace_back(getNextSubString());
	(*columns)[L"Код позиции"].emplace_back(L"-");
	return true;
}

bool TextParserLGN::readLastComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	//Если не найден номер компонента
	if (componentNumberStr.size() < 3 || !isdigit(componentNumberStr.at(2))) {
		std::wstring thisDocumentStr;
		size_t position = currentPositionInText;
		for (size_t i = 0; i < 2; i++) {
			thisDocumentStr = getNextSubString(position);
		}
		if (thisDocumentStr.starts_with(L"This document is") ||
			componentNumberStr.starts_with(L"CUT PIPE LENGTH") ||
			componentNumberStr.starts_with(L"*********")) {
			componentsCountPerList->push_back(lastComponentNumber);
		}
		return false;
	}
	else {
		lastComponentNumber = std::stoi(componentNumberStr);
		return true;
	}
}

bool TextParserLGN::readDescriptionAndDiameter(const std::wstring&
	descriptionStr, std::wstring& diameterStr)
{
	std::wregex diameterPattern(LR"(\d{1,4}x |\d{1,4}x\d{1,4}|\d{1,4})");
	if (std::regex_match(diameterStr, diameterPattern))
	{
		(*columns)[L"Описание компонента"].emplace_back(descriptionStr);
		if (text->at(currentPositionInText + 3) == L'x')
		{
			diameterStr += getNextSubString();
			diameterStr += getNextSubString();
		}
		else if (text->at(currentPositionInText + 2) == L'x') {
			diameterStr += getNextSubString();
			if (diameterStr.ends_with(L"x")) {
				diameterStr += getNextSubString();
			}
		}

		(*columns)[L"Условный диаметр"].emplace_back(diameterStr);
		return true;
	}
	else {
		return false;
	}
}

void TextParserLGN::readDocument()
{
	std::vector<std::wstring> subStringsBuffer;

	std::wregex countPattern(LR"( {1,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
	std::wregex componentNumberPattern(LR"( {1,2}\d{1,3})");
	std::wregex categoryPattern(LR"([A-Z ]+\/[А-Я ]+)");
	std::wregex documentPattern(LR"([0-9A-Z"\-\/ .;()А-Я]+)");

	size_t position = currentPositionInText;
	for (int i{ 0 }; i < 3; i++) {
		subStringsBuffer.emplace_back(getNextSubString(position));
	}

	if (subStringsBuffer[0] == L"–" || std::regex_match(subStringsBuffer[0],
		countPattern)) {
		(*columns)[L"Документ"].emplace_back(L"–");
		getNextSubString();
		return;
	}

	// Подсчёт количества подстрок, которые составляют строку документа
	// компонента
	size_t documentSubStrCount{ 1 };
	while (true)
	{
		bool matches[3]{ false };
		if ((subStringsBuffer.end() - 1)->starts_with(L"This document is"))
		{
			documentSubStrCount -= 2;
			matches[0] = true; matches[1] = true; matches[2] = true;
		}
		else
		{
			matches[0] = std::regex_match(*(subStringsBuffer.end() - 3),
				documentPattern);
			matches[1] = std::regex_match(*(subStringsBuffer.end() - 2),
				countPattern);
			matches[2] = std::regex_match(*(subStringsBuffer.end() - 1),
				componentNumberPattern);
			if (!matches[2]) {
				matches[2] = matches[2] || (subStringsBuffer.end() - 1)
					->starts_with(L"CUT PIPE LENGTH");
			}
			if (!matches[2]) {
				matches[2] = matches[2] || std::regex_match(*(subStringsBuffer
					.end() - 1), categoryPattern);
			}
			if (!matches[2]) {
				matches[2] = matches[2] || (subStringsBuffer.end() - 1)
					->starts_with(L"*********");
			}
		}
		if (matches[0] && matches[1] && matches[2])
		{
			std::wstring documentSubStr;
			for (size_t i = 0; i < documentSubStrCount; i++) {
				documentSubStr = documentSubStr + subStringsBuffer[i];
				getNextSubString();
			}
			(*columns)[L"Документ"].emplace_back(documentSubStr);
			break;
		}
		else {
			subStringsBuffer.push_back(getNextSubString(position));
			documentSubStrCount++;
		}
	}
}

bool TextParserLGN::readList()
{
	std::wstring findedCountStr = getNextSubString(L"КОЛ-ВО");
	if (findedCountStr.empty()) { // Если не найдено
		return false;
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

	bool haveOnlyOneList = isHaveOnlyOneList();

	if (haveOnlyOneList) {
		(*columns)[L"Листов"].emplace_back(L"1");
		(*columns)[L"Лист"].emplace_back(L"1");
	}
	else {
		(*columns)[L"Листов"].emplace_back(
			getPreviouslySubString(L"This document"));
		(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
	}
	std::wstring operatingTemperatureStr = getPreviouslySubString(L"Issued for");
	std::wregex operatingTemperaturePattern(LR"(-?\d+\.?(\d+)? \/ -?\d+\.?(\d+)?)");
	while (!std::regex_match(operatingTemperatureStr,
		operatingTemperaturePattern)) {
		operatingTemperatureStr = getPreviouslySubString();
	}
	(*columns)[L"Рабочая температура"].emplace_back(operatingTemperatureStr);
	(*columns)[L"Рабочее давление"].emplace_back(getPreviouslySubString());
	(*columns)[L"Спутниковый обогрев"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(2, true);

	(*columns)[L"Класс трубопровода"].emplace_back(getPreviouslySubString());
	(*columns)[L"Технологическая среда"].emplace_back(getPreviouslySubString());

	std::wstring testEnvironmentStr = getPreviouslySubString();
	if (testEnvironmentStr == L"Air/Nitrogen") {
		(*columns)[L"Среда испытаний"].emplace_back(testEnvironmentStr + L' '
			+ getPreviouslySubString());
	}
	else {
		(*columns)[L"Среда испытаний"].emplace_back(testEnvironmentStr);
	}
	(*columns)[L"Система покраски"].emplace_back(getPreviouslySubString());
	(*columns)[L"Послесвар. Термообраб"].emplace_back(getPreviouslySubString());

	std::wstring weldInspectionStr(getPreviouslySubString());
	if (weldInspectionStr.ends_with(L"%037")) {
		weldInspectionStr.erase(weldInspectionStr.size() - 4);
	}
	(*columns)[L"Контроль сварных швов"].emplace_back(weldInspectionStr);

	(*columns)[L"Давление испыт"].emplace_back(getPreviouslySubString());
	(*columns)[L"Категория трубопр. Гост"].emplace_back(getPreviouslySubString());
	(*columns)[L"Расчет. Темп"].emplace_back(getPreviouslySubString());
	(*columns)[L"Расчет. Давление"].emplace_back(getPreviouslySubString());

	std::wstring cipherDocumentStr(getPreviouslySubString(
		L"Газохимический комплекс"));
	std::wregex cipherDocumentPattern(
		LR"(GCC-\w{3}-DDD-\d{5}-\d{2}-\d{4}-\w{2,3}(\d{1,2})?-\w{3}-\d{5})");
	if (std::regex_match(cipherDocumentStr, cipherDocumentPattern)) {
		(*columns)[L"Шифр документа"].emplace_back(getPreviouslySubString(
			L"Газохимический комплекс"));
	}
	else {
		moveToNextSubString(L"Газохимический комплекс");
		moveOnCountSubStr(2);
		(*columns)[L"Шифр документа"].emplace_back(getNextSubString());
	}

	(*columns)[L"Диаметр трубопровода"].emplace_back(getSubString(L"DN"));
	(*columns)[L"Изоляция"].emplace_back(getNextSubString());
	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getNextSubString());
	moveToNextSubString();

	std::wstring schemeNumber(getNextSubString());
	if (!schemeNumber.starts_with(L"* (See note /")) {
		(*columns)[L"Номер схемы"].emplace_back(schemeNumber);
	}

	(*columns)[L"Номер линии"].emplace_back(getNextSubString());
	(*columns)[L"Расчет напряжений"].emplace_back(getNextSubString());
	moveToNextSubString();
	(*columns)[L"Изометрический чертеж"].emplace_back(getNextSubString());
	(*columns)[L"Имя файла"].emplace_back(getNextSubString());
	if (schemeNumber.starts_with(L"* (See note /")) {
		moveToNextSubString(L"Note / Примечание");
		schemeNumber = getNextSubString();
		schemeNumber.erase(0, 3);
		(*columns)[L"Номер схемы"].emplace_back(schemeNumber);
	}
	return true;
}

bool TextParserLGN::isHaveOnlyOneList()
{
	size_t currPosition = 0;

	for (size_t i = 0; i < 2; i++)
	{
		currPosition = text->find(L"This document", currPosition) + 1;
	}
	if (currPosition - 1 == std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}

TextParserLGN::TextParserLGN(const std::wstring& text, Columns& columns,
	std::vector<int>& componentsCountPerList, wchar_t separator) : 
	BaseTextParser(text, columns, componentsCountPerList, separator)
{

}

void TextParserLGN::parse(const std::wstring& fileName)
{
	reset();

	bool success{ false };
	do {
		success = readList();
	} while (success);
}