//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/LEMessageWindow.h"
// Widget
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"


#if 0
void ULEMessageWindow::SetMessageData_Implementation(const FLEMessageData& InData)
{
	MessageState = EMessageState::Going;

	// ウィジェットにテキストを設定する
	SetTextToWidget(Txt_SpeakerName, InData.SpeakerName);
	SetTextToWidget(Txt_Message, InData.Message);

	// 各時間の設定
	MessageWaitTime = InData.Message.ToString().Len() * 0.25f;
	MessageAfterWaitTime = 2.0f;

	// イベントの実行
	OnMessageStarted.Broadcast();

	// 自動でオープンする
	Open();
}
#endif

void ULEMessageWindow::ReceiveAutoPlay_Implementation(const bool bAutoPlay)
{
	// 待ち時間の復活
	if (bAutoPlay)
	{
		MessageAfterWaitTime = 2.0f;
	}
}


void ULEMessageWindow::SetTextToWidget(TObjectPtr<UTextLayoutWidget> InWidget, const FText& InText)
{
	// 型に応じて設定する
	if (UTextBlock* tb = Cast<UTextBlock>(InWidget))
	{
		tb->SetText(InText);
	}
	else if (URichTextBlock* rtb = Cast<URichTextBlock>(InWidget))
	{
		rtb->SetText(InText);
	}
}


void ULEMessageWindow::TickMessageGoing(float InDeltaTime)
{
	MessageWaitTime -= InDeltaTime;
	if (MessageWaitTime <= 0.0f)
	{
		MessageState = EMessageState::After;
	}
}

void ULEMessageWindow::TickMessageAfter(float InDeltaTime)
{
	MessageAfterWaitTime -= InDeltaTime;
	if (MessageAfterWaitTime <= 0.0f)
	{
		MessageState = EMessageState::Finished;

		// イベントの実行
		OnMessageFinished.Broadcast();
	}
}


void ULEMessageWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsOpenFinished())
	{
		return;
	}

	// 状態に応じたTickを行う
	switch (MessageState)
	{
	case EMessageState::Going:
	{
		TickMessageGoing(InDeltaTime);
		break;
	}
	case EMessageState::After:
	{
		TickMessageAfter(InDeltaTime);
		break;
	}
	}
}

void ULEMessageWindow::NativeFinishClose()
{
	Super::NativeFinishClose();

	RemoveFromParent();
}