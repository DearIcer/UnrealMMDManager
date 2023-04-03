// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvanceDeletion/AdvanceDeletionTab.h"
#include "AssetRegistry/AssetData.h"
#include "Components/VerticalBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "MMDManager.h"
#include"DebugHeader.h"

#pragma region SlateWidget
void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	// 允许该组件获取焦点
	bCanSupportFocus = true;
	// 定义字体样式，设置字体大小为 30
	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 30;
	AssetDatasUnderSelectedFolderArray = InArgs._AssetsDataArray;
	// 设置组件的子槽（ChildSlot），并添加一个名为STextBlock的Slate组件，其文本内容来自构造参数InArgs的TestString字段
	ChildSlot
	[
		// 垂直盒子
		SNew(SVerticalBox)

		// 标题
		+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
					.Text(FText::FromString(TEXT("高级删除")))
					.Font(TitleTextFont)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::White)
			]
		// 水平盒子
		+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
			]
			// 滚动列表
		+SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			[
				SNew(SScrollBox)
				+SScrollBox::Slot()
				[
					//SNew(SListView<TSharedPtr<FAssetData>>)// 创建列表视图
					//	.ItemHeight(24.f)
					//	.ListItemsSource(&AssetDatasUnderSelectedFolderArray)// 列表项数据源
					//	.OnGenerateRow(this,&SAdvanceDeletionTab::OnGenerateRowForList)// 列表项生成委托
					ConstructAssetListView()
				]
			]
		// 水平盒子
		+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
			]
	];
}
TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTab::ConstructAssetListView()
{
	ConstructedAssetListView = SNew(SListView<TSharedPtr<FAssetData>>)// 创建列表视图
		.ItemHeight(24.f)
		.ListItemsSource(&AssetDatasUnderSelectedFolderArray)// 列表项数据源
		.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList);// 列表项生成委托

	return ConstructedAssetListView.ToSharedRef();
}
TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay,
	const TSharedRef<STableViewBase>& OwnerTable)
{

	if (!AssetDataToDisplay->IsValid())
	{
		return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	}

	// 获取要显示的资产名称
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();
	const FString DisplayAssetClassName =
		/*AssetDataToDisplay->AssetClass.ToString();*/
		AssetDataToDisplay->GetClass()->GetName();
	FSlateFontInfo DisplayAssetFont = GetEmboseedTextFont();
	DisplayAssetFont.Size = 10;
	// 使用 SNew 宏创建一个新的 STableRow 对象，模板参数为 TSharedPtr<FAssetData>。
	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				//1.多选框
			+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(.05f)
				[
					ConstructCheckBox(AssetDataToDisplay)
				]
			//2.显示的资源类名
			+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Fill)
				.FillWidth(.2f)
				[
					ConstructTextForRowWidget(DisplayAssetClassName, DisplayAssetFont)
				]
			//3.显示的资产名
			+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					/*SNew(STextBlock)
						.Text(FText::FromString(DisplayAssetName))*/
					ConstructTextForRowWidget(DisplayAssetName, DisplayAssetFont)
				]
			//4.按钮
			+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Fill)
				[
					ConstructButton(AssetDataToDisplay)
				]
	];

	
	// 返回一个 TSharedRef<ITableRow> 对象，其中包含 ListViewRowWidget。
	return ListViewRowWidget;
}
void SAdvanceDeletionTab::RefreshAssetListView()
{
	if (ConstructedAssetListView.IsValid())
	{
		ConstructedAssetListView->RebuildList();
	}
}
TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	// 创建 SCheckBox 控件，并将其赋值给 TSharedRef<SCheckBox> 类型的变量 ConstructedCheckBox
	TSharedRef<SCheckBox> ConstructedCheckBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox) // 指定复选框类型
		.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay);
	return ConstructedCheckBox;// 返回构造的复选框控件
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{

	switch (NewState)
	{
	case ECheckBoxState::Unchecked:
		DebugHeader::ShowNotifyinfo(AssetData->AssetName.ToString() + TEXT(" is Unchecked"));
		break;
	case ECheckBoxState::Checked:
		DebugHeader::ShowNotifyinfo(AssetData->AssetName.ToString() + TEXT("is Checked"));
		break;
	case ECheckBoxState::Undetermined:
		break;
	default:
		break;
	}
	
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontInfo)
{
	TSharedRef<STextBlock>ConstructedTextBlock = SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(FontInfo)
		.ColorAndOpacity(FColor::White);
	return ConstructedTextBlock;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructButton(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SButton>ConstructedButton = SNew(SButton)
		.Text(FText::FromString(TEXT("删除")))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteButtonClicked, AssetDataToDisplay);
	return ConstructedButton;

}
FReply SAdvanceDeletionTab::OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData)
{
	FMMDManagerModule& MMDManagerModule =
		FModuleManager::LoadModuleChecked<FMMDManagerModule>(TEXT("MMDManager"));
	const bool bAssetDeleted = MMDManagerModule.DeleteSingleAssetForAssetList(*ClickedAssetData.Get());
	if (bAssetDeleted)
	{
		//刷新列表
		if (AssetDatasUnderSelectedFolderArray.Contains(ClickedAssetData))
		{
			AssetDatasUnderSelectedFolderArray.Remove(ClickedAssetData);
		}
		//---
		RefreshAssetListView();
	}
	else
	{
		DebugHeader::ShowNotifyinfo(TEXT("资产删除失败"));
	}
	return FReply::Handled();
}

#pragma endregion