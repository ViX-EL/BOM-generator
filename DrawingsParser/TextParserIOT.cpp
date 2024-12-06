#include "TextParserIOT.h"
#include "BaseTextParser.h"

#include <regex>
#include <string>

TextParserIOT::TextParserIOT(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList, wchar_t separator) : 
	BaseTextParser(text, columns, componentsCountPerList, separator)
{

}

bool TextParserIOT::readComponent()
{
	if (!readLastComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 4; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	std::wregex diameterPattern(LR"(\d{1,4} x \d{1,4}|\d{1,4})");
	std::wregex itemCodePattern(LR"([a-zA-Z0-9()-]+)");
	std::wregex countPattern(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
	bool match{ false };

	int descriptionSubStrCount{ 1 };
	while (!match)
	{
		match = std::regex_match(*(subStrBuffer.end() - 3), diameterPattern);
		match = match && std::regex_match(*(subStrBuffer.end() - 2), itemCodePattern);
		match = match && std::regex_match(*(subStrBuffer.end() - 1), countPattern);

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
	(*columns)[L"�������� ����������"].emplace_back(descriptionStr);

	(*columns)[L"�������� �������"].emplace_back(*(subStrBuffer.end() - 3));
	(*columns)[L"��� �������"].emplace_back(*(subStrBuffer.end() - 2));
	(*columns)[L"���-��"].emplace_back(*(subStrBuffer.end() - 1));

	(*columns)[L"��������"].emplace_back(L"-");
	return true;
}

bool TextParserIOT::readLastComponentNumber()
{
	if (componentsEnded) {
		return false;
	}

	std::wstring componentNumberStr(getNextSubString());
	//���� �� ������ ����� ����������
	std::wregex componentNumberPattern(LR"( *\d+)");
	if (!std::regex_match(componentNumberStr, componentNumberPattern)) {
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") || componentNumberStr.starts_with(L"GCC-IOT-DDD")) {
			componentsCountPerList->push_back(lastComponentNumber);
			componentsEnded = true;
		}
		return false;
	}
	else
	{
		lastComponentNumber++;
		return true;
	}
}

void TextParserIOT::readTablePartData()
{
	(*columns)[L"����� ������������"].emplace_back(getPreviouslySubString(L"TOTAL FOR"));
	(*columns)[L"������� ������������"].emplace_back(getPreviouslySubString());
	(*columns)[L"��������"].emplace_back(getPreviouslySubString());
	(*columns)[L"����������� �������"].emplace_back(getPreviouslySubString());
	(*columns)[L"������� �����������"].emplace_back(getPreviouslySubString());
	(*columns)[L"������. ����"].emplace_back(getPreviouslySubString());
	(*columns)[L"������� ��������"].emplace_back(getPreviouslySubString());
	(*columns)[L"�������� �����"].emplace_back(getPreviouslySubString());
	(*columns)[L"������. ��������"].emplace_back(getPreviouslySubString());
	moveToPreviouslySubString();
	(*columns)[L"����� �����"].emplace_back(getPreviouslySubString());
	(*columns)[L"�������������� ������"].emplace_back(getSubString(L"Isometric drawing", true).erase(0, 18));
	moveToPreviouslySubString();
	(*columns)[L"��� �����"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(8, true);
	(*columns)[L"����� �����"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(3, true);
	(*columns)[L"������"].emplace_back(getPreviouslySubString());
	(*columns)[L"����"].emplace_back(getPreviouslySubString());
	moveToPreviouslySubString();
	(*columns)[L"���� ���������"].emplace_back(getPreviouslySubString());
	(*columns)[L"������� ��������"].emplace_back(getNextSubString(L"����������"));
	(*columns)[L"����� ���������"].emplace_back(getNextSubString());
	(*columns)[L"�������� ������� ����"].emplace_back(getNextSubString());
	(*columns)[L"���������. ����������"].emplace_back(getNextSubString());
	(*columns)[L"������ ����������"].emplace_back(getNextSubString());
	(*columns)[L"��������������� �����"].emplace_back(getNextSubString());
	(*columns)[L"��������� �������. �� ��"].emplace_back(getNextSubString());
	(*columns)[L"��������� �������. ����"].emplace_back(getNextSubString());
}

void TextParserIOT::parse(const std::wstring& fileName)
{
	reset();

	moveToNextSubString(L"���-��");

	while (true) //������ ���� �����������
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