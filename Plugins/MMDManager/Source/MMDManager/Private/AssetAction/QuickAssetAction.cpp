// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetAction/QuickAssetAction.h"
#include"DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "Blutility/Public/EditorUtilityLibrary.h"
#include "Materials/MaterialInstanceConstant.h"
#include "ObjectTools.h"
/************************************************************************/
/*批量复制资产操作                                                         */
/************************************************************************/
void UQuickAssetAction::DuplicateAssets(int32 Num)
{
	//首先，它检查 Num 是否小于等于零。如果是，则使用 PrintMessage 函数打印一条红色消息并返回。
	if (Num <= 0)
	{
		//PrintMessage(TEXT("Please enter a correct number"), FColor::Red);
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Please enter a correct number!")));
		ShowMesDialog(EAppMsgType::Ok, TEXT("Please enter a correct number"));
		return;
	}
	//获取选定的资产数据并将其存储在 SelectedAssetsData 数组中。
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 counter = 0;

	//对于每个选定的资产数据，它会循环 Num 次，并为每次循环创建一个新的重复资产名称。新名称由原始资产名称、下划线和循环计数器组成。
	for (const FAssetData& SelectedAssetData : SelectedAssetsData)
	{
		for (int32 i = 0; i < Num; i++)
		{
			const FString SourceAssetPath = SelectedAssetData.ObjectPath.ToString();
			const FString NewDuplicateAssetName = SelectedAssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i);
			//使用 FPaths::Combine 函数将包路径和新重复资产名称组合成新路径名。
			const FString NewPathName = FPaths::Combine(SelectedAssetData.PackagePath.ToString(), NewDuplicateAssetName);
			PrintMessage(NewPathName, FColor::Red);
			//如果复制成功，则使用 UEditorAssetLibrary::SaveAsset 函数保存新资产，并增加计数器。
			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName))
			{
				UEditorAssetLibrary::SaveAsset(NewPathName, false);
				++counter;
			}
		}
	}
	//如果计数器大于零，则使用 PrintMessage 函数打印一条绿色消息表示成功复制了多少个文件。
	if (counter > 0)
	{
		// PrintMessage(TEXT("Successfully Duplicated" + FString::FromInt(counter) + "File"), FColor::Green);
		ShowNotifyinfo(TEXT("Successfully Duplicated" + FString::FromInt(counter) + "File"));
	}
}
/************************************************************************/
/*批量自动添加前缀                                                         */
/************************************************************************/
void UQuickAssetAction::AddPrefixes()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	int32 succeed = 0;
	int32 failed = 0;
	for (UObject * SelectedObject : SelectedObjects)
	{
		//如果对象无效，则跳过这次循环
		if (!SelectedObject)
		{
			failed++;
			continue;
		}
		//匹配map里的类名
		const FString* Prefix = PrefixMap.Find(SelectedObject->GetClass());
		//确认Prefix是否有效
		if(!Prefix|| Prefix->IsEmpty())
		{
			PrintMessage(TEXT("Failed to find prefix for class") + SelectedObject->GetClass()->GetName(),FColor::Red);
			failed++;
			continue;
		}
		//获取资产原有名字
		FString OldName = SelectedObject->GetName();
		//StartsWith是寻找 以。。。开头，
		//如果选中资产已经重命名。则跳过
		if(OldName.StartsWith(*Prefix))
		{
			PrintMessage(OldName + ("Already has prefix added"), FColor::Red);
			failed++;
			continue;
		}
		if(SelectedObject->IsA<UMaterialInstanceConstant>())
		{
			OldName.RemoveFromStart(TEXT("M_"));
			OldName.RemoveFromStart(TEXT("_Inst"));
		}
		//前缀+旧名字
		const FString NewName = *Prefix + OldName;
		UEditorUtilityLibrary::RenameAsset(SelectedObject,NewName);
		++succeed;
	}
	//拉个通知告诉用户都干了什么
	ShowNotifyinfo(TEXT("succeed:") + FString::FromInt(succeed) + TEXT("failed:") + FString::FromInt(failed));
}
/************************************************************************/
/*删除未使用资产                                                           */
/************************************************************************/
void UQuickAssetAction::RemoveUnusedAsset()
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetsData;

	//获取资产的引用
	for (const FAssetData & SelectedAssetData : SelectedAssetsData)
	{
		TArray<FString> AssetRefrencers = UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAssetData.ObjectPath.ToString());
		//如果等于0则表示资产没有使用
		if (AssetRefrencers.Num() == 0)
		{
			//将未使用资产加入到另外一个数组
			UnusedAssetsData.Add(SelectedAssetData);
		}
		
		// for (FString & i : AssetRefrencers)
		// {
		// 	PrintMessage(i,FColor::Red);
		// }
	}
	if (UnusedAssetsData.Num() == 0)
	{
		ShowNotifyinfo(TEXT("没有找到未使用资产！"));
		return;
	}
	
	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsData);
	if (NumOfAssetsDeleted == 0)
	{
		return;
	}
	
	ShowNotifyinfo(TEXT("成功删除") + FString::FromInt(NumOfAssetsDeleted) + TEXT("个未使用资产"));
	
}
