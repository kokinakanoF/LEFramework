//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Interaction/LERadioButtonManager.h"
#include "UI/Interaction/LEToggleButton.h"



void ULERadioButtonManager::Register(ULERadioButton* InButton)
{
	if (InButton && !RadioButtons.Contains(InButton))
	{
		// イベントのバインド＆配列に追加
		InButton->OnChecked.AddDynamic(this, &ThisClass::HandleCheckedButtonChanged);
		RadioButtons.Add(InButton);
	}
	else
	{
		//UE_LOG(Log_Temp, Warning, TEXT("The button is invalid or already registered"));
	}
}

void ULERadioButtonManager::Unregister(ULERadioButton* InButton)
{
	if (RadioButtons.Contains(InButton))
	{
		// イベントのアンバインド
		InButton->OnChecked.RemoveDynamic(this, &ThisClass::HandleCheckedButtonChanged);
	}
	else
	{
		//UE_LOG(Log_Temp, Warning, TEXT("Couldn't find specific RadioButton"));
	}
}

void ULERadioButtonManager::UnregisterAll()
{
	for (ULERadioButton* rb : RadioButtons)
	{
		if (rb)
		{
			// イベントのアンバインド
			rb->OnChecked.RemoveDynamic(this, &ThisClass::HandleCheckedButtonChanged);
		}
	}

	RadioButtons.Empty();
}


void ULERadioButtonManager::ResetCheck()
{
	if (TObjectPtr<ULERadioButton> rb = GetCheckedButton())
	{
		// 選択解除
		rb->ForceUncheck();
		// イベントの実行
		OnCheckedButtonChanged.Broadcast(nullptr, INDEX_NONE);
	}
}


ULERadioButton* ULERadioButtonManager::GetCheckedButton() const
{
	for (ULERadioButton* rb : RadioButtons)
	{
		if (rb && rb->IsChecked())
		{
			return rb;
		}
	}

	return nullptr;
}

int32 ULERadioButtonManager::GetCheckedButtonIndex() const
{
	for (int32 i = 0; i < RadioButtons.Num(); i++)
	{
		TObjectPtr<ULERadioButton> rb = RadioButtons[i];
		if (rb && rb->IsChecked())
		{
			return i;
		}
	}

	return INDEX_NONE;
}


void ULERadioButtonManager::HandleCheckedButtonChanged(ULEToggleButton* InButton)
{
	TObjectPtr<ULERadioButton> rb = Cast<ULERadioButton>(InButton);

	int32 newIndex = RadioButtons.Find(rb);
	if (newIndex == INDEX_NONE)
	{
		//UE_LOG(Log_Temp, Warning, TEXT("Couldn't find specific RadioButton"));
	}
	else
	{
		// 現在選択中のボタンをオフにする
		if (ULERadioButton* lastChecked = GetCheckedButton())
		{
			lastChecked->ForceUncheck();
		}

		// イベントの実行
		OnCheckedButtonChanged.Broadcast(rb, newIndex);
	}
}