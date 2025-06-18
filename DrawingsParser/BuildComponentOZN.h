#pragma once

#ifndef BuildComponentOZN_h__
#define BuildComponentOZN_h__

#include "BuildComponent.h"

class BuildComponentOZN : public BuildComponent
{
public:
	explicit BuildComponentOZN(const std::wstring& positionNumberStr);
};

#endif // BuildComponentOZN_h__
