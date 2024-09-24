#include "TextParserNAG.h"

#include <regex>

bool TextParserNAG::readLastComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	//���� �� ������ ����� ����������
	std::wregex componentNumberPattern(LR"( *\d+)");
	std::wregex lineNumberPattern(LR"(\d+-\d+-\d+\/\d+-\w+-\d+-[0-9A-Z]+-\d+)");
	bool isLineNumber = std::regex_match(componentNumberStr, lineNumberPattern);
	if (!std::regex_match(componentNumberStr, componentNumberPattern) || isLineNumber)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") ||
			componentNumberStr.starts_with(L"UT PIPE LENGTH")) {
			if (lastComponentNumber != 0) {
				componentsCountPerList->push_back(lastComponentNumber);
				moveToPreviouslySubString(L"������");
				moveToPreviouslySubString();
				(*columns)[L"������"].emplace_back(getPreviouslySubString());
				(*columns)[L"����"].emplace_back(getPreviouslySubString());
				(*columns)[L"����� �����"].emplace_back(getPreviouslySubString());
				moveOnCountSubStr(2);
			}
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			moveToSubString(L"���-��");
		}
		else if (isLineNumber) {
			(*columns)[L"����� �����"].emplace_back(componentNumberStr);
			(*columns)[L"����"].emplace_back(getNextSubString());
			(*columns)[L"������"].emplace_back(getNextSubString());
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

void TextParserNAG::readTablePartData()
{
	(*columns)[L"����� �����"].emplace_back(getNextSubString(L"������"));
	(*columns)[L"������. ��������"].emplace_back(getNextSubString());
	(*columns)[L"������. ����"].emplace_back(getNextSubString());
	(*columns)[L"������� ��������"].emplace_back(getNextSubString());
	(*columns)[L"������� �����������"].emplace_back(getNextSubString());
	(*columns)[L"�������� �����"].emplace_back(getNextSubString());
	(*columns)[L"����� ���������"].emplace_back(getNextSubString());
	(*columns)[L"�������� ������� ����"].emplace_back(getNextSubString());
	(*columns)[L"���������. ����������"].emplace_back(getNextSubString());
	(*columns)[L"������ ����������"].emplace_back(getNextSubString());
	(*columns)[L"������� ��������"].emplace_back(getNextSubString());
	(*columns)[L"����������� �������"].emplace_back(getNextSubString());
	(*columns)[L"��������"].emplace_back(getNextSubString());
	(*columns)[L"��������������� �����"].emplace_back(getNextSubString());
	(*columns)[L"��������� �������. �� ��"].emplace_back(getNextSubString());
	(*columns)[L"��������� �������. ����"].emplace_back(getNextSubString());
	(*columns)[L"������� ������������"].emplace_back(getNextSubString());
	(*columns)[L"����� ������������"].emplace_back(getNextSubString());
	(*columns)[L"���� ���������"].emplace_back(getNextSubString());
	(*columns)[L"�������������� ������"].emplace_back(getNextSubString(L"Isometric drawing"));
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
		moveToSubString(L"���-��");
	}
	while (true) //������ ���� �����������
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

		(*columns)[L"��� �����"].emplace_back(fileName);
	}
}
