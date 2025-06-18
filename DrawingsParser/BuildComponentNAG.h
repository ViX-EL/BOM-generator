#pragma once

#ifndef BuildComponentNAG_h__
#define BuildComponentNAG_h__

#include "BuildComponent.h"

class BuildComponentNAG : public BuildComponent
{
public:
	explicit BuildComponentNAG(const std::wstring& positionNumberStr);
};

#endif // BuildComponentNAG_h__
