#include "BuildComponent.h"
#include "StringUtilities.h"

#include <string>
#include <regex>
#include <cassert>
#include <string_view>

BuildComponent::BuildComponent(const std::wstring& positionNumberStr)
{
	positionNumber = std::stoi(positionNumberStr);
}

BuildComponent::BuildComponent(int positionNumber)
{
	this->positionNumber = positionNumber;
}

bool BuildComponent::trySetDescription(const std::wstring& descriptionStr, ValuesCheker::Type checkType)
{
	return ValuesCheker::ValuesCheker::trySetValue(descriptionStr, description, descriptionPattern, false, checkType, "Недопустимое значение для описания компонента!");
}

bool BuildComponent::trySetNominalDiameter(const std::wstring& nominalDiameterStr, ValuesCheker::Type checkType)
{
	std::wstring newNominalDiameterStr = nominalDiameterStr;
	if (*(nominalDiameterStr.end() - 1) != L'N') {
		newNominalDiameterStr = StringUtilities::removeSpaces(nominalDiameterStr);
	}
	return ValuesCheker::trySetValue(newNominalDiameterStr, nominalDiameter, nominalDiameterPattern, false, checkType, "Недопустимое значение для условного диаметра компонента!");
}

bool BuildComponent::trySetAmount(const std::wstring& amountStr, ValuesCheker::Type checkType)
{
	return ValuesCheker::trySetValue(amountStr, amount, amountPattern, false, checkType, "Недопустимое значение для количества компонента!");
}

bool BuildComponent::trySetPositionCode(const std::wstring& positionCodeStr, ValuesCheker::Type checkType)
{
	return ValuesCheker::trySetValue(positionCodeStr, positionCode, positionCodePattern, false, checkType, "Недопустимое значение для кода позиции компонента!");
}

bool BuildComponent::trySetDocument(const std::wstring& documentStr, ValuesCheker::Type checkType)
{
	return ValuesCheker::trySetValue(documentStr, document, documentPattern, false, checkType, "Недопустимое значение для документа компонента!");
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
	createEmptySplitData();
	splitData->parse();
}

void BuildComponent::createEmptySplitData()
{
	if (splitData == nullptr) {
		splitData = std::make_shared<SplitBuildComponentData>(this);
	}
}

std::shared_ptr<SplitBuildComponentData> BuildComponent::getSplitData()
{
	if (splitData != nullptr) {
		return splitData;
	}
	else {
		throw std::exception("Разбитых данных компонента не существует!");
	}
}
