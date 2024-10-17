#include "TextParserLGN.h"

#include "StringConvert.h"

#include <regex>

bool TextParserLGN::readComponent()
{
	if (!readLastComponentNumber()) {
		return false;
	}

	std::wstring descriptionStr(getNextSubString());
	std::wstring diameterStr(getNextSubString());
	while (!readDescriptionAndDiameter(descriptionStr, diameterStr))
	{
		descriptionStr += diameterStr;
		diameterStr = getNextSubString();
	}

	readDocument();
	std::wstring countStr(getNextSubString());
	(*columns)[L"Кол-во"].emplace_back(countStr);
	(*columns)[L"Код позиции"].emplace_back(L"-");
	return true;
}

bool TextParserLGN::readLastComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	size_t position = currentPositionInText;
	std::wstring thisDocumentStr;
	//Если не найден номер компонента
	if (componentNumberStr.size() < 3 || !isdigit(componentNumberStr.at(2))) { ///TODO Исправить на регулярку?
		for (size_t i = 0; i < 2; i++) {
			thisDocumentStr = getNextSubString(position);
		}
		if (thisDocumentStr.starts_with(L"This document is") ||
			componentNumberStr.starts_with(L"This document is") ||
			componentNumberStr.starts_with(L"CUT PIPE LENGTH") ||
			componentNumberStr.starts_with(L"*******")) {
			if (componentNumberStr.starts_with(L"*******")) {
				lineBreak = true;
			}
			componentsCountPerList->push_back(lastComponentNumber);
			lastComponentNumber = 0;
		}
		return false;
	}
	else {
		thisDocumentStr = getNextSubString(position);
		if (thisDocumentStr.starts_with(L"This document is")) { /// TODO Это точно нужно? проверить
			componentsCountPerList->push_back(lastComponentNumber);
			lastComponentNumber = 0;
			return false;
		}
		//lastComponentNumber = std::stoi(componentNumberStr);
		lastComponentNumber++;
		return true;
	}
}

bool TextParserLGN::readDescriptionAndDiameter(const std::wstring&
	descriptionStr, std::wstring& diameterStr)
{
	std::wregex diameterPattern(LR"(\d{1,4}x |\d{1,4} x|\d{1,4}x\d{1,4}|\d{1,4})");
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
		else if (diameterStr.ends_with(L"x")) {
			diameterStr += getNextSubString();
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

	std::wregex countPattern(LR"( ? {1,2}\d{1,3}M?| ? {0,2}\d{1,3}\.\d{1,4}M)");
	std::wregex componentNumberPattern(LR"( {1,2}\d{1,3})");
	std::wregex categoryPattern(LR"([A-Z ]+\/[А-Я ]+)");
	std::wregex documentPattern(LR"([0-9a-zA-Z"\-\/ .;,()А-Яa-я]+)");

	size_t position = currentPositionInText;
	for (int i{ 0 }; i < 3; i++) {
		subStringsBuffer.emplace_back(getNextSubString(position));
	}

	if (subStringsBuffer[0] == L"–" || std::regex_match(subStringsBuffer[0], countPattern) || subStringsBuffer[0] == L"None") 
	{
		(*columns)[L"Документ"].emplace_back(L"–");
		//moveToNextSubString();
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
			//documentSubStrCount -= 2;
			matches[0] = true; matches[1] = true; matches[2] = true;
		}
		else
		{
			matches[0] = std::regex_match(*(subStringsBuffer.end() - 3), documentPattern);
			matches[1] = std::regex_match(*(subStringsBuffer.end() - 2), countPattern);
			matches[2] = std::regex_match(*(subStringsBuffer.end() - 1), componentNumberPattern);
			if (!matches[2]) {
				matches[2] = matches[2] || (subStringsBuffer.end() - 1)->starts_with(L"CUT PIPE LENGTH");
			}
			if (!matches[2]) {
				matches[2] = matches[2] || std::regex_match(*(subStringsBuffer.end() - 1), categoryPattern);
			}
			if (!matches[2]) {
				matches[2] = matches[2] || (subStringsBuffer.end() - 1)->starts_with(L"*******");
			}
		}
		if (matches[0] && matches[1] && matches[2])
		{
			std::wstring documentSubStr;
			for (size_t i = 0; i < documentSubStrCount; i++) {
				documentSubStr = documentSubStr + subStringsBuffer[i];
				moveToNextSubString();
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

bool TextParserLGN::findCountStr()
{
	std::wstring findedCountStr;
	if (lineBreak)
	{
		for (int i = 0; i < 2; i++) {
			findedCountStr = getNextSubString(L"*******");
		}
		std::wregex componentNumberPattern(LR"( *\d+)");
		lineBreak = false;
		if (std::regex_match(findedCountStr, componentNumberPattern)) {
			moveToPreviouslySubString();
		}
		else {
			findedCountStr = getNextSubString(L"КОЛ-ВО");
		}
	}
	else {
		findedCountStr = getNextSubString(L"КОЛ-ВО");
	}

	if (findedCountStr.empty()) { // Если не найдено
		return false;
	}

	return true;
}

bool TextParserLGN::readList()
{
	if (!findCountStr()) {
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
		std::wstring thisDocumentStr = getSubString(L"Настоящий документ");
		moveOnCountSubStr(2, true);
		std::wstring listsStr = getPreviouslySubString();
		std::wstring listStr = getPreviouslySubString();
		(*columns)[L"Листов"].emplace_back(listsStr);
		(*columns)[L"Лист"].emplace_back(listStr);
	}

	moveToSubString(L"НА ПЛОЩАДКЕ");
	std::wstring currentSubStr;
	std::wregex moveToPattern(LR"(GCC-LGN-DDD-\d+-\d+-\d+-\w+-\w+-\d+)");
	while (!std::regex_match(currentSubStr, moveToPattern)) {
		currentSubStr = getNextSubString();
	}
	//moveOnCountSubStr(2);
	(*columns)[L"Расчет. Давление"].emplace_back(getNextSubString());
	(*columns)[L"Расчет. Темп"].emplace_back(getNextSubString());
	(*columns)[L"Категория трубопр. Гост"].emplace_back(getNextSubString());
	(*columns)[L"Давление испыт"].emplace_back(getNextSubString());

	std::wstring weldInspectionStr(getNextSubString());
	if (weldInspectionStr.ends_with(L"%037")) {
		weldInspectionStr.erase(weldInspectionStr.size() - 4);
	}
	(*columns)[L"Контроль сварных швов"].emplace_back(weldInspectionStr);

	(*columns)[L"Послесвар. Термообраб"].emplace_back(getNextSubString());
	(*columns)[L"Система покраски"].emplace_back(getNextSubString());

	std::wstring testEnvironmentStr = getNextSubString();
	if (testEnvironmentStr == L"Воздух/Азот") {
		(*columns)[L"Среда испытаний"].emplace_back(testEnvironmentStr + L' ' + getNextSubString());
	}
	else {
		(*columns)[L"Среда испытаний"].emplace_back(testEnvironmentStr);
	}

	(*columns)[L"Технологическая среда"].emplace_back(getNextSubString());
	(*columns)[L"Класс трубопровода"].emplace_back(getNextSubString());
	moveOnCountSubStr(2);
	(*columns)[L"Спутниковый обогрев"].emplace_back(getNextSubString());
	(*columns)[L"Рабочее давление"].emplace_back(getNextSubString());
	(*columns)[L"Рабочая температура"].emplace_back(getNextSubString());

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

	(*columns)[L"Изометрический чертеж"].emplace_back(getNextSubString(L"Изометрический чертеж"));
	moveToPreviouslySubString();
	(*columns)[L"Расчет напряжений"].emplace_back(getPreviouslySubString());
	(*columns)[L"Номер линии"].emplace_back(getPreviouslySubString());

	std::wstring schemeNumber(getPreviouslySubString());
	if (!schemeNumber.starts_with(L"* (See note /")) {
		(*columns)[L"Номер схемы"].emplace_back(schemeNumber);
	}

	(*columns)[L"Имя файла"].emplace_back(getSubString(L"Имя файла"));
	if (schemeNumber.starts_with(L"* (See note /")) {
		moveOnCountSubStr(3);
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