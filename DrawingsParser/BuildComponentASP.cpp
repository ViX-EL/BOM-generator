#include "BuildComponentASP.h"
#include "StringUtilities.h"
#include "ValuesCheker.h"

#include <regex>

BuildComponentASP::BuildComponentASP(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	descriptionPattern.assign(LR"([\[A-Za-zА-Яа-я0-9 -№.:,/"=#\]]{12,})");
	nominalDiameterPattern.assign(LR"(\d{1,4} ?x( ?\d+)?|\d{1,4})");
	amountPattern.assign(LR"( {0,2}\d{1,3}\.\d{1,4}M| {0,2}[1-9]\d{0,2}M?)");
	positionCodePattern.assign(LR"([a-zA-ZА-Яа-я0-9()_]{3,11})");
}

bool BuildComponentASP::trySetPositionCode(const std::wstring& positionCodeStr, ValuesCheker::Type checkType)
{
	std::wregex patternWithoutRusSimbols(L"[a-zA-Z0-9()_]+");
	if (std::regex_match(positionCodeStr, positionCodePattern) && !std::regex_match(positionCodeStr, patternWithoutRusSimbols))
	{
		return BuildComponent::trySetPositionCode(StringUtilities::replaceSimilarRuSimbols(positionCodeStr), checkType);
	}
	return BuildComponent::trySetPositionCode(positionCodeStr, checkType);
}
