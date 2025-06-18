#include "BuildComponentFGG.h"

BuildComponentFGG::BuildComponentFGG(const std::wstring& positionNumberStr) : BuildComponent(positionNumberStr)
{
	descriptionPattern.assign(LR"([\[A-Za-zА-Яа-я0-9 \-;,¦/„ѓ#'\.\(\)=Φ%℃]{11,})");
	nominalDiameterPattern.assign(LR"(\d{1,4}( ?x ?\d{1,4})?|\d{1,2}\/\d{1,2}\")");
	amountPattern.assign(LR"( {0,2}\d{1,3}\.\d{1,4}M| {0,2}[1-9]\d{0,2}M?)");
	//documentPattern.assign(LR"([0-9a-zA-Z"\-\/ .;,()ЁёА-Яa-я]+)");
}
