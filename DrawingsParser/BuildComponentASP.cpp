#include "BuildComponentASP.h"

#include <regex>
#include <string>

bool BuildComponentASP::trySetDescription(const std::wstring& descriptionStr, bool assertionCheck)
{
    return trySetValue(descriptionStr, description, std::wregex(LR"([A-Za-z�-��-�0-9 -./"=#]+)"),assertionCheck, "������������ �������� ��� �������� ����������!");
}

bool BuildComponentASP::trySetNominalDiameter(const std::wstring& nominalDiameterStr, bool assertionCheck)
{
    return trySetValue(nominalDiameterStr, nominalDiameter, std::wregex(LR"(\d{1,4} x( \d+)?|\d{1,4})"), assertionCheck, 
        "������������ �������� ��� ��������� �������� ����������!");
}

bool BuildComponentASP::trySetAmount(const std::wstring& amountStr, bool assertionCheck)
{
    return trySetValue(amountStr, amount, std::wregex(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)"), assertionCheck,
        "������������ �������� ��� ���������� ����������!");
}

bool BuildComponentASP::trySetPositionCode(const std::wstring& positionCodeStr, bool assertionCheck)
{
	return trySetValue(positionCodeStr, positionCode, std::wregex(LR"([a-zA-Z0-9()_]+)"), assertionCheck,
		"������������ �������� ��� ���� ������� ����������!");
}
