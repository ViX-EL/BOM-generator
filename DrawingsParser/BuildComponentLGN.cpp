#include "BuildComponentLGN.h"
#include "ValuesCheker.h"

BuildComponentLGN::BuildComponentLGN(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	descriptionPattern.assign(LR"([A-Za-zА-Яа-я0-9 -=./)(Ёё"]{12,})");
	nominalDiameterPattern.assign(LR"(\d{1,4} ?x( ?\d+)?|\d{1,4})");
	amountPattern.assign(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
	documentPattern.assign(LR"([0-9a-zA-Z"\-\/ .;,()ЁёА-Яa-я]+)");
}

bool BuildComponentLGN::trySetAmount(const std::wstring& amountStr, ValuesCheker::Type checkType)
{
	if (amountStr == L"None") {
		return true;
	}
	return BuildComponent::trySetAmount(amountStr, checkType);
}
