#include "BuildComponentASP.h"
#include "StringUtilities.h"

#include <regex>

BuildComponentASP::BuildComponentASP(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	descriptionPattern.assign(LR"([\[A-Za-zР-пр-џ0-9 -Й.:,/"=#\]]{12,})");
	nominalDiameterPattern.assign(LR"(\d{1,4} ?x( ?\d+)?|\d{1,4})");
	amountPattern.assign(LR"( {0,2}\d{1,3}\.\d{1,4}M| {0,2}[1-9]\d{0,2}M?)");
	positionCodePattern.assign(LR"([a-zA-ZР-пр-џ0-9()_]{3,11})");
}

bool BuildComponentASP::trySetPositionCode(const std::wstring& positionCodeStr, bool assertionCheck)
{
	std::wregex patternWithoutRusSimbols(L"[a-zA-Z0-9()_]+");
	if (std::regex_match(positionCodeStr, positionCodePattern) && !std::regex_match(positionCodeStr, patternWithoutRusSimbols))
	{
		return BuildComponent::trySetPositionCode(StringUtilities::replaceSimilarRuSimbols(positionCodeStr), assertionCheck);
	}
	return BuildComponent::trySetPositionCode(positionCodeStr, assertionCheck);
}
