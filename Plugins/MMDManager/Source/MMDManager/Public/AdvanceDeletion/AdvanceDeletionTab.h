// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widgets/Views/STableRow.h"
#include "Widgets/SCompoundWidget.h"

// 定义了一个名为SAdvanceDeletionTab的Slate组件类
class SAdvanceDeletionTab : public SCompoundWidget
{
	// SLATE_BEGIN_ARGS 和 SLATE_END_ARGS 宏用于在类定义中声明Slate组件的构造参数
	SLATE_BEGIN_ARGS(SAdvanceDeletionTab){}

	SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>,AssetsDataArray)
	SLATE_END_ARGS()

public:
	// 该函数用于构建SAdvanceDeletionTab组件的UI布局
	void Construct(const FArguments& InArgs);
private:
#pragma region SlateWidget
	TArray<TSharedPtr<FAssetData>> AssetDatasUnderSelectedFolderArray;
	TSharedRef<SListView<TSharedPtr<FAssetData>>> ConstructAssetListView();
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedAssetListView;
	TSharedRef<ITableRow>OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay,
		const TSharedRef<STableViewBase> &OwnerTable);
	void RefreshAssetListView();

	TSharedRef<SCheckBox>ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay);
	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData);

	TSharedRef<STextBlock>ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontInfo);

	FSlateFontInfo GetEmboseedTextFont() const
	{
		return FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	}

	TSharedRef<SButton>ConstructButton(const TSharedPtr<FAssetData>& AssetDataToDisplay);
	FReply OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData);

	TSharedRef<SButton>ConstructDeleteAllButton();
	TSharedRef<SButton>ConstructSelectedButton();
	TSharedRef<SButton>ConstructDelectedButton();

	FReply OnDeleteAllButtonClicked();
	TSharedRef<STextBlock> ConstructTextForTabButton(const FString& Content);

	FReply OnSelectedButtonClicked();
	FReply OnDelectedButtonClicked();
#pragma endregion
};