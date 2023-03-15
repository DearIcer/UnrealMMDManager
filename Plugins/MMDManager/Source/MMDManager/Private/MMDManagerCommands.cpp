// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMDManagerCommands.h"

#define LOCTEXT_NAMESPACE "FMMDManagerModule"

void FMMDManagerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MMDManager", "Bring up MMDManager window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
