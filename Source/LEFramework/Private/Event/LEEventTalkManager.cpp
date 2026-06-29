//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "Event/LEEventTalkManager.h"



void ULEEventTalkManager::AddTalkData(const FLEMessageData& InData)
{
	if (MessageWindow)
	{
		TalkData.Add(InData);

		// 会話開始
		StartTalk();
	}
}

void ULEEventTalkManager::AddTalkDataArray(const TArray<FLEMessageData>& InData)
{
	if (MessageWindow)
	{
		TalkData.Append(InData);

		// 会話開始
		StartTalk();
	}
}

void ULEEventTalkManager::ClearTalkData()
{
	TalkData.Empty();
}

void ULEEventTalkManager::CancelTalk()
{
	if (HasTalkData())
	{
		TalkData.Empty();
		EndTalk();
	}
}

void ULEEventTalkManager::NextMessage()
{
	if (!TalkData.IsEmpty())
	{
		// メッセージの消費
		TalkData.RemoveAt(0);

		if (!TalkData.IsEmpty())
		{
			StartMessage(TalkData[0]);
		}
		else
		{
			// 会話終了
			EndTalk();
		}
	}
}

bool ULEEventTalkManager::IsTalking() const
{
	// 会話ウィンドウのチェック
	if (MessageWindow && MessageWindow->IsInViewport())
	{
		return true;
	}

	// 会話データのチェック
	return HasTalkData();
}


void ULEEventTalkManager::RegisterMessageWindow(ULEMessageWindow* InWindow)
{
	// 現在の登録を解除
	UnregisterMessageWindow();

	// 新規登録
	if (InWindow)
	{
		MessageWindow = InWindow;
		MessageWindow->ReceiveAutoPlay(bAutoPlay);

		// イベントのバインド
		MessageWindow->OnMessageFinished.AddUniqueDynamic(this, &ThisClass::NextMessage);
	}
}

void ULEEventTalkManager::UnregisterMessageWindow()
{
	if (MessageWindow)
	{
		// 既に行われている会話を停止する
		CancelTalk();

		// イベントのアンバインド
		MessageWindow->OnMessageFinished.RemoveDynamic(this, &ThisClass::NextMessage);
		MessageWindow = nullptr;
	}
}

void ULEEventTalkManager::SetAutoPlay(const bool bEnable)
{
	if (bAutoPlay != bEnable)
	{
		bAutoPlay = bEnable;

		if (MessageWindow)
		{
			MessageWindow->ReceiveAutoPlay(bAutoPlay);
		}
	}
}


void ULEEventTalkManager::StartTalk()
{
	if (MessageWindow && HasTalkData())
	{
		MessageWindow->AddToViewport();

		StartMessage(TalkData[0]);
		OnTalkStart.Broadcast();
	}
}

void ULEEventTalkManager::EndTalk()
{
	if (MessageWindow)
	{
		MessageWindow->Close();
		OnTalkEnd.Broadcast();
	}
}

void ULEEventTalkManager::StartMessage(const FLEMessageData& InData)
{
#if 0
	if (MessageWindow)
	{
		MessageWindow->SetMessageData(InData);
	}
#endif
}