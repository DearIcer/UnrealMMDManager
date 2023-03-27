// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMMDManagerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	/************************************************************************/
	/*选中文件夹路径	                                                        */
	/************************************************************************/
	TArray<FString> FolderPathsSelected;
	/************************************************************************/
	/*创建内容浏览器扩展菜单                                                    */
	/************************************************************************/
	void InitCBMenuExtention();
	/************************************************************************/
	/*创建扩展对象	                                                        */
	/************************************************************************/
	TSharedRef<FExtender>CustomCBMenuExtender(const TArray<FString>& SelectedPaths);
	/************************************************************************/
	/*添加扩展菜单条目	                                                        */
	/************************************************************************/
	void AddCBMenuEntry(class FMenuBuilder& MenuBuilder);
	/************************************************************************/
	/*删除未使用资产	                                                        */
	/************************************************************************/
	void OnDeleteUnsuedAssetButtonClicked();
	/************************************************************************/
	/*删除空文件夹	                                                        */
	/************************************************************************/
	void OnDeleteEmptyFolder();
	/************************************************************************/
	/*修复资产引用	                                                        */
	/************************************************************************/
	void FixUpRedirectors();
	/************************************************************************/
	/*空函数，不进行任何操作                                                    */
	/************************************************************************/
	void NullFun();
	/************************************************************************/
	/*高级选项点击事件					                                        */
	/************************************************************************/
	void OnAdvanceDeleteonButtonClicked();
#pragma region CustomEditorTab
	/************************************************************************/
	/*注册名为“AdvanceDeletion”的全局选项卡                                     */
	/************************************************************************/
	void RegisterAdvanceDeletionTab();
	/************************************************************************/
	/*当名为“AdvanceDeletion”的选项卡被生成时，将会调用该函数                      */
	/************************************************************************/
	TSharedRef<SDockTab>OnSpawnAdvanceDeltionTab(const FSpawnTabArgs& SpawnTabArgs);
#pragma endregion 
};
