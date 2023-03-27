// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

// 定义了一个名为SAdvanceDeletionTab的Slate组件类
class SAdvanceDeletionTab : public SCompoundWidget
{
	// SLATE_BEGIN_ARGS 和 SLATE_END_ARGS 宏用于在类定义中声明Slate组件的构造参数
	SLATE_BEGIN_ARGS(SAdvanceDeletionTab){}

	// 声明构造参数TestString，它的类型为FString
	SLATE_ARGUMENT(FString,TestString)
	SLATE_END_ARGS()

public:
	// 该函数用于构建SAdvanceDeletionTab组件的UI布局
	void Construct(const FArguments& InArgs);
};