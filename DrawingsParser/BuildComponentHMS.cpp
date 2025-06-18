#include "BuildComponentHMS.h"

BuildComponentHMS::BuildComponentHMS(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	descriptionPattern.assign(LR"([A-Za-zА-Яа-я0-9 \-\.\/«»\(\)\,–"]{12,})");
	nominalDiameterPattern.assign(LR"(\d{1,4} ?x( ?\d+)?|[1-9]\d{0,4})");
	amountPattern.assign(LR"( {0,2}\d{1,3}(\.\d)?M?)");
}
