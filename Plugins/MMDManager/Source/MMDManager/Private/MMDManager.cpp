// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMDManager.h"
#include "MMDManagerStyle.h"
#include "MMDManagerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName MMDManagerTabName("MMDManager");

#define LOCTEXT_NAMESPACE "FMMDManagerModule"

void FMMDManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMMDManagerStyle::Initialize();
	FMMDManagerStyle::ReloadTextures();

	FMMDManagerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMMDManagerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMMDManagerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMMDManagerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MMDManagerTabName, FOnSpawnTab::CreateRaw(this, &FMMDManagerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMMDManagerTabTitle", "MMDManager"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMMDManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMMDManagerStyle::Shutdown();

	FMMDManagerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MMDManagerTabName);
}

TSharedRef<SDockTab> FMMDManagerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FMMDManagerModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("MMDManager.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FMMDManagerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MMDManagerTabName);
}

void FMMDManagerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMMDManagerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMMDManagerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMMDManagerModule, MMDManager)