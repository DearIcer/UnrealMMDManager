// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvanceDeletion/AdvanceDeletionTab.h"
#include "AssetRegistry/AssetData.h"
#include "Components/VerticalBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include"DebugHeader.h"

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
				SNew(SListView<TSharedPtr<FAssetData>>)// 创建列表视图
				.ItemHeight(24.f)
				.ListItemsSource(&AssetDatasUnderSelectedFolderArray)// 列表项数据源
				.OnGenerateRow(this,&SAdvanceDeletionTab::OnGenerateRowForList)// 列表项生成委托
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
TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay,
	const TSharedRef<STableViewBase>& OwnerTable)
{

	if (!AssetDataToDisplay->IsValid())
	{
		return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	}

	// 获取要显示的资产名称
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();

	// 使用 SNew 宏创建一个新的 STableRow 对象，模板参数为 TSharedPtr<FAssetData>。
	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			// 在 STableRow 对象中创建一个 STextBlock 对象，用于显示要显示的资产名称。
			/*SNew(STextBlock)
			.Text(FText::FromString(DisplayAssetName))*/
			// 在 STableRow 对象中创建一个 STextBlock 对象，用于显示要显示的资产名称。
				//SNew(STextBlock)
				//.Text(FText::FromString(DisplayAssetName))
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
			//3.显示的资产名
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(DisplayAssetName))
			]
			//4.按钮
	];

	
	// 返回一个 TSharedRef<ITableRow> 对象，其中包含 ListViewRowWidget。
	return ListViewRowWidget;
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

