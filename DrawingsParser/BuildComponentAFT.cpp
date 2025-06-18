#include "BuildComponentAFT.h"

BuildComponentAFT::BuildComponentAFT(const std::wstring& positionNumberStr) : BuildComponentASP(positionNumberStr)
{
	nominalDiameterPattern.assign(LR"(\d{1,4} ?x?(?: ?\d+)? ?x?(?: ?\d+)?|\d\/\d IN)");
}
