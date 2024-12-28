#pragma once

#include <string>
#include <regex>

bool trySetValue(const std::wstring& sourceValueStr, int& targetValue, const std::wregex& valuePattern, bool assertionCheck = true, const char* assertionMessage = "");
bool trySetValue(const std::wstring& sourceValueStr, std::wstring& targetValue, const std::wregex& valuePattern, bool assertionCheck = true, const char* assertionMessage = "", 
	bool inputCheckOff = false);

class BuildComponent
{
protected:
	int positionNumber = 0;
	std::wstring description;
	std::wstring nominalDiameter;
	std::wstring document;
	std::wstring amount;
	std::wstring positionCode;

	std::wregex descriptionPattern;
	std::wregex nominalDiameterPattern;
	std::wregex documentPattern;
	std::wregex amountPattern;
	std::wregex positionCodePattern;
	static inline std::wregex positionNumberPattern{ LR"( *[1-9]\d*)" };

	explicit BuildComponent(const std::wstring& positionNumberStr);
	explicit BuildComponent(int positionNumber);

public:
	bool trySetDescription(const std::wstring& descriptionStr, bool assertionCheck = true);
	bool trySetNominalDiameter(const std::wstring& nominalDiameterStr, bool assertionCheck = true);
	virtual bool trySetAmount(const std::wstring& amountStr, bool assertionCheck = true);
	bool trySetDocument(const std::wstring& documentStr, bool assertionCheck = true);
	virtual bool trySetPositionCode(const std::wstring& positionCodeStr, bool assertionCheck = true);

	int getPositionNumber() const;
	std::wstring getDescription() const;
	std::wstring getNominalDiameter() const;
	std::wstring getDocument() const;
	std::wstring getAmount() const;
	std::wstring getPositionCode() const;

	static const std::wregex& getPositionNumberPattern();
	const std::wregex& getDescriptionPattern() { return descriptionPattern; }
	const std::wregex& getNominalDiameterPattern() { return nominalDiameterPattern; }
	const std::wregex& getDocumentPattern() { return documentPattern; }
	const std::wregex& getAmountPattern() { return amountPattern; }
	const std::wregex& getPositionCodePattern() { return positionCodePattern; }
};

