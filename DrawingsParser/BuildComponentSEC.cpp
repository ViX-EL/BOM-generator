#include "BuildComponentSEC.h"

BuildComponentSEC::BuildComponentSEC(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	descriptionPattern.assign(LR"([A-Za-zА-Яа-я0-9 /"\-:\(\)\,\.&=+%_# ]{11,})");
	nominalDiameterPattern.assign(LR"(\d{1,4}(?:\.\d{1,4})?\/\d{1,4}(?:\.\d{1,4})?|\d{1,4} ?x(?: ?\d{1,4})?|\d{1,4})");
	positionCodePattern.assign(LR"([A-Z0-9-_]{4,16})");
	amountPattern.assign(LR"( {0,2}\d{1,3}(?:\.\d)?M?)");
}
