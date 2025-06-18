#pragma once

#ifndef BuildComponentLGN_h__
#define BuildComponentLGN_h__

#include "BuildComponent.h"
#include "ValuesCheker.h"

class BuildComponentLGN : public BuildComponent
{
public:
	explicit BuildComponentLGN(const std::wstring& positionNumberStr);
	bool trySetAmount(const std::wstring& amountStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
};

#endif // BuildComponentLGN_h__
