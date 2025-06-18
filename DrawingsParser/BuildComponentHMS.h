#pragma once

#ifndef BuildComponentHMS_h__
#define BuildComponentHMS_h__

#include "BuildComponent.h"

class BuildComponentHMS : public BuildComponent
{
public:
	explicit BuildComponentHMS(const std::wstring& positionNumberStr);
};

#endif // BuildComponentHMS_h__
