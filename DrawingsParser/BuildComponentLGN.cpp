#include "BuildComponentLGN.h"

BuildComponentLGN::BuildComponentLGN(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	descriptionPattern.assign(LR"([A-Za-zÀ-ßà-ÿ0-9 -=./)(¨¸"]{12,})");
	nominalDiameterPattern.assign(LR"(\d{1,4} ?x( ?\d+)?|\d{1,4})");
	amountPattern.assign(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
	documentPattern.assign(LR"([0-9a-zA-Z"\-\/ .;,()¨¸À-ßa-ÿ]+)");
}

bool BuildComponentLGN::trySetAmount(const std::wstring& amountStr, bool assertionCheck)
{
	if (amountStr == L"None") {
		return true;
	}
	return BuildComponent::trySetAmount(amountStr, assertionCheck);
}
