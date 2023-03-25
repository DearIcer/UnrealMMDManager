// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMDManager.h"

#include "AssetToolsModule.h"
#include "MMDManagerStyle.h"
#include "MMDManagerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "ContentBrowserModule.h"
#include"DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"


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
		FolderPathsSelected = SelectedPaths;
	}
	return MenuExtender;
}
void FMMDManagerModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	// 向菜单构建器添加一个菜单条目
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("---------MMD工具集选项---------")),
		FText::FromString(TEXT("遇到Bug记得告诉大冰！")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FMMDManagerModule::NullFun)
	);
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("删除未使用资产")),
		FText::FromString(TEXT("这个操作会将当前文件夹下的未使用资产删除！")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FMMDManagerModule::OnDeleteUnsuedAssetButtonClicked)
	);
	MenuBuilder.AddMenuEntry
	(
	FText::FromString(TEXT("删除空文件夹")),
	FText::FromString(TEXT("这个操作会删除所有空文件夹")),
	FSlateIcon(),
	FExecuteAction::CreateRaw(this,&FMMDManagerModule::OnDeleteEmptyFolder)
	);

	MenuBuilder.AddMenuEntry
	(
	FText::FromString(TEXT("---------MMD工具集选项---------")),
	FText::FromString(TEXT("遇到Bug记得告诉大冰！")),
	FSlateIcon(),
	FExecuteAction::CreateRaw(this, &FMMDManagerModule::NullFun)
	);
}
void FMMDManagerModule::OnDeleteUnsuedAssetButtonClicked()
{
	// 如果选中的文件夹数量大于1，则提示用户只能选择一个文件夹进行操作，并返回
	if (FolderPathsSelected.Num() > 1)
	{
		DebugHeader::ShowMesDialog(EAppMsgType::Ok,TEXT("只能选一个文件夹进行操作！"));
		return;
	}
	// 获取选中文件夹内的所有资产路径和名称
	TArray<FString> AssetsPathName = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);
	// 如果文件夹为空，则提示用户并返回
	if (AssetsPathName.Num() == 0)
	{
		DebugHeader::ShowMesDialog(EAppMsgType::Ok,TEXT("文件夹为空！"));
		return;
	}
	// 弹出确认对话框，询问用户是否删除未使用的资产
	EAppReturnType::Type ConfirmResult =
		DebugHeader::ShowMesDialog(EAppMsgType::YesNo,TEXT("一共有 ")
			+ FString::FromInt(AssetsPathName.Num()) + TEXT(" 个资产！是否继续？"));
	// 如果用户选择“否”，则返回
	if (ConfirmResult == EAppReturnType::No)
	{
		return;
	}
	// 修复引用
	FixUpRedirectors();
	// 定义一个数组，用于保存未使用的资产数据
	TArray<FAssetData>  UnusedAssetsDataArray;
	// 遍历选中文件夹内的所有资产，检查其是否未被使用，并将未被使用的资产数据保存到数组中
	for (const FString& AssetPathName : AssetsPathName)
	{
		// 过滤引擎内容
		if (AssetPathName.Contains(TEXT("Developers")) ||
			AssetPathName.Contains(TEXT("Collections")))
		{
			continue;
		}
		// 如果资产不存在，则跳过此资产
		if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName))
		{
			continue;
		}
		// 查找引用此资产的资产数量，如果为0，则将此资产添加到未使用的资产数据数组中
		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPathName);
		if (AssetReferencers.Num() == 0)
		{
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);
			UnusedAssetsDataArray.Add(UnusedAssetData);
		}
	}
	// 如果未使用的资产数据数组不为空，则删除其中的所有资产
	if (UnusedAssetsDataArray.Num() > 0)
	{
		ObjectTools::DeleteAssets(UnusedAssetsDataArray);
	}
	else
	{
		// 如果未使用的资产数据数组为空，则提示用户未找到未使用资产
		DebugHeader::ShowMesDialog(EAppMsgType::Ok,TEXT("未找到未使用资产！"));
	}
}

void FMMDManagerModule::OnDeleteEmptyFolder()
{
	FixUpRedirectors();
	// 列出所选文件夹中的所有资产
	TArray<FString> FolderPathsArray = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0],true,true);
	int32 Counter = 0;
	// 用于存储空文件夹的路径名和路径名数组
	FString EmptyFolderPathsNames;
	TArray<FString> EmptyFolderPathsNamesArray;
	for(const FString& FolderPath : FolderPathsArray)
	{
		if (FolderPath.Contains(TEXT("Developers")) ||
			FolderPath.Contains(TEXT("Collections")))
		{
			continue;
		}
		if(!UEditorAssetLibrary::DoesDirectoryExist(FolderPath))
		{
			continue;
		}
		// 如果文件夹不包含资产，则将其添加到空文件夹路径名和路径名数组中
		if(!UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath))
		{
			EmptyFolderPathsNames.Append(FolderPath);
			EmptyFolderPathsNames.Append(TEXT("\n"));
			EmptyFolderPathsNamesArray.Add(FolderPath);
		}
	}
	if (EmptyFolderPathsNamesArray.Num() == 0)
	{
		DebugHeader::ShowNotifyinfo(TEXT("选中文件夹不为空！"));
		return;;
	}
	EAppReturnType::Type ConfirmResult =
		DebugHeader::ShowMesDialog(EAppMsgType::OkCancel,
			TEXT("空文件夹有: \n") + EmptyFolderPathsNames + TEXT("\n 你真的要删除它吗?"),
			false);
	if (ConfirmResult == EAppReturnType::Cancel)
	{
		return;
	}
	// 对于每个空文件夹，尝试删除它
	for (const FString& EmptyFolderPath : EmptyFolderPathsNamesArray)
	{
		UEditorAssetLibrary::DeleteDirectory(EmptyFolderPath)
		?
			++Counter : DebugHeader::ShowNotifyinfo(TEXT("删除失败") + EmptyFolderPath);
	}
	if (Counter > 0)
	{
		DebugHeader::ShowNotifyinfo(TEXT("删除成功") + FString::FromInt(Counter) + TEXT("个文件夹"));
	}
}

void FMMDManagerModule::FixUpRedirectors()
{
	// 创建一个指向 UObjectRedirector 对象的 TArray 数组
	TArray<UObjectRedirector*> RedirectorsToFixArray;

	//如果模块已经加载，则返回实例
	// 使用 FAssetRegistryModule 类，我们可以从 AssetRegistry 模块中检索到现有的资产
	FAssetRegistryModule& AssetRegistryModule =
	FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	//拿到模块后，可以对容器里的对象进行控制

	//设置过滤器
	// 创建一个 Asset Registry Filter，以获取所有的 ObjectRedirector 资产
	FARFilter Filter;
	Filter.bRecursivePaths = true;// 遍历所有子文件夹
	Filter.PackagePaths.Emplace("/Game");// 只检查 /Game 文件夹下的资源
	Filter.ClassNames.Emplace("ObjectRedirector");// 只检查 ObjectRedirector 类型的资源
	// 使用 AssetRegistry 模块获取所有符合 Filter 的资产数据
	TArray<FAssetData> OutRedirectors;
	AssetRegistryModule.Get().GetAssets(Filter,OutRedirectors);

	// 遍历符合 Filter 的资产数据，如果是 ObjectRedirector 类型的资产，就添加到 RedirectorsToFixArray 数组中
	for (const FAssetData & RedirectorsData : OutRedirectors)
	{
		if (UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorsData.GetAsset()))
		{
			RedirectorsToFixArray.Add(RedirectorToFix);
		}

		// 加载 AssetTools 模块，使用其 FixupReferencers 函数来修复所有指向 RedirectorsToFixArray 中的 ObjectRedirector 资产的引用
		FAssetToolsModule & AssetToolsModule =
			FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
		AssetToolsModule.Get().FixupReferencers(RedirectorsToFixArray);
	}
}

void FMMDManagerModule::NullFun()
{
}
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMMDManagerModule, MMDManager)