#pragma once

#include "BuildComponent.h"

class BuildComponentASP : public BuildComponent
{
public:
	explicit BuildComponentASP(const std::wstring& positionNumberStr);
	bool trySetPositionCode(const std::wstring& positionCodeStr, bool assertionCheck = true) override;
};

