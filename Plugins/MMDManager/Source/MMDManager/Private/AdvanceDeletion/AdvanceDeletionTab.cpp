// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvanceDeletion/AdvanceDeletionTab.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	// 允许该组件获取焦点
	bCanSupportFocus = true;
	// 设置组件的子槽（ChildSlot），并添加一个名为STextBlock的Slate组件，其文本内容来自构造参数InArgs的TestString字段
	ChildSlot[SNew(STextBlock).Text(FText::FromString(InArgs._TestString))];
}
