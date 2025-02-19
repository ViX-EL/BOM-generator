#include "BuildComponent.h"
#include "StringUtilities.h"

#include <string>
#include <regex>
#include <cassert>
#include <string_view>

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

bool trySetValue(const std::wstring& sourceValueStr, std::wstring& targetValue, const std::wregex& valuePattern, bool assertionCheck, const char* assertionMessage, bool inputCheckOff)
{
	if (inputCheckOff) 
	{
		targetValue = sourceValueStr;
		return true;
	}
	else
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
}

BuildComponent::BuildComponent(const std::wstring& positionNumberStr)
{
	positionNumber = std::stoi(positionNumberStr);
}

BuildComponent::BuildComponent(int positionNumber)
{
	this->positionNumber = positionNumber;
}

bool BuildComponent::trySetDescription(const std::wstring& descriptionStr, bool assertionCheck)
{
	return trySetValue(descriptionStr, description, descriptionPattern, assertionCheck, "Недопустимое значение для описания компонента!");
}

bool BuildComponent::trySetNominalDiameter(const std::wstring& nominalDiameterStr, bool assertionCheck)
{
	return trySetValue(StringUtilities::removeSpaces(nominalDiameterStr), nominalDiameter, nominalDiameterPattern, assertionCheck, 
		"Недопустимое значение для условного диаметра компонента!");
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

std::wstring_view BuildComponent::getDescription() const
{
	return std::wstring_view(description);
}

std::wstring_view BuildComponent::getNominalDiameter() const
{
	return std::wstring_view(nominalDiameter);
}

std::wstring_view BuildComponent::getDocument() const
{
	return std::wstring_view(document);
}

std::wstring_view BuildComponent::getAmount() const
{
	return std::wstring_view(amount);
}

std::wstring_view BuildComponent::getPositionCode() const
{
	return std::wstring_view(positionCode);
}

const std::wregex& BuildComponent::getPositionNumberPattern()
{
	return positionNumberPattern;
}

void BuildComponent::parseSplitData()
{
	splitData = std::make_shared<SplitBuildComponentData>(this);
}

std::shared_ptr<SplitBuildComponentData> BuildComponent::getSplitData()
{
	return splitData;
}
