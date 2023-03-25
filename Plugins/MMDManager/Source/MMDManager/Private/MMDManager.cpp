// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMDManager.h"
#include "MMDManagerStyle.h"
#include "MMDManagerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "ContentBrowserModule.h"


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
	InitCBMenuExtention();
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
void FMMDManagerModule::InitCBMenuExtention()
{
	// 获取 ContentBrowser 模块的引用
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	// 获取 ContentBrowser 模块的所有扩展菜单项
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders =
		ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	// 创建一个自定义菜单项扩展代理
	// FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	//CustomCBMenuDelegate.BindRaw(this,&FSuperManagerModule::CustomCBMenuExtender);
	// 将自定义菜单项扩展代理添加到 Content Browser 的菜单项扩展列表中
	//ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate);
	ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::
		CreateRaw(this, &FMMDManagerModule::CustomCBMenuExtender));
}
TSharedRef<FExtender> FMMDManagerModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	// 创建一个扩展对象
	TSharedRef<FExtender> MenuExtender(new FExtender);
	// 如果选中的文件夹不为空，则添加一个菜单扩展
	if (SelectedPaths.Num() > 0)
	{
		// 向菜单扩展中添加一个菜单条目
		MenuExtender->AddMenuExtension(FName("Delete"),// 扩展到 "Delete" 菜单后面
			EExtensionHook::After, // 在菜单末尾添加扩展
			TSharedPtr<FUICommandList>(),// 不使用命令列表
			FMenuExtensionDelegate::CreateRaw(this, &FMMDManagerModule::AddCBMenuEntry) // 绑定动作
		);
	}
	return MenuExtender;
}
void FMMDManagerModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	// 向菜单构建器添加一个菜单条目
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Delete Unsed Assets")),
		FText::FromString(TEXT("Safely delete all unused assets under folder")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FMMDManagerModule::OnDeleteUnsuedAssetButtonClicked)
	);
}
void FMMDManagerModule::OnDeleteUnsuedAssetButtonClicked()
{

}
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMMDManagerModule, MMDManager)