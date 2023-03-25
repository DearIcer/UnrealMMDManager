#pragma once
#include "Editor.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Math/Color.h"
#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"


namespace DebugHeader
{
	/************************************************************************/
	/* 将信息打印到视口                                                        */
	/************************************************************************/
	static void PrintMessage(const FString & Message, const FColor &Color)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,8,Color,Message);
		}
	}
	/************************************************************************/
	/* 将信息打印到Log                                                        */
	/************************************************************************/
	static void PrintUELog(const FString &Message)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"),*Message);

	}
	/************************************************************************/
	/* 弹出一个对话框，按钮为YES，NO .默认类型为警告							    */
	/************************************************************************/
	static EAppReturnType::Type ShowMesDialog(EAppMsgType::Type MsgType, const FString& Message, bool bShowMesAsWarning = true, FText MesTitle = FText::FromString(TEXT("Warning")))
	{
		if (bShowMesAsWarning)
		{
			return FMessageDialog::Open(MsgType, FText::FromString(Message), &MesTitle);
		}
		else
		{
			return FMessageDialog::Open(MsgType, FText::FromString(Message));
		}
	}

	/************************************************************************/
	/* 弹出一个通知列表													    */
	/************************************************************************/
	static void ShowNotifyinfo(const FString & Message)
	{
		//定义一个通知列表
		FNotificationInfo NotifyInfo(FText::FromString(Message));
		//使用大字体
		NotifyInfo.bUseLargeFont = true;
		//淡出时间
		NotifyInfo.FadeOutDuration = 8.f;
		//添加到编辑器中，需要一个FSlateNotificationManager管理器
		FSlateNotificationManager::Get().AddNotification(NotifyInfo);
	}

}
