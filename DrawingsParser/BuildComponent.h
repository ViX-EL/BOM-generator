#pragma once

#ifndef BuildComponent_h__
#define BuildComponent_h__

#include "SplitBuildComponentData.h"
#include "ValuesCheker.h"

#include <string>
#include <regex>
#include <memory>
#include <string_view>

class BuildComponent
{
protected:
	int positionNumber = 0;
	std::wstring description { L"-" };
	std::wstring nominalDiameter { L"-" };
	std::wstring document { L"-" };
	std::wstring amount { L"-" };
	std::wstring positionCode { L"-" };

	std::shared_ptr<SplitBuildComponentData> splitData;

	std::wregex descriptionPattern;
	std::wregex nominalDiameterPattern;
	std::wregex documentPattern;
	std::wregex amountPattern;
	std::wregex positionCodePattern;
	static inline std::wregex positionNumberPattern{ LR"( *[1-9]\d*)" };

	explicit BuildComponent(const std::wstring& positionNumberStr);
	explicit BuildComponent(int positionNumber);

public:
	bool trySetDescription(const std::wstring& descriptionStr, ValuesCheker::Type checkType = ValuesCheker::getType());
	bool trySetNominalDiameter(const std::wstring& nominalDiameterStr, ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetAmount(const std::wstring& amountStr, ValuesCheker::Type checkType = ValuesCheker::getType());
	bool trySetDocument(const std::wstring& documentStr, ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetPositionCode(const std::wstring& positionCodeStr, ValuesCheker::Type checkType = ValuesCheker::getType());

	int getPositionNumber() const;
	std::wstring_view getDescription() const;
	std::wstring_view getNominalDiameter() const;
	std::wstring_view getDocument() const;
	std::wstring_view getAmount() const;
	std::wstring_view getPositionCode() const;

	static const std::wregex& getPositionNumberPattern();
	const std::wregex& getDescriptionPattern() { return descriptionPattern; }
	const std::wregex& getNominalDiameterPattern() { return nominalDiameterPattern; }
	const std::wregex& getDocumentPattern() { return documentPattern; }
	const std::wregex& getAmountPattern() { return amountPattern; }
	const std::wregex& getPositionCodePattern() { return positionCodePattern; }

	void parseSplitData();
	void createEmptySplitData();
	std::shared_ptr<SplitBuildComponentData> getSplitData();
};

#endif // BuildComponent_h__

