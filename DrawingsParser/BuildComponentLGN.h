#pragma once

#include "BuildComponent.h"

class BuildComponentLGN : public BuildComponent
{
public:
	explicit BuildComponentLGN(const std::wstring& positionNumberStr);
	bool trySetAmount(const std::wstring& amountStr, bool assertionCheck = true) override;
};

