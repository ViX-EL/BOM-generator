#include "BuildComponentIOT.h"

BuildComponentIOT::BuildComponentIOT(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	initPatterns();
}

BuildComponentIOT::BuildComponentIOT(int positionNumber) : BuildComponent(positionNumber)
{
	initPatterns();
}

void BuildComponentIOT::initPatterns()
{
	descriptionPattern.assign(LR"([A-Za-zА-Яа-я0-9 \-="#.,\/\\⁄°:–)(Ёё№Ø«»]{12,})");
	nominalDiameterPattern.assign(LR"(\d{1,4} ?x( ?\d+)?|\d{1,4})");
	amountPattern.assign(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
	positionCodePattern.assign(LR"([a-zA-ZА-Яа-я0-9()_-]{3,11})");
}
