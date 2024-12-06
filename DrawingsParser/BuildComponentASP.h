#pragma once

#include "BuildComponent.h"

class BuildComponentASP : public BuildComponent
{
	bool trySetDescription(const std::wstring& descriptionStr, bool assertionCheck = true) override;
	bool trySetNominalDiameter(const std::wstring& nominalDiameterStr, bool assertionCheck = true) override;
	bool trySetAmount(const std::wstring& amountStr, bool assertionCheck = true) override;
	bool trySetPositionCode(const std::wstring& positionCodeStr, bool assertionCheck = true);
};

