#include "BuildComponentASP.h"

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
		return BuildComponent::trySetPositionCode(replaceRuSimbols(positionCodeStr), assertionCheck);
	}
	return BuildComponent::trySetPositionCode(positionCodeStr, assertionCheck);
}

std::wstring BuildComponentASP::replaceRuSimbols(const std::wstring& sourceStr)
{
	std::wstring newStr;
	for (wchar_t simbol : sourceStr)
	{
		if (!((simbol >= L'A' && simbol <= L'Z') || (simbol >= L'a' && simbol <= L'z')))
		{
			switch (simbol)
			{
			case L'б': simbol = L'C'; break;
			case L'ё': simbol = L'c'; break;
			case L'Х': simbol = L'E'; break;
			case L'х': simbol = L'e'; break;
			case L'в': simbol = L'T'; break;
			case L'ѓ': simbol = L'y'; break;
			case L'Ю': simbol = L'O'; break;
			case L'ю': simbol = L'o'; break;
			case L'а': simbol = L'P'; break;
			case L'№': simbol = L'p'; break;
			case L'Р': simbol = L'A'; break;
			case L'р': simbol = L'a'; break;
			case L'е': simbol = L'X'; break;
			case L'ѕ': simbol = L'x'; break;
			case L'Т': simbol = L'B'; break;
			case L'Ь': simbol = L'M'; break;
			default:
				break;
			}
		}
		newStr += simbol;
	}
	return newStr;
}
