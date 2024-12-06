#include "BuildComponent.h"

#include <string>
#include <regex>
#include <cassert>

bool trySetValue(const std::wstring& sourceValueStr, int& targetValue, const std::wregex& valuePattern, bool assertionCheck, const char* assertionMessage)
{
	bool isMatch = std::regex_match(sourceValueStr, valuePattern);
	if (assertionCheck) {
		assert(isMatch && assertionMessage);
	}
	if (isMatch)
	{
		targetValue = stoi(sourceValueStr);
		return true;
	}
	return false;
}

bool trySetValue(const std::wstring& sourceValueStr, std::wstring& targetValue, const std::wregex& valuePattern, bool assertionCheck, const char* assertionMessage)
{
	bool isMatch = std::regex_match(sourceValueStr, valuePattern);
	if (assertionCheck) {
		assert(isMatch && assertionMessage);
	}
	if (isMatch)
	{
		targetValue = sourceValueStr;
		return true;
	}
	return false;
}

bool BuildComponent::trySetPositionNumber(const std::wstring& positionNumberStr, bool assertionCheck = true)
{
	return trySetValue(positionNumberStr, positionNumber, std::wregex(LR"( *\d{1,3})"), assertionCheck, 
		"Значение для номера позиции должно быть числом не больше трёх символов!");
}

int BuildComponent::getPositionNumber() const
{
	return positionNumber;
}

std::wstring BuildComponent::getDescription() const
{
	return description != L"" ? description : L"-";
}

std::wstring BuildComponent::getNominalDiameter() const
{
	return nominalDiameter != L"" ? nominalDiameter : L"-";
}

std::wstring BuildComponent::getDocument() const
{
	return document != L"" ? document : L"-";
}

std::wstring BuildComponent::getAmount() const
{
	return amount != L"" ? amount : L"-";
}

std::wstring BuildComponent::getPositionCode() const
{
	return positionCode != L"" ? positionCode : L"-";
}
