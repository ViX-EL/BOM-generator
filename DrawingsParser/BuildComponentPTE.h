#pragma once

#ifndef BuildComponentPTE_h__
#define BuildComponentPTE_h__

#include "BuildComponent.h"

class BuildComponentPTE : public BuildComponent
{
public:
	explicit BuildComponentPTE(const std::wstring& positionNumberStr);
};

#endif // BuildComponentPTE_h__
