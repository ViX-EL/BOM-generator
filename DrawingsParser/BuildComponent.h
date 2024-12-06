#pragma once

#include <string>
#include <regex>

bool trySetValue(const std::wstring& sourceValueStr, int& targetValue, const std::wregex& valuePattern, bool assertionCheck = true, const char* assertionMessage = "");
bool trySetValue(const std::wstring& sourceValueStr, std::wstring& targetValue, const std::wregex& valuePattern, bool assertionCheck = true,
	const char* assertionMessage = "");

class BuildComponent
{
protected:
	int positionNumber = 0;
	std::wstring description;
	std::wstring nominalDiameter;
	std::wstring document;
	std::wstring amount;
	std::wstring positionCode;

public:
	bool trySetPositionNumber(const std::wstring& positionNumberStr, bool assertionCheck = true);
	virtual bool trySetDescription(const std::wstring& descriptionStr, bool assertionCheck = true) = 0;
	virtual bool trySetNominalDiameter(const std::wstring& nominalDiameterStr, bool assertionCheck = true) = 0;
	virtual bool trySetAmount(const std::wstring& amountStr, bool assertionCheck = true) = 0;

	int getPositionNumber() const;
	std::wstring getDescription() const;
	std::wstring getNominalDiameter() const;
	std::wstring getDocument() const;
	std::wstring getAmount() const;
	std::wstring getPositionCode() const;
};

