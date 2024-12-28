#include "BuildComponentNAG.h"

BuildComponentNAG::BuildComponentNAG(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	descriptionPattern.assign(LR"([A-Za-zÀ-ßà-ÿ0-9 .,…/"()=\-–\”_]{12,})");
	nominalDiameterPattern.assign(LR"(\d{1,4} x( \d+)?|\d{1,4})");
	amountPattern.assign(LR"( {0,2}\d{1,3}(\.\d)?M?)");
}
