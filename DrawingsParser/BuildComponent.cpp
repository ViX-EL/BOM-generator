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

BuildComponent::BuildComponent(const std::wstring& positionNumberStr)
{
	positionNumber = std::stoi(positionNumberStr);
}

bool BuildComponent::trySetDescription(const std::wstring& descriptionStr, bool assertionCheck)
{
	return trySetValue(descriptionStr, description, descriptionPattern, assertionCheck, "Недопустимое значение для описания компонента!");
}

bool BuildComponent::trySetNominalDiameter(const std::wstring& nominalDiameterStr, bool assertionCheck)
{
	return trySetValue(nominalDiameterStr, nominalDiameter, nominalDiameterPattern, assertionCheck, "Недопустимое значение для условного диаметра компонента!");
}

bool BuildComponent::trySetAmount(const std::wstring& amountStr, bool assertionCheck)
{
	return trySetValue(amountStr, amount, amountPattern, assertionCheck, "Недопустимое значение для количества компонента!");
}

bool BuildComponent::trySetPositionCode(const std::wstring& positionCodeStr, bool assertionCheck)
{
	return trySetValue(positionCodeStr, positionCode, positionCodePattern, assertionCheck, "Недопустимое значение для кода позиции компонента!");
}

bool BuildComponent::trySetDocument(const std::wstring& documentStr, bool assertionCheck)
{
	return trySetValue(documentStr, document, documentPattern, assertionCheck, "Недопустимое значение для документа компонента!");
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

const std::wregex& BuildComponent::getPositionNumberPattern()
{
	return positionNumberPattern;
}
