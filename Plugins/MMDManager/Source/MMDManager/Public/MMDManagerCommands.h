// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MMDManagerStyle.h"

class FMMDManagerCommands : public TCommands<FMMDManagerCommands>
{
public:

	FMMDManagerCommands()
		: TCommands<FMMDManagerCommands>(TEXT("MMDManager"), NSLOCTEXT("Contexts", "MMDManager", "MMDManager Plugin"), NAME_None, FMMDManagerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};